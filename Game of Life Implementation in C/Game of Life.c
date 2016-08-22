#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define dead ' '
#define alive '*'

/*Function Declarations*/
void Operator();
void user_Input();
void rand_Input();
void free_Arrays();
void check();
int alive_counter();
/*---------------------*/

void main()
{
	int choice = 1;

	while(choice!=0){
		Operator();

		do{printf("Play again - enter 1, exit - enter 0: \n");
		scanf("%d", &choice);}while((choice<0) || (choice>1));
	}

	system("pause");
}

/*Function Definitions */
void Operator()
{
	int rows, cols, **matrix, i, inp_Choice, **temp, k, l;
	printf("Welcome to the game of life! \nplease enter the number of rows your matrix will contain: \n");
	scanf("%d", &rows);
	rows++;
	printf("Please enter the number of columns your matrix will contain: \n");
	scanf("%d", &cols);
	cols++;

	/*Assigning the chosen row size to the matrix*/
	matrix = (int**)malloc((rows+1)*(cols+1)*sizeof(int*));
	temp = (int**)malloc((rows+1)*(cols+1)*sizeof(int*));			//Note: we declare a temporary matrix so that the calculations will be casted there instead of the original matrix. this way we keep the calculations pure.

	/*Assigning the chosen column size to each row*/
	for(i=0;i<=rows;i++)
	{matrix[i]= (int*)malloc((cols+1)*sizeof(int));}
	for(i=0;i<=rows;i++)
	{temp[i]= (int*)malloc((cols+1)*sizeof(int));}
	
	
	/*The user chooses whether he provids input or distribute by random*/
	printf("Would you like to enter the values yourself or go by random values?\n1-yourself\t0-random\n");
	scanf("%d", &inp_Choice);
	if(inp_Choice==1)
	{
		user_Input(matrix, rows, cols, temp);
	}else{rand_Input(matrix, rows, cols, temp);}
	
	check(matrix,rows,cols,temp);

	free_Arrays(matrix, rows, cols, temp);
}
void user_Input(int **matrix, int rows, int cols, int **temp)
{
	int i, j;
	char var;
	printf("please enter your matrix values for each element \n('*' for 'alive', any other value for 'dead')\n");

	for(i=1; i<rows;i++)
	{
		for(j=1;j<cols;j++)
		{
			printf("matrix[%d][%d]: \n", i, j);
			scanf(" %c", &var);
			if(var!= alive)
			{var = dead;}
			matrix[i][j] = var;
			temp[i][j] = matrix[i][j];
		}
	}
	printf("This is your matrix: \n");

	for(i=1; i<rows;i++)
	{
		for(j=1;j<cols;j++)
		{
			printf("%c ",matrix[i][j]);
		}
		printf("\n");
	}
	system("pause");
	system("cls");
}

void rand_Input(int **matrix, int rows, int cols, int **temp)
{
	int i, j, var;
	srand(time(NULL));
	for(i=1;i<rows;i++)
	{
		for(j=1;j<cols;j++)
		{
			var = (rand())%2;
			if(var==1)
			{
				matrix[i][j]='*';
			}else{matrix[i][j]=' ';}
		}
	}
	printf("This is your matrix: \n");
	for(i=1; i<rows;i++)
	{
		for(j=1;j<cols;j++)
		{
			printf("%c ",matrix[i][j]);
		}
		printf("\n");
	}
	system("pause");
	system("cls");
}

void check(int **matrix, int rows, int cols, int **temp)
{
	int i, j, Change_Counter=0, Same_Counter=0;
	char ask_User=0;
	/*Same_Counter counts if the previous matrix repeats itself. and if it does - stops*/
	/*This loop checks the current matrix[row][column] condition. if it is alive and changes to dead - increments Change_Counter by 1, same for the opposite*/
	while((Same_Counter<1) && (ask_User!='1')){
	for(i=1;i<rows;i++)
	{
		for(j=1;j<cols;j++)
		{
			if((matrix[i][j]==alive) && (((alive_counter(matrix,i,j,rows,cols))==3)||((alive_counter(matrix,i,j,rows,cols))==2))){temp[i][j]=alive;}else{temp[i][j]=dead;}
			if((matrix[i][j]==dead) && ((alive_counter(matrix,i,j,rows,cols))==3)){temp[i][j]=alive;Change_Counter=Change_Counter+1;}
		}
	}
	for(i=1;i<rows;i++)
	{
		for(j=1;j<cols;j++)
		{
			matrix[i][j] = temp[i][j];
		}
	}
	if(Change_Counter==0){Same_Counter++;}else{Same_Counter=0;}
	Change_Counter=0;
	for(i=1; i<rows;i++)
	{
		for(j=1;j<cols;j++)
		{
			printf("%c ",matrix[i][j]);
		}
		printf("\n");
	}
	printf("Would you like to stop? \n 1 - yes \t 0 - no\n");
	scanf(" %c", &ask_User);
	if(Same_Counter==0){system("cls");}
	}
}

int alive_counter(int **matrix, int row, int col, int rows, int cols)	//Function to count the neighbours in the Alive state
{
	int alive_counter=0, k, l;

	/*Checking defined rows ONLY*/
	for(k=-1;((k<=1) && (row+k<=rows));k++)
	{
		
		for(l=-1;((l<=1) && (l+col<=cols));l++)
		{
			if((col+l<0)){l++;}											//If previous column is undefined, check next column
			if((l==0)&&(k==0)){l++;}												//Skip current position
			if(matrix[(row+k)%rows][col+l]==alive){alive_counter++;}
		}
	}
	return alive_counter;
}

/*Function to free arrays*/
void free_Arrays(int **matrix, int rows,int cols, int **temp)
{
	int i,j;
	for(i=0;i<rows;i++)
	{
		free(matrix[i]);
		free(temp[i]);
	}
	free(matrix);
	free(temp);
}
/*---------------------------------*/



