#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like 

// data type for avl tree nodes
typedef struct AVLTreeNode {
	int key; //key of this item
	int  value;  //value (int) of this item 
	int height; //height of the subtree rooted at this node
	struct AVLTreeNode *parent; //pointer to parent
	struct AVLTreeNode *left; //pointer to left child
	struct AVLTreeNode *right; //pointer to right child
} AVLTreeNode;

//data type for AVL trees
typedef struct AVLTree{
	int  size;      // count of items in avl tree
	AVLTreeNode *root; // root
} AVLTree;

typedef struct Tuple{
	int  key;
	int value;
} Tuple;
// create a new AVLTreeNode
AVLTreeNode *newAVLTreeNode(int k, int v )
{
	AVLTreeNode *new;
	new = malloc(sizeof(AVLTreeNode));
	assert(new != NULL);
	new->key = k;
	new->value = v;
	new->height = 0; // height of this new node is set to 0
	new->left = NULL; // this node has no child
	new->right = NULL;
	new->parent = NULL; // no parent
	return new;
}

// create a new empty avl tree
AVLTree *newAVLTree()
{
	AVLTree *T;
	T = malloc(sizeof (AVLTree));
	assert (T != NULL);
	T->size = 0;
	T->root = NULL;
	return T;
}

//int isAVLTree();
//void AddToLeft();
//void AddToRight();
void AdjustHeight(AVLTreeNode *node);
void Balance();
AVLTreeNode *Successor(AVLTreeNode *node);
void Predecessor();
AVLTreeNode *Search(AVLTree *T, int k, int v);
void PrintAVLTree(AVLTree *T);
void inOrderPrint(AVLTreeNode *node);
void setHeight(AVLTreeNode *node);
int maxValue(int a, int b);

int InsertNode(AVLTree *T, int k, int v);
// put your time complexity analysis of CreateAVLTree() here
// init insert
// compare key and then value if necessary
AVLTree *CreateAVLTree(const char *filename)
{
 // put your code here
	AVLTree *res = newAVLTree();
	if(strcmp(filename, "stdin") == 0)
	{
		char c;
		int flag1 = 1;
		int flag2 = 1;
		//char buff[50];
		int num1, num2;
		while(1)
		{	
			fread(&c, sizeof(c), 1, stdin);
			if(c == '(')
			{
				fscanf(stdin, " %d , %d ", &num1, &num2);
				while(1)
				{
					fread(&c, sizeof(c), 1, stdin);
					if(c != ')')
					{
						printf("Input Error when reading file\n");
						exit(0);
					}
					else
					{
						break;
					}
				}
				flag2 = flag1;
				flag1 = -1 * flag1;
				InsertNode(res, num1, num2);
			}
			else if(c == '\n')
			{
				if(flag1 == flag2)
				{
					break;
				}
				else
				{
					flag1 = 1;
					flag2 = 1;
				}
			}
			else if(c == ' ')
			{
				continue;
			}
			else
			{
				printf("Input Error when reading file\n");
				exit(0);
			}
		}
	}	
	else
	{
		FILE *fp;
		int num1, num2;
		char c;
		fp = fopen(filename, "r");
		if(fp == NULL)
		{
			printf("Cannot open file or file does not exist.\n");
			exit(0);
		}
		while(1)
		{
			int q = fscanf(fp, " ( %d , %d ) ", &num1, &num2);
			if(q == EOF)
			{
				break;
			}
			else if(q != 2)
			{
				printf("Cannot open file or file does not exist.\n");
				exit(0);
			}
			InsertNode(res, num1, num2);
		}
	}
	return res;
  
}

AVLTreeNode *Clone(AVLTreeNode *node)
{
	if(node == NULL)
	{
		return NULL;
	}
	AVLTreeNode *newNode = newAVLTreeNode(node->key, node->value);
	newNode->height = node->height;
	newNode->left = Clone(node->left);
	if(newNode->left != NULL)
	{
		newNode->left->parent = newNode;
	}
	newNode->right = Clone(node->right);
	if(newNode->right != NULL)
	{
		newNode->right->parent = newNode;
	}
	return newNode;
}

