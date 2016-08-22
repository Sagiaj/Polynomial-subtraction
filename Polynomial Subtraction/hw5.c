#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "avl.h"

#define MAX(x,y) ((x>y)?(x):(y))
#define RUN_TIME_TEST_APP

static int debug_level = 0;

/* =================== Polynoms ============================= */

void print_poly(const Poly *p) {
  printf("%s%g*x^%d", (p->coeff>=0) ? "+" : "", p->coeff, p->degree);
}
int print_poly1(const Poly *p,void* params) {
  printf("%s%g*x^%d", (p->coeff>=0) ? "+" : "", p->coeff, p->degree);
  return(0);
}
int compare_poly(const Poly *p1, const Poly *p2) {
  if (p1->degree == p2->degree)
    return(0);
  if (p1->degree < p2->degree)
    return(-1);
  return(1);
}

typedef struct {
  int count;
  double sum;
} MeanAssist;

int mean_coeff(Poly* const p, void *params) {
  MeanAssist *mean_assist = (MeanAssist*)params;
  mean_assist->sum += p->coeff;
  mean_assist->count++;
  return(0);
}

/* ============================================================== */

int is_leaf(const TreeNode* const node) {
  return(!(node->left || node->right));
}

TreeNode* new_tree_node() {
  TreeNode* node = NULL;
  node = (TreeNode*)calloc(1,sizeof(TreeNode));
  return node;
}

void delete_leaf(TreeNode* const node) {
  if (!node)
    return;
  assert(is_leaf(node));
  if (node->parent) {
    node->left_h = -1;
    node->right_h = -1;
    AVL_balance_update(node);
    if (node->parent->left == node)
      node->parent->left = NULL;
    else node->parent->right = NULL;
  };
  free(node);
}

void delete_aid(Tree* AID, TreeNode* aid)
{
	TreeNode *n1,*n2;
	if(aid==NULL)
		return;
	if(aid->left_h>0)
	{
		n1=aid->left;
		if(n1!=NULL)
			delete_aid(AID,n1);
	}
	if(aid->right_h>0)
	{
		n2=aid->right;
		if(n2!=NULL)
			delete_aid(AID,n2);
	}
	AVL_delete(AID,aid->data);
	return;
}

void delete_single(TreeNode* const node) {
  TreeNode *conseq = NULL;
  if (!node)
    return;
  assert(!node->left || !node->right);
  if (node->left)
    conseq = node->left;
  else
    conseq = node->right;
  conseq->parent = node->parent;
  if (node->parent) {
    if (node->parent->left == node)
      node->parent->left = conseq;
    else node->parent->right = conseq;
  };
  AVL_balance_update(conseq);
  free(node);
}

int height(const TreeNode* const node) {
  return(node ? MAX(node->left_h,node->right_h) : -1);
}

int balance(const TreeNode* const node) {
  return(node ? node->left_h-node->right_h : 0);
}

void print_tree(const Tree* const tree, PrintData print_data) {
  print_node(tree->root, print_data);
}

void print_node(const TreeNode* const node, PrintData print_data) {
  if (debug_level > 0) {
    if (node) {
      print_data(&node->data);
      if (debug_level > 1)
        printf("[%d,%d]:(L:", node->left_h, node->right_h);
      assert(!node->left || (node->left->parent && node->left->parent == node));
      print_node(node->left, print_data);
      if (debug_level > 1)
        printf(",R:");
      assert(!node->right || (node->right->parent && node->right->parent == node));
      print_node(node->right, print_data);
      if (debug_level > 1) {
        printf("){");
        print_data(&node->data);
        printf("};");
      }
    }
    else if (debug_level > 1)
      printf("_");
  };
}
void printTree(TreeNode* root)
{
	if(!root)
		return;
	if(root){
		Poly p = root->data;
		printTree(root->right);
		printf("%d %g\n",p.degree,p.coeff);
		printTree(root->left);}
}

int AVL_check(const Tree* const tree, const Data search_data) {
  const TreeNode* const found_node = AVL_search(tree, search_data);
  return(found_node && compare(&found_node->data, &search_data) == 0);
}

const TreeNode *AVL_min(const Tree* const tree) {
  const TreeNode *min = tree->root;
  while (min && min->left)
    min = min->left;
  return(min);
}

const TreeNode *AVL_max(const Tree* const tree) {
  const TreeNode *max = tree->root;
  while (max && max->right)
    max = max->right;
  return(max);
}

