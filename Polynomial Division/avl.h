typedef struct {
  int degree;
  double coeff;
} Poly;

void print_poly(const Poly *p);
int compare_poly(const Poly *p1, const Poly *p2);

/*typedef int Data;*/
typedef Poly Data;

typedef void (*PrintData)(const Data *p);
typedef int (*ComparePoly)(const Data *p1, const Data *p2);

PrintData print_data=print_poly;
ComparePoly compare=compare_poly;

/*void print_data(const Data *d);*/
/*int compare(const Data* d1, const Data *d2);*/

typedef struct TreeNode TreeNode;

typedef int (*DataOpr)(Data* const node_data, void *params);

typedef struct {
  TreeNode *root;
} Tree;

struct TreeNode {
  TreeNode* left;
  TreeNode* right;
  TreeNode* parent;
  Data data; /* or key */
  int left_h;
  int right_h;
};

typedef enum {PREORDER, INORDER, POSTORDER} ScanType;

void print_tree(const Tree* const tree, PrintData print_data);
void print_node(const TreeNode* const node, PrintData print_data);
void printTree(TreeNode*);
void AVL_insert(Tree* const tree, const Data new_data);
void AVL_delete(Tree* const tree, const Data remove_data);
void poly_sub2(Tree* REMAIN,TreeNode* aid,Tree* AID);
const TreeNode* AVL_search(const Tree* const tree, const Data search_data);
TreeNode* AVL_search1(const Tree* const tree, const Data search_data);
TreeNode* AVL_search2(const Tree* const tree, const Data search_data);
const TreeNode *AVL_min(const Tree* const tree);
const TreeNode *AVL_max(const Tree* const tree);
void AVL_scan(Tree* const tree, DataOpr node_opr, void *params, const ScanType scan_type);
int AVL_check(const Tree* const tree, const Data search_data);
int poly_mul(Data* node_data,void *params);

TreeNode* new_tree_node();
int height(const TreeNode* const node);
int balance(const TreeNode* const node);

void delete_leaf(TreeNode* const node);
void delete_single(TreeNode* const node);
void AVL_rotate(TreeNode** const n1, TreeNode** const n2);
void AVL_balance_update(TreeNode* const node);
const TreeNode *AVL_conseq(const TreeNode *node);
TreeNode *AVL_rebalance(TreeNode* const node);
void *read_poly_num(Tree*);
void *read_poly_denom(Tree*);