// put your time complexity analysis for CloneAVLTree() here
// recursion
AVLTree *CloneAVLTree(AVLTree *T)
{
 // put your code here
	AVLTree *newTree = newAVLTree();
	newTree->size = T->size;
	newTree->root = Clone(T->root);
	return newTree;

	//newTree->root = ;
}
 
void BuildTupleArray(AVLTreeNode *node, Tuple *tuple, int *index)
{
	if(node == NULL)
	{
		return;
	}
	BuildTupleArray(node->left, tuple, index);
	tuple[*index].key = node->key;
	tuple[*index].value = node->value;
	(*index)++;
	BuildTupleArray(node->right, tuple, index);
}

int tupleCompare(Tuple tuple1, Tuple tuple2)
{
	if(tuple1.key < tuple2.key)
	{
		return -1;
	}
	else if(tuple1.key > tuple2.key)
	{
		return 1;
	}
	else
	{
		if(tuple1.value < tuple2.value)
		{
			return -1;
		}
		else if(tuple1.value > tuple2.value)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}
int UnionArray(Tuple *tuple1, Tuple *tuple2, Tuple *tuple3, int l1, int l2)
{
	int i = 0;
	int j = 0;
	int k = 0;
	while(j < l1 || k < l2)
	{
		if(j == l1)
		{
			for(int tk = k; tk < l2; tk++)
			{
				tuple3[i].key = tuple2[tk].key;
				tuple3[i].value = tuple2[tk].value;		
				i++;		
			}
			break;
		}
		if(k == l2)
		{
			for(int tj = j; tj < l1; tj++)
			{
				tuple3[i].key = tuple1[tj].key;
				tuple3[i].value = tuple1[tj].value;		
				i++;		
			}
			break;			
		}
		if(tupleCompare(tuple1[j], tuple2[k]) < 0)
		{
			tuple3[i].key = tuple1[j].key;
			tuple3[i].value = tuple1[j].value;
			i++;
			j++;
		}
		else if(tupleCompare(tuple1[j], tuple2[k]) > 0)
		{
			tuple3[i].key = tuple2[k].key;
			tuple3[i].value = tuple2[k].value;
			i++;
			k++;
		}
		else
		{
			tuple3[i].key = tuple1[j].key;
			tuple3[i].value = tuple1[j].value;
			i++;
			j++;
			k++;
		}

	}
	return i;
}

AVLTreeNode *BuildTreeFromArray(Tuple *tuple, int start, int last)
{
	if(last < start)
	{
		return NULL;
	}
	int mid = (start + last) / 2;
	int leftHeight = -1; 
	int rightHeight = -1;
	AVLTreeNode *node = newAVLTreeNode(tuple[mid].key, tuple[mid].value);
	node->left = BuildTreeFromArray(tuple, start, mid - 1);
	if(node->left != NULL)
	{
		leftHeight = node->left->height;
		node->left->parent = node;
	}
	node->right = BuildTreeFromArray(tuple, mid + 1, last);
	if(node->right != NULL)
	{
		rightHeight = node->right->height;
		node->right->parent = node;
	}
	node->height = maxValue(leftHeight, rightHeight) + 1;
	return node; 
}
// put your time complexity for ALVTreesUNion() here
AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2)
{
	//put your code here
	Tuple tuple1[T1->size], tuple2[T2->size], tuple3[T2->size + T1->size];
	int index = 0;
	int l;
	int leftHeight = 0;
	int rightHeight = 0;
	AVLTree *res = newAVLTree();
	BuildTupleArray(T1->root, tuple1, &index);
	index = 0;
	BuildTupleArray(T2->root, tuple2, &index);
	l = UnionArray(tuple1, tuple2, tuple3, T1->size, T2->size);
	res->size = l;
	res->root = BuildTreeFromArray(tuple3, 0, l - 1);
	return res;
}