int poly_mul(Data* node_data,void *params)
{
	Poly *p=(Poly*)params;
	node_data->degree+=p->degree;
	node_data->coeff*=p->coeff;
	return(0);
}

void TreeCopy(Tree* AID, TreeNode* denom)
{
	if(denom==NULL)
		return;
	AVL_insert(AID,denom->data);
	if(denom->right!=NULL){
		TreeCopy(AID,denom->right);}
	if(denom->left!=NULL){
		TreeCopy(AID,denom->left);}
	return;
}

void ReverseCoeff(TreeNode* root)
{
	if(root==NULL)
		return;
	root->data.coeff*=(-1);
	ReverseCoeff(root->left);
	ReverseCoeff(root->right);
}

void poly_sub2(Tree* REMAIN,TreeNode* aid,Tree* AID)
{
	TreeNode* remain=(TreeNode*)AVL_search(REMAIN,aid->data);
	if(AID->root==NULL)
		return;
	if(remain->data.degree==aid->data.degree)
	{
		if((double)((remain->data.coeff-aid->data.coeff)*(remain->data.coeff-aid->data.coeff))<=(1e-64))
		{
			if(aid->left_h>0)
				poly_sub2(REMAIN,aid->left,AID);
			if(aid->right_h>0)
				poly_sub2(REMAIN,aid->right,AID);
			AVL_delete(AID,remain->data);
			AVL_delete(REMAIN,remain->data);
		}
		else{
			Poly aidpoly={aid->data.degree,-(aid->data.coeff)};
			if(aid->left_h>0)
				poly_sub2(REMAIN,aid->left,AID);
			if(aid->right_h>0)
				poly_sub2(REMAIN,aid->right,AID);
			remain->data.coeff-=aid->data.coeff;
			AVL_delete(AID,aidpoly);
		}
	}else
	{
		Poly aidpoly={aid->data.degree,-(aid->data.coeff)};
		if(aid->left_h>0)
			poly_sub2(REMAIN,aid->left,AID);
		if(aid->right_h>0)
			poly_sub2(REMAIN,aid->right,AID);
		AVL_insert(REMAIN,aidpoly);
		AVL_delete(AID,aidpoly);
	}
	return;
}

void AVL_node_scan(TreeNode* const node, DataOpr node_opr, void *params, const ScanType scan_type) {
  int rtn = 1;
  if (!node)
    return;
  switch (scan_type) {
  case PREORDER:
    rtn = node_opr(&node->data, params);
    if (rtn) {
      AVL_node_scan(node->left, node_opr, params, scan_type);
      AVL_node_scan(node->right, node_opr, params, scan_type);
    }
    break;
  case INORDER:
    AVL_node_scan(node->left, node_opr, params, scan_type);
    rtn = node_opr(&node->data, params);
    if (rtn)
      AVL_node_scan(node->right, node_opr, params, scan_type);
    break;
  case POSTORDER:
    AVL_node_scan(node->left, node_opr, params, scan_type);
    AVL_node_scan(node->right, node_opr, params, scan_type);
    rtn = node_opr(&node->data, params);
    break;
  }
}

void AVL_scan(Tree* const tree, DataOpr node_opr, void *params, const ScanType scan_type) {
  AVL_node_scan(tree->root, node_opr, params, scan_type);
}

const TreeNode* AVL_search(const Tree* const tree, const Data search_data) {
  const TreeNode *node = tree->root;
  const TreeNode *last_node = node;
  int cmp = 0;
  while (node) {
    last_node = node;
    cmp = compare(&node->data, &search_data);
    if (cmp == 0) /* node->data == search_data */
      return(node);
    else if (cmp < 0) /* node->data < search_data */
      node = node->right;
    else node = node->left;
  };
  return(last_node);
}

const TreeNode* AVL_conseq(const TreeNode* node) {
  const TreeNode *conseq = node->right;
  while (conseq) {
    node = conseq;
    conseq = conseq->left;
  };
  return(node);
}

