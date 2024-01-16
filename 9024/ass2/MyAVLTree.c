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

//data type for tuple: (key, value)
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

//create avl tree from a file or stdin and return the avl tree
AVLTree *CreateAVLTree(const char *filename);
//recursion, clone avl tree from the root and return the root
AVLTreeNode *Clone(AVLTreeNode *node);
//clone avl tree and return cloned avl tree
AVLTree *CloneAVLTree(AVLTree *T);
//build a tuple array which saves all elements in avl tree in ascending order
void BuildTupleArray(AVLTreeNode *node, Tuple *tuple, int *index);
//compare two tuple(key, value), and return the result
int tupleCompare(Tuple tuple1, Tuple tuple2);
//use tuple1 and tuple2 to build an union array tuple3, and return the length of tuple3
int UnionArray(Tuple *tuple1, Tuple *tuple2, Tuple *tuple3, int l1, int l2);
//use tuple array to build an avl tress.
AVLTreeNode *BuildTreeFromArray(Tuple *tuple, int start, int last);
//return the union avl tree of T1 and T2
AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2);
//use tuple1 and tuple2 to build an intersection array tuple3, and return the length of tuple3
int IntersectArray(Tuple *tuple1, Tuple *tuple2, Tuple *tuple3, int l1, int l2);
//return the intersection avl tree of T1 and T2
AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2);
//add child as the left child of the node
void AddToLeft(AVLTree *T, AVLTreeNode *node, AVLTreeNode *child);
//add child as the right child of the node
void AddToRight(AVLTree *T, AVLTreeNode *node, AVLTreeNode *child);
//return the larger value between a and b
int maxValue(int a, int b);
//adjust height from the given node to the root
void AdjustHeight(AVLTreeNode *n);
//insert a node as a binary search tree
AVLTreeNode *InsertAsBST(AVLTree *T, int k, int v);
//return the root of the subtree of the given node with larger height
AVLTreeNode *LargerHeightSubtreeRoot(AVLTreeNode *n);
//compute the height of the given node
void setHeight(AVLTreeNode *node);
//restructuring the avl tree to balanced tree and return the new root of the subtree
AVLTreeNode *Restructuring(AVLTree *T, AVLTreeNode *n);
//find the first unbalanced node from the given node to the root, and return that node
AVLTreeNode *FindUnbalancedNode(AVLTreeNode *n);
//insert a node storing (k,v) into an avl tree and return if succeed
int InsertNode(AVLTree *T, int k, int v);
//return the immediately inorder successor of the given node
AVLTreeNode *Successor(AVLTreeNode *node);
//delete the given node as binary search tree delete and return the parent of deleted node
AVLTreeNode *BSTDelete(AVLTree *T, AVLTreeNode *n);
//find and delete node stroing (k,v) and return the result
int DeleteNode(AVLTree *T, int k, int v);
//recursion, find a node storing (k,v) and return that node
AVLTreeNode *SearchHelper(AVLTreeNode *node, int k, int v);
//find a node storing (k,v) in the avl tree T and return that node
AVLTreeNode *Search(AVLTree *T, int k, int v);
//recursion, free all nodes from the root
void postOrderFree(AVLTreeNode *node);
//free all elements in the given tree
void FreeAVLTree(AVLTree *T);
//recursion, inorder (non-decreasing) print all elements
void inOrderPrint(AVLTreeNode *node);
//print all elements in a non-decreasing order in the given tree
void PrintAVLTree(AVLTree *T);