int IntersectArray(Tuple *tuple1, Tuple *tuple2, Tuple *tuple3, int l1, int l2)
{
	int i = 0;
	int j = 0;
	int k = 0;
	while(j < l1 || k < l2)
	{
		if(j == l1)
		{
			break;
		}
		if(k == l2)
		{
			break;			
		}
		if(tupleCompare(tuple1[j], tuple2[k]) < 0)
		{
			j++;
		}
		else if(tupleCompare(tuple1[j], tuple2[k]) > 0)
		{
			k++;
		}
		else
		{
			tuple3[i].key = tuple1[j].key;
			tuple3[i].value = tuple1[j].value;
			i++;
			j++;
			k++;
		}

	}
	return i;
}
// put your time complexity for ALVTreesIntersection() here
AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2)
{
	 //put your code here
	Tuple tuple1[T1->size], tuple2[T2->size], tuple3[T1->size];
	int index = 0;
	int l;
	int leftHeight = 0;
	int rightHeight = 0;
	AVLTree *res = newAVLTree();
	BuildTupleArray(T1->root, tuple1, &index);
	index = 0;
	BuildTupleArray(T2->root, tuple2, &index);
	l = IntersectArray(tuple1, tuple2, tuple3, T1->size, T2->size);
	res->size = l;
	res->root = BuildTreeFromArray(tuple3, 0, l - 1);
	return res;
}

void AddToLeft(AVLTree *T, AVLTreeNode *node, AVLTreeNode *child)
{
	node->left = child;
	child->parent = node;
	(T->size++);
	AdjustHeight(child);
}

void AddToRight(AVLTree *T, AVLTreeNode *node, AVLTreeNode *child)
{
	node->right = child;
	child->parent = node;
	(T->size)++;
	AdjustHeight(child);
}