void AVL_insert(Tree* const tree, const Data new_data) {
  TreeNode *new_node = new_tree_node(), *last_node;
  new_node->data = new_data;
  if (tree->root) {
    int cmp = 0;
    last_node = (TreeNode*) AVL_search(tree, new_data);
    cmp = compare(&last_node->data, &new_data);
    if (cmp == 0) { /* last_node->data == new_data */
      if (debug_level > 0)
        printf("already in tree %d\n", new_data);
      return;
    };
    if (cmp < 0) { /* last_node->data < new_data */
      assert(last_node->right == NULL);
      last_node->right = new_node;
    }
    else {
      assert(last_node->left == NULL);
      last_node->left = new_node;
    };
    new_node->parent = last_node;
    if (debug_level > 0) {
      printf("after insert [%d]: ", balance(tree->root));
      print_tree(tree, print_data);
      printf("\n");
    };
    AVL_balance_update(new_node);
    while (tree->root->parent)
      tree->root = tree->root->parent;
    if (debug_level > 0) {
      printf("after balance update [%d]: ", balance(tree->root));
      print_tree(tree, print_data);
      printf("\n");
    };
    assert(fabs((float)balance(tree->root))<2);
  }
  else tree->root = new_node;
}

void AVL_delete(Tree* const tree, const Data remove_data) {
  TreeNode *last_node = (TreeNode*) AVL_search(tree, remove_data);
  if (last_node && compare(&last_node->data, &remove_data) == 0) {
    TreeNode *conseq = NULL;
    int remove_leaf = is_leaf(last_node);
    if (remove_leaf) {
      conseq = last_node;
      if (last_node->parent == NULL) {
        assert(tree->root == last_node);
        tree->root = NULL;
      };
      delete_leaf(conseq);
    }
    else if (last_node->right && last_node->left) {
      conseq = (TreeNode*) AVL_conseq(last_node);
      assert(conseq != last_node);
      assert(compare(&conseq->data, &last_node->right->data) <= 0);
      assert(compare(&conseq->data, &last_node->left->data) > 0);
      last_node->data = conseq->data;
      if (is_leaf(conseq))
        delete_leaf(conseq);
      else {
        assert(!conseq->left);
        delete_single(conseq);
      };
    }
    else {
      if (last_node->parent == NULL) {
        assert(tree->root == last_node);
        tree->root = last_node->right ? last_node->right : last_node->left;
      };
      delete_single(last_node);
    };
    conseq = NULL;
  };
  while (tree->root && tree->root->parent)
    tree->root = tree->root->parent;
  if (debug_level > 0) {
    printf("after delete [%d]: ", balance(tree->root));
    print_tree(tree, print_data);
    printf("\n");
  };
  assert(fabs((float)balance(tree->root))<2);
}


void AVL_balance_update(TreeNode* const node) {
  TreeNode *parent = node ? node->parent : NULL;
  int done = 0, parent_h = node ? height(node)+1 : 0;
  int prev_parent_h = 0, bl_factor = 0;
  if (!parent)
    return;
  prev_parent_h = height(parent);
  if (parent->left == node) {
    if (parent->left_h != parent_h) {
      parent->left_h = parent_h;
      done = (height(parent) == prev_parent_h);
    };
  }
  else if (parent->right_h != parent_h) {
    assert(parent->right == node);
    parent->right_h = parent_h;
    done = (height(parent) == prev_parent_h);
  };
  bl_factor = balance(parent);
  if (bl_factor*bl_factor == 4) {
    parent = AVL_rebalance(parent);
    done = (height(parent) == prev_parent_h);
    /*done = !balance(parent);*/
  };
  if (!done)
    AVL_balance_update(parent);
}

TreeNode *AVL_rebalance(TreeNode* const node) {
  TreeNode *x=node, *y=node, *z=node; /* min, mid, max */
  if (node->left_h > node->right_h) {
    x = NULL;
    y = node->left;
    if (y->left_h < y->right_h) {
      x = y;
      y = x->right;
      AVL_rotate(&x,&y);
    }; /* else x = y->left; */
    AVL_rotate(&y,&z);
  }
  else {
    z = NULL;
    y = node->right;
    if (y->left_h > y->right_h) {
      z = y;
      y = z->left;
      AVL_rotate(&z,&y);
    }; /* else z = y->right; */
    AVL_rotate(&y,&x);
  };
  assert(!x || fabs((float)balance(x))<2);
  assert(!y || fabs((float)balance(y))<2);
  assert(!z || fabs((float)balance(z))<2);
  assert((!x || compare(&x->data, &y->data) < 0) && (!z || compare(&y->data, &z->data) < 0));
  return(y);
}