// time complexity: O(nlogn)
// one while loop runs n times
// InsertNode() in the while loop takes O(log) time 
// so the time comlexity is O(nlogn)
AVLTree *CreateAVLTree(const char *filename)
//create avl tree from a file or stdin and return the avl tree
{
	AVLTree *res = newAVLTree();
	// create from stdin
	if(strcmp(filename, "stdin") == 0)
	{
		char c;
		int flag1 = 1;
		int flag2 = 1;
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
	// create from a file	
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

// time comlexity: O(n)
// recursion takes O(n) time
AVLTreeNode *Clone(AVLTreeNode *node)
//recursion, clone avl tree from the root and return the root
{
	if(node == NULL)
	{
		return NULL;
	}
	AVLTreeNode *newNode = newAVLTreeNode(node->key, node->value);
	newNode->height = node->height;
	// add left
	newNode->left = Clone(node->left);
	if(newNode->left != NULL)
	{
		newNode->left->parent = newNode;
	}
	// add right
	newNode->right = Clone(node->right);
	if(newNode->right != NULL)
	{
		newNode->right->parent = newNode;
	}
	return newNode;
}

// time comlexity: O(n)
// Clone() takes O(n) time
AVLTree *CloneAVLTree(AVLTree *T)
//clone avl tree and return cloned avl tree
{
	AVLTree *newTree = newAVLTree();
	newTree->size = T->size;
	newTree->root = Clone(T->root);
	return newTree;
}
 
// time comlexity: O(n)
// recursion takes O(n) time
void BuildTupleArray(AVLTreeNode *node, Tuple *tuple, int *index)
//build a tuple array which saves all elements in avl tree in ascending order
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

// time comlexity: O(1)
int tupleCompare(Tuple tuple1, Tuple tuple2)
//compare two tuple(key, value), and return the result
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

// time comlexity: O(m+n)
// one while loop takes O(m+n) time
int UnionArray(Tuple *tuple1, Tuple *tuple2, Tuple *tuple3, int l1, int l2)
//use tuple1 and tuple2 to build an union array tuple3, and return the length of tuple3
{
	int i = 0;
	int j = 0;
	int k = 0;
	while(j < l1 || k < l2)
	{
		// if the pointer has been pointed to the last index, we save all the rest elements	
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
		// tuple1[j] < tuple2[k]
		if(tupleCompare(tuple1[j], tuple2[k]) < 0)
		{
			tuple3[i].key = tuple1[j].key;
			tuple3[i].value = tuple1[j].value;
			i++;
			j++;
		}
		// tuple1[j] > tuple2[k]
		else if(tupleCompare(tuple1[j], tuple2[k]) > 0)
		{
			tuple3[i].key = tuple2[k].key;
			tuple3[i].value = tuple2[k].value;
			i++;
			k++;
		}
		// equal
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

// time comlexity: O(n)
// recursion takes O(n) time
AVLTreeNode *BuildTreeFromArray(Tuple *tuple, int start, int last)
//use tuple array to build an avl tress.
{
	// base case
	if(last < start)
	{
		return NULL;
	}
	int mid = (start + last) / 2;
	int leftHeight = -1; 
	int rightHeight = -1;
	AVLTreeNode *node = newAVLTreeNode(tuple[mid].key, tuple[mid].value);
	// add left
	node->left = BuildTreeFromArray(tuple, start, mid - 1);
	if(node->left != NULL)
	{
		leftHeight = node->left->height;
		node->left->parent = node;
	}
	// add right
	node->right = BuildTreeFromArray(tuple, mid + 1, last);
	if(node->right != NULL)
	{
		rightHeight = node->right->height;
		node->right->parent = node;
	}
	// set height
	node->height = maxValue(leftHeight, rightHeight) + 1;
	return node; 
}

// time comlexity: O(m+n)
// BuildTupleArray(tuple1) takes O(m) time and BuildTupleArray(tuple2) takes O(n) time
// UnionArray() takes O(m+n) time
// BuildTreeFromArray() takes O(m+n) time
AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2)
//return the union avl tree of T1 and T2
{
	// init 3 tuple array storing elements in T1, T2 and T1 union T2.
	Tuple tuple1[T1->size], tuple2[T2->size], tuple3[T2->size + T1->size];
	int index = 0;
	int l;
	int leftHeight = 0;
	int rightHeight = 0;
	AVLTree *res = newAVLTree();
	// save all elements in T1 in tuple1 array in ascending order
	BuildTupleArray(T1->root, tuple1, &index);
	index = 0;
	// save all elements in T2 in tuple2 array in ascending order
	BuildTupleArray(T2->root, tuple2, &index);
	// save the union result in tuple3 array and set l as the length of the tuple3
	l = UnionArray(tuple1, tuple2, tuple3, T1->size, T2->size);
	// build avl tree from the tuple3 array and set the root of the avl tree
	res->root = BuildTreeFromArray(tuple3, 0, l - 1);
	res->size = l;
	return res;
}

// time comlexity: O(m+n)
// one while loop takes O(m+n) time
int IntersectArray(Tuple *tuple1, Tuple *tuple2, Tuple *tuple3, int l1, int l2)
//use tuple1 and tuple2 to build an intersection array tuple3, and return the length of tuple3
{
	int i = 0;
	int j = 0;
	int k = 0;
	while(j < l1 || k < l2)
	{
		// if the pointer has been pointed to the last index, we quit the while loop	
		if(j == l1)
		{
			break;
		}
		if(k == l2)
		{
			break;			
		}
		// tuple1[j] < tuple2[k]
		if(tupleCompare(tuple1[j], tuple2[k]) < 0)
		{
			j++;
		}
		// tuple1[j] > tuple2[k]
		else if(tupleCompare(tuple1[j], tuple2[k]) > 0)
		{
			k++;
		}
		// equal
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

// time comlexity: O(m+n)
// BuildTupleArray(tuple1) takes O(m) time
// BuildTupleArray(tuple2) takes O(n) time
// IntersectArray() takes O(m+n) time
// BuildTreeFromArray() takes O(m+n) time
AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2)
//return the intersection avl tree of T1 and T2
{
	// init 3 tuple array storing elements in T1, T2 and T1 intersect T2.
	Tuple tuple1[T1->size], tuple2[T2->size], tuple3[T1->size];
	int index = 0;
	int l;
	int leftHeight = 0;
	int rightHeight = 0;
	AVLTree *res = newAVLTree();
	// save all elements in T1 in tuple1 array in ascending order
	BuildTupleArray(T1->root, tuple1, &index);
	index = 0;
	// save all elements in T2 in tuple2 array in ascending order
	BuildTupleArray(T2->root, tuple2, &index);
	// save the intersection result in tuple3 array and set l as the length of the tuple3
	l = IntersectArray(tuple1, tuple2, tuple3, T1->size, T2->size);
	// build avl tree from the tuple3 array and set the root of the avl tree
	res->root = BuildTreeFromArray(tuple3, 0, l - 1);
	res->size = l;
	return res;
}

// time comlexity: O(logn)
// AdjustHeight() takes O(logn) time
void AddToLeft(AVLTree *T, AVLTreeNode *node, AVLTreeNode *child)
//add child as the left child of the node
{
	node->left = child;
	child->parent = node;
	(T->size++);
	AdjustHeight(child);
}

// time comlexity: O(logn)
// AdjustHeight() takes O(logn) time
void AddToRight(AVLTree *T, AVLTreeNode *node, AVLTreeNode *child)
//add child as the right child of the node
{
	node->right = child;
	child->parent = node;
	(T->size)++;
	AdjustHeight(child);
}

// time comlexity: O(1)
int maxValue(int a, int b)
//return the larger value between a and b
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

// time comlexity: O(logn)
// one while loop takes O(logn) time
// setHeight takes O(1) time
void AdjustHeight(AVLTreeNode *n)
//adjust height from the given node to the root
{
	AVLTreeNode *node = n;
	while(node != NULL)
	{
		setHeight(node);
		node = node->parent;
	}
}

// time complexity: O(logn)
// one while loop takes O(logn) time
// AddToLeft() and AddToRight() takes O(logn) time
AVLTreeNode *InsertAsBST(AVLTree *T, int k, int v)
//insert a node as a binary search tree
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
	// find the positon to insert the node
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
	// add the node
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

// time comlexity: O(1)
AVLTreeNode *LargerHeightSubtreeRoot(AVLTreeNode *n)
//return the root of the subtree of the given node with larger height
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

// time comlexity: O(1)
void setHeight(AVLTreeNode *node)
//compute the height of the given node
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

// time complexity: O(logn)
// LargerHeightSubtreeRoot() takes O(1) time
// setHeight() takes O(1) time
// AdjustHeight() takes O(logn) time
AVLTreeNode *Restructuring(AVLTree *T, AVLTreeNode *n)
//restructuring the avl tree to balanced tree and return the new root of the subtree
{
	AVLTreeNode *x = n;
	AVLTreeNode *y, *z;
	AVLTreeNode *a, *b, *c;
	AVLTreeNode *t1, *t2, *t3, *t4;
	y = LargerHeightSubtreeRoot(x);
	z = LargerHeightSubtreeRoot(y);
	// set a,b,c
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
	// adjust tree
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
	//set a
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
	//set b
	b->left = a;
	b->right = c;
	//set c
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
	setHeight(a);
	setHeight(c);
	setHeight(b);
	AdjustHeight(b);
	return b;
}

// time comlexity: O(logn)
// one while loop takes O(logn)
AVLTreeNode *FindUnbalancedNode(AVLTreeNode *n)
//find the first unbalanced node from the given node to the root, and return that node
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

// time comlexity: O(logn)
// InsertAsBST() takes O(logn) time
// FindUnbalancedNode() takes O(logn) time
// Restructuring() takes O(logn) time
int InsertNode(AVLTree *T, int k, int v)
//insert a node storing (k,v) into an avl tree and return if succeed
{
	if(T == NULL)
	{
		return 0;
	}
	// insert based on the Binary Search Tree rule
	AVLTreeNode *newNode = InsertAsBST(T, k, v);
	if(newNode != NULL)
	{
		AVLTreeNode *unbalancedNode = FindUnbalancedNode(newNode);
		//when inserting, there needs at most 1 adjustment
		if(unbalancedNode != NULL)
		{
			// adjust the tree to a balanced tree
			Restructuring(T, unbalancedNode);
		}
		return 1;
	}
	// item exists
	else
	{
		return 0;
	}
}

// time comlexity: O(logn)
// one while loop takes O(logn) time
AVLTreeNode *Successor(AVLTreeNode *node)
//return the immediately inorder successor of the given node
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

// time complexity: O(logn)
// AdjustHeight() takes O(logn) time
// Successor() takes O(logn) time
AVLTreeNode *BSTDelete(AVLTree *T, AVLTreeNode *n)
//delete the given node as binary search tree delete and return the parent of deleted node
{
	if(n == NULL)
	{
		return NULL;
	}
	// if the node has no child, delete it directly
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
	// the node has only left child, delete it and make its left child as the child of its parent
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
	// the node has only right child, delete it and make its right child as the child of its parent
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
	// if the node has two children, we delete its immediately inorder successor
	else
	{
		AVLTreeNode *succ = Successor(n);
		n->key = succ->key;
		n->value = succ->value;
		return (BSTDelete(T, succ));
	}
}

// time complexity: O(logn)
// Search() takes O(logn) time
// BSTDelete() takes O(logn) time 
// one loop runs constant times
// FindUnbalancedNode() takes O(log) time
// Restructuring() takes O(log) time
int DeleteNode(AVLTree *T, int k, int v)
//find and delete node stroing (k,v) and return the result
{
	if(T == NULL)
	{
		return 0;
	}
	// find if (k,v) exists in the tree
	AVLTreeNode *node = Search(T, k, v);
	// if not exists, return 0
	if(node == NULL)
	{
		return 0;
	}
	else
	{
		// delete that node based on the Binary Search Tree rule
		AVLTreeNode *tempNode = BSTDelete(T, node);
		// check and adjust balance until the tree becomes balanced
		while(1)
		{
			// find the first unbalanced node from the temp node to the root
			AVLTreeNode *unbalancedNode = FindUnbalancedNode(tempNode);
			if(unbalancedNode != NULL)
			{
				// adjust the unbalaced subtree and get the root of the unbalanced subtree
				tempNode = Restructuring(T, unbalancedNode);
			}
			else
			{
				break;
			}
		}
		return 1;
	}
}

// time complexity: O(logn)
// recursion takes O(logn) time
AVLTreeNode *SearchHelper(AVLTreeNode *node, int k, int v)
//recursion, find a node storing (k,v) and return that node
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

// time complexity: O(logn)
// SearchHelper() takes O(logn) time
AVLTreeNode *Search(AVLTree *T, int k, int v)
//find a node storing (k,v) in the avl tree T and return that node
{
	if(T == NULL)
	{
		return NULL;
	}
	// return the node storing (k,v)
	return SearchHelper(T->root, k, v);
}

// time comlexity: O(n)
// recursion takes O(n) time
void postOrderFree(AVLTreeNode *node)
//recursion, free all nodes from the root
{
	if(node == NULL)
	{
		return;
	}
	postOrderFree(node->left);
	postOrderFree(node->right);
	free(node);
}

// time complexity: O(n)
// postOrderFree() takes O(n) time
void FreeAVLTree(AVLTree *T)
//free all elements in the given tree
{
	if(T == NULL)
	{
		return;
	}	
	// free all nodes
	postOrderFree(T->root);
	// free T
	free(T);
}

// time complexity: O(n)
// recursion takes O(n) time
void inOrderPrint(AVLTreeNode *node)
//recursion, inorder (non-decreasing) print all elements
{
	if(node == NULL)
	{
		return;
	}
	inOrderPrint(node->left);
	printf("(%d, %d), %d\n", node->key, node->value, node->height);
	inOrderPrint(node->right);
}

// time complexity: O(n)
// inOrderPrint() takes O(n) time
void PrintAVLTree(AVLTree *T)
//print all elements in a non-decreasing order in the given tree
{
	if(T == NULL)
	{

		return;
	}
	// in-order print all elements in the tree
	inOrderPrint(T->root);
}

int main() //sample main for testing 
{ int i,j;
 AVLTree *tree1, *tree2, *tree3, *tree4, *tree5, *tree6, *tree7, *tree8;
 AVLTreeNode *node1;
 
 tree1=CreateAVLTree("stdin");
 PrintAVLTree(tree1);
 FreeAVLTree(tree1);
 //you need to create the text file file1.txt
 // to store a set of items without duplicate items
 tree2=CreateAVLTree("file1.txt"); 
 PrintAVLTree(tree2);
 tree3=CloneAVLTree(tree2);
 PrintAVLTree(tree3);
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
    PrintAVLTree(tree4);
  }
 FreeAVLTree(tree4);
 //Create tree5
 tree5=newAVLTree();
 j=InsertNode(tree5, 6, 25);
 j=InsertNode(tree5, 6, 10);
 j=InsertNode(tree5, 6, 12);
 j=InsertNode(tree5, 6, 20);
 j=InsertNode(tree5, 9, 25);
 j=InsertNode(tree5, 10, 25);
 PrintAVLTree(tree5);
 //Create tree6
 tree6=newAVLTree();
 j=InsertNode(tree6, 6, 25);
 j=InsertNode(tree6, 5, 10);
 j=InsertNode(tree6, 6, 12);
 j=InsertNode(tree6, 6, 20);
 j=InsertNode(tree6, 8, 35);
 j=InsertNode(tree6, 10, 25);
 PrintAVLTree(tree6);
 tree7=AVLTreesIntersection(tree5, tree6);
 tree8=AVLTreesUnion(tree5,tree6);
 PrintAVLTree(tree7);
 PrintAVLTree(tree8);
 return 0; 
}