int maxValue(int a, int b)
{
	if(a >= b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

// recompute height from the given node to the root
void AdjustHeight(AVLTreeNode *n)
{
	AVLTreeNode *node = n;
	while(node != NULL)
	{
		setHeight(node);
		node = node->parent;
	}
}

AVLTreeNode *InsertAsBST(AVLTree *T, int k, int v)
{
	AVLTreeNode *node = T->root;
	if(node == NULL)
	{
		AVLTreeNode * newNode = newAVLTreeNode(k, v);
		T->root = newNode;
		(T->size)++;
		return newNode;
	}
	AVLTreeNode *parentNode;
	while(node != NULL)
	{
		parentNode = node;
		if(k < node->key)
		{
			node = node->left;
		}
		else if(k > node->key)
		{
			node = node->right;
		}
		else{
			if(v < node->value)
			{
				node = node->left;

			}
			else if(v > node->value)
			{
				node = node->right;
			}
			else
			{
				return NULL;
			}

		}
	}

	AVLTreeNode *newNode = newAVLTreeNode(k, v);
	if(k < parentNode->key)
	{
		AddToLeft(T, parentNode, newNode);
	}
	else if(k > parentNode->key)
	{
		AddToRight(T, parentNode, newNode);
	}
	else
	{
		if(v < parentNode->value)
		{
			AddToLeft(T, parentNode, newNode);
		}
		else if(v > parentNode->value)
		{
			AddToRight(T, parentNode, newNode);
		}
		else
		{
			return NULL;
		}

	}
	return newNode;
}

// return the root of the subtree of the given node with larger height
AVLTreeNode *LargerHeightSubtreeRoot(AVLTreeNode *n)
{
	AVLTreeNode *node = n;
	if(node->left == NULL && node->right != NULL)
	{
		return node->right;
	}
	else if(node->left != NULL && node->right == NULL)
	{
		return node->left;
	}
	else if(node->left == NULL && node->right == NULL)
	{
		return NULL;
	}
	else
	{
		if(node->left->height > node->right->height)
		{
			return node->left;
		}
		else if(node->left->height < node->right->height)
		{
			return node->right;
		}
		else
		{
			return node->right;
		}
	}
}

void setA(AVLTreeNode *a, AVLTreeNode *b, AVLTreeNode *c, AVLTreeNode *t1, AVLTreeNode *t2, AVLTreeNode *t3, AVLTreeNode *t4)
{
	a->parent = b;
	a->left = t1;
	a->right = t2;
	if(t1 != NULL)
	{
		t1->parent = a;
	}
	if(t2 != NULL)
	{
		t2->parent = a;
	}

}
void setB(AVLTreeNode *a, AVLTreeNode *b, AVLTreeNode *c, AVLTreeNode *t1, AVLTreeNode *t2, AVLTreeNode *t3, AVLTreeNode *t4)
{
	b->left = a;
	b->right = c;
}
void setC(AVLTreeNode *a, AVLTreeNode *b, AVLTreeNode *c, AVLTreeNode *t1, AVLTreeNode *t2, AVLTreeNode *t3, AVLTreeNode *t4)
{
	c->parent = b;
	c->left = t3;
	c->right = t4;
	if(t3 != NULL)
	{
		t3->parent = c;
	}
	if(t4 != NULL)
	{
		t4->parent = c;
	}
}
// compute height of the given node
void setHeight(AVLTreeNode *node)
{
	if(node->left == NULL && node->right != NULL)
	{
		node->height = (node->right->height) + 1;
	}
	else if(node->left != NULL && node->right == NULL)
	{
		node->height = (node->left->height) + 1;
	}
	else if(node->left == NULL && node->right == NULL)
	{
		node->height = 0;
	}
	else
	{
		node->height = maxValue(node->left->height, node->right->height) + 1;
	}
}

void Restructuring(AVLTree *T, AVLTreeNode *n)
{
	AVLTreeNode *x = n;
	AVLTreeNode *y, *z;
	AVLTreeNode *a, *b, *c;
	AVLTreeNode *t1, *t2, *t3, *t4;
	y = LargerHeightSubtreeRoot(x);
	z = LargerHeightSubtreeRoot(y);
	if(x->left == y)
	{
		if(y->left == z)
		{
			a = z;
			b = y;
			c = x;
			t1 = a->left;
			t2 = a->right;
			t3 = b->right;
			t4 = c->right;
		}
		else
		{
			a = y;
			b = z;
			c = x;
			t1 = a->left;
			t2 = b->left;
			t3 = b->right;
			t4 = c->right;
		}
	}
	else
	{
		if(y->left == z)
		{
			a = x;
			b = z;
			c = y;
			t1 = a->left;
			t2 = b->left;
			t3 = b->right;
			t4 = c->right;
		}
		else
		{
			a = x;
			b = y;
			c = z;
			t1 = a->left;
			t2 = b->left;
			t3 = c->left;
			t4 = c->right;
		}

	}
	AVLTreeNode *parent = n->parent;
	if(n == T->root)
	{
		b->parent = NULL;
		T->root = b;
	}
	else
	{
		b->parent = parent;
		if(parent->left == n)
		{
			parent->left = b;
		}
		else
		{
			parent->right = b;
		}
	}
	setA(a, b, c, t1, t2, t3, t4);
	setB(a, b, c, t1, t2, t3, t4);
	setC(a, b, c, t1, t2, t3, t4);
	setHeight(a);
	setHeight(c);
	setHeight(b);
	AdjustHeight(b);
}

// find the first unbalanced node from the given node to the root
AVLTreeNode *FindUnbalancedNode(AVLTreeNode *n)
{
	AVLTreeNode *node = n;
	//find the first unbalanced node
	while(node != NULL)
	{	
		int heightDifference;
		if(node->left == NULL && node->right != NULL)
		{
			heightDifference = node->right->height + 1;
		}
		else if(node->left != NULL && node->right == NULL)
		{
			heightDifference = node->left->height + 1;
		}
		else if(node->left == NULL && node->right == NULL)
		{
			heightDifference = 0;
		}
		else
		{
			heightDifference = abs(node->left->height - node->right->height);
		}
		if(heightDifference > 1)
		{
			return node;
		}
		node = node->parent;
	}
	return NULL;
}

// put the time complexity analysis for InsertNode() here    
// BST_insert height balance
int InsertNode(AVLTree *T, int k, int v)
{
	if(T == NULL)
	{
		return 0;
	}
	// insert first
	AVLTreeNode *newNode = InsertAsBST(T, k, v);
	if(newNode != NULL)
	{
		AVLTreeNode *unbalancedNode = FindUnbalancedNode(newNode);
		//when inserting, there needs at most 1 adjustment
		if(unbalancedNode != NULL)
		{
			Restructuring(T, unbalancedNode);
		}
		return 1;
	}
	else
	{
		return 0;
	}
	//put your code here
}

// return the immediately inorder successor of the given node
AVLTreeNode *Successor(AVLTreeNode *node)
{
	AVLTreeNode *n = node->right;
	AVLTreeNode *res;
	while(n != 	NULL)
	{
		res = n;
		n = n->left;
	}
	return res;
}
AVLTreeNode *BSTDelete(AVLTree *T, AVLTreeNode *n)
{
	if(n == NULL)
	{
		return NULL;
	}
	if(n->left == NULL && n->right == NULL)
	{
		AVLTreeNode *parentNode = n->parent;
		if(n->parent == NULL)
		{
			T->root = NULL;
		}
		else
		{
			if(n->parent->left == n)
			{
				n->parent->left = NULL;
			}
			else if(n->parent->right == n)
			{
				n->parent->right = NULL;
			}			
		}
		AdjustHeight(parentNode);
		free(n);
		(T->size)--;
		return parentNode;
	}
	else if(n->left != NULL && n->right == NULL)
	{
		AVLTreeNode *parentNode = n->parent;
		if(n->parent == NULL)
		{
			T->root = n->left;
			n->left->parent = NULL;		
		}
		else
		{
			if(n->parent->left == n)
			{
				n->parent->left = n->left;
				n->left->parent = n->parent;
			}
			else if(n->parent->right == n)
			{
				n->parent->right = n->left;
				n->left->parent = n->parent;
			}
		}
		AdjustHeight(parentNode);
		free(n);
		(T->size)--;
		return parentNode;
	}
	else if(n->left == NULL && n->right != NULL)
	{
		AVLTreeNode *parentNode = n->parent;
		if(n->parent == NULL)
		{
			T->root = n->right;
			n->right->parent = NULL;			
		}
		else
		{
			if(n->parent->left == n)
			{
				n->parent->left = n->right;
				n->right->parent = n->parent;
			}
			else if(n->parent->right == n)
			{
				n->parent->right = n->right;
				n->right->parent = n->parent;
			}			
		}
		AdjustHeight(parentNode);
		free(n);
		(T->size)--;
		return parentNode;
	}
	else
	{
		AVLTreeNode *succ = Successor(n);
		n->key = succ->key;
		n->value = succ->value;
		return (BSTDelete(T, succ));
	}
}

// put your time complexity for DeleteNode() here
int DeleteNode(AVLTree *T, int k, int v)
{
 // put your code here
	if(T == NULL)
	{
		return 0;
	}
	AVLTreeNode *node = Search(T, k, v);
	if(node == NULL)
	{
		return 0;
	}
	else
	{
		AVLTreeNode *deletedNode = BSTDelete(T, node);
		while(1)
		{
			AVLTreeNode *unbalancedNode = FindUnbalancedNode(deletedNode);
			if(unbalancedNode != NULL)
			{
				Restructuring(T, unbalancedNode);
			}
			else
			{
				break;
			}
		}
		return 1;
	}
}

// O(log n)
AVLTreeNode *SearchHelper(AVLTreeNode *node, int k, int v)
{
	if(node == NULL)
	{
		return NULL;
	}
	else
	{
		if(node->key == k && node->value == v)
		{
			return node;
		}
		if(k < node->key)
		{
			return SearchHelper(node->left, k, v);
		}
		else if(k > node->key)
		{
			return SearchHelper(node->right, k, v);
		}
		else
		{
			if(v < node->value)
			{
				return SearchHelper(node->left, k, v);
			}
			else if(v > node->value)
			{
				return SearchHelper(node->right, k, v);
			}
			else
			{
				return NULL;
			}
		}
	}
}
// put your time complexity analysis for Search() here
AVLTreeNode *Search(AVLTree *T, int k, int v)
{
  // put your code here
	if(T == NULL)
	{
		return NULL;
	}
	return SearchHelper(T->root, k, v);
}

// O(n)
void postOrderFree(AVLTreeNode *node)
{
	if(node == NULL)
	{
		return;
	}
	postOrderFree(node->left);
	postOrderFree(node->right);
	free(node);
}

// put your time complexity analysis for freeAVLTree() here
void FreeAVLTree(AVLTree *T)
{
// put your code here
	if(T == NULL)
	{
		return;
	}	
	postOrderFree(T->root);
	free(T);
}

// O(n)
void inOrderPrint(AVLTreeNode *node)
{
	if(node == NULL)
	{
		return;
	}
	inOrderPrint(node->left);
	printf("(%d, %d), %d\n", node->key, node->value, node->height);
	inOrderPrint(node->right);
}

// put your time complexity analysis for PrintAVLTree() here
void PrintAVLTree(AVLTree *T)
{
 // put your code here
	if(T == NULL)
	{

		return;
	}
	inOrderPrint(T->root);
}

int main() //sample main for testing 
{ 


	int i,j;
 AVLTree *tree1, *tree2, *tree3, *tree4, *tree5, *tree6, *tree7, *tree8;
 AVLTreeNode *node1;
 
 tree1=CreateAVLTree("stdin");
 printf("\ntree1:\n");
  //PrintAVLTree(tree1);
 FreeAVLTree(tree1);

 //you need to create the text file file1.txt
 // to store a set of items without duplicate items
 tree2=CreateAVLTree("file1.txt"); 
  printf("\ntree2:\n");
 PrintAVLTree(tree2);
return 0;
 //return 0;
 tree3=CloneAVLTree(tree2);
  printf("\ntree3:\n");
 //PrintAVLTree(tree3);
 FreeAVLTree(tree2);
 FreeAVLTree(tree3);
 //Create tree4 
 tree4=newAVLTree();
 j=InsertNode(tree4, 10, 10);
 for (i=0; i<15; i++)
  {
   j=InsertNode(tree4, i, i);
   if (j==0) printf("(%d, %d) already exists\n", i, i);
  }
    printf("\ntree4:\n");
  PrintAVLTree(tree4);
  node1=Search(tree4,20,20);
  if (node1!=NULL)
    printf("key= %d value= %d\n",node1->key,node1->value);
  else 
    printf("Key 20 does not exist\n");
  
  for (i=17; i>0; i--)
  {
    j=DeleteNode(tree4, i, i);
	if (j==0) 
	  printf("Key %d does not exist\n",i);  
    printf("after delete %d\n", i);
    PrintAVLTree(tree4);
  }
    printf("\ntree4:\n");
 FreeAVLTree(tree4);
 //Create tree5
 
 tree5=newAVLTree();
 j=InsertNode(tree5, 6, 25);
 j=InsertNode(tree5, 6, 10);
 j=InsertNode(tree5, 6, 12);
 j=InsertNode(tree5, 6, 20);
 j=InsertNode(tree5, 9, 25);
 j=InsertNode(tree5, 10, 25);
   printf("\ntree5:\n");
 PrintAVLTree(tree5);

 //Create tree6
 tree6=newAVLTree();
 j=InsertNode(tree6, 6, 25);
 j=InsertNode(tree6, 5, 10);
 j=InsertNode(tree6, 6, 12);
 j=InsertNode(tree6, 6, 20);
 j=InsertNode(tree6, 8, 35);
 j=InsertNode(tree6, 10, 25);
   printf("\ntree6:\n");
 PrintAVLTree(tree6);
 tree7=AVLTreesIntersection(tree5, tree6);
 tree8=AVLTreesUnion(tree5,tree6);
   printf("\ntree7:\n");
 PrintAVLTree(tree7);
   printf("\ntree8:\n");
 PrintAVLTree(tree8);
 return 0; 
}