void AVL_rotate(TreeNode** const n1, TreeNode** const n2) {
  TreeNode *x = *n1, *y = *n2;
  if (debug_level > 0) {
    printf("before rotate:");
    print_node(x, print_data);
    printf(" & ");
    print_node(y, print_data);
    printf("\n");
  };
  if (x->parent == y) {
    AVL_rotate(n2, n1);
    return;
  };
  assert(y->parent == x);
  if (y == x->left) {
    x->left = y->right;
    y->right = x;
    if (x->left)
      x->left->parent = x;
  }
  else {
    assert(y == x->right);
    x->right = y->left;
    y->left = x;
    if (x->right)
      x->right->parent = x;
  };
  y->parent = x->parent;
  if (x->parent) {
    if (x->parent->left == x)
      x->parent->left = y;
    else x->parent->right = y;
  };
  x->parent = y;

  if (y->right == x) {
    x->left_h = height(x->left)+1;
    y->right_h = height(x)+1;
  }
  else {
    assert(y->left == x);
    x->right_h = height(x->right)+1;
    y->left_h = height(x)+1;
  };
  if (debug_level > 0) {
    printf("after rotate:");
    print_node(x, print_data);
    printf(" & ");
    print_node(y, print_data);
    printf("\n");
  };
}

void *read_poly_num(Tree* tree) {
 int pow, cur_len=0, elem_len=0, max_len=1024;
 Poly p;
 char poly_elem[32], var;
 double coeff;
 int n = scanf("%c", &var);
 while (n>0 && (var < 'A' || var > 'z'))
 n = scanf("%c", &var);
 while (n>0 && (n=scanf("%d %lf", &pow, &coeff)>0)) {
 sprintf(poly_elem, "%s%.2lf*%c^%d",
 ( coeff<0 || cur_len==0) ? "" : "+", coeff, var, pow);
 p.coeff=coeff;
 p.degree=pow;
 AVL_insert(tree,p);
 elem_len = strlen(poly_elem);
 if (elem_len+cur_len>max_len) {
 max_len *= 2;
	};
 cur_len += elem_len;
 };
 return;
}

void *read_poly_denom(Tree* tree) {
 int pow, cur_len=0, elem_len=0, max_len=1024;
 Poly p;
 char poly_elem[32], var;
 double coeff;
 int n = scanf("%c", &var);
 while (n>0 && (var < 'A' || var > 'z'))
 n = scanf("%c", &var);
 while (n>0 && (n=scanf("%d %lf", &pow, &coeff)>0)) {
 sprintf(poly_elem, "%s%.2lf*%c^%d",
 ( coeff<0 || cur_len==0) ? "" : "+", coeff, var, pow);
 p.coeff=coeff;
 p.degree=pow;
 AVL_insert(tree,p);
 elem_len = strlen(poly_elem);
 if (elem_len+cur_len>max_len) {
 max_len *= 2;
	};
 cur_len += elem_len;
 };
 return;
}
#ifdef RUN_TIME_TEST_APP

int main(int argc, char **argv) {
  Tree numer={0}, denom={0}, remain={0}, quot={0}, aid={0};
  Poly max_denom;
  int i=0, j=0;
  read_poly_num(&numer);
  read_poly_denom(&denom);
  if (argc > 1)
    debug_level = atoi(argv[1]);
  srand(time(NULL));
  max_denom = AVL_max(&denom)->data; //highest degree of the denominator
  remain=numer;
  while(remain.root!=NULL && (AVL_max(&remain)->data.degree>=max_denom.degree))
  {
	  Poly p = {(AVL_max(&remain)->data.degree-max_denom.degree), (AVL_max(&remain)->data.coeff/max_denom.coeff)};
	  AVL_insert(&quot,p);
	  assert(AVL_check(&quot, p));
	  TreeCopy(&aid,denom.root);
	  AVL_scan(&aid,poly_mul,&p,POSTORDER);
	  while(aid.root!=NULL)
		 poly_sub2(&remain,aid.root,&aid);
	  if(aid.root!=NULL)
	  {
		ReverseCoeff(aid.root);
		TreeCopy(&remain,aid.root);
		delete_aid(&aid,aid.root);
		aid.root=NULL;
	  }
  }
  printf("x\n");printTree(quot.root);
  printf("x\n");printTree(remain.root);
  printf("\n");
  return(0);
}
#endif