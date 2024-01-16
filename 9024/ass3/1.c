#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

// This template is only a guide 
// You need to include additional fields, data structures and functions  

// data type for heap nodes
typedef struct HeapNode { 
	// each node stores the priority (key), name, execution time,
	//  release time and deadline of one task
	int key; //key of this task
	int TaskName;  //task name 
	int Etime; //execution time of this task
	int Rtime; // release time of this task
	int Dline; // deadline of this task
	struct HeapNode *child; // first child pointer
	struct HeapNode *sibling; // sibling pointer
	int degree; // number of children
} HeapNode;

//data type for a priority queue (heap) 
typedef struct BinomialHeap{ //this is heap header
	int  size;      // count of items in the heap
	HeapNode *head; // pointer to the root of the first heap
} BinomialHeap;

typedef struct Task{
	int name;
	int Etime;
	int Rtime;
	int Dline;
} Task;

// create a new heap node to store an item (task) 
HeapNode *newHeapNode(int k, int n, int c, int r, int d)
{ // k:key, n:task name, c: execution time, r:release time, d:deadline
  // ... you need to define other parameters yourself) 	 
	HeapNode *new;
	new = malloc(sizeof(HeapNode));
	assert(new != NULL);
	new->key = k;
	new->TaskName = n;
	new->Etime = c;
	new->Rtime = r; 
	new->Dline = d;
	new->child = NULL;
	new->sibling = NULL;
	new->degree = 0;
	return new;
}

// create a new empty heap-based priority queue
BinomialHeap *newHeap()
{ // this function creates an empty binomial heap-based priority queue
	BinomialHeap *T;
	T = malloc(sizeof(BinomialHeap));
	assert (T != NULL);
	T->head = NULL;
	return T;
}


// merge largeNode as the child of the smallNode
// time complexity: O(1)
void addSubTree(HeapNode *smallNode, HeapNode *largeNode)
{
	// add largeNode as the child of the smallNode
	largeNode->sibling = smallNode->child;
	smallNode->child = largeNode;
	(smallNode->degree)++;
	smallNode->sibling = NULL;
}

// Merge two same degree tree and return the merged tree
// time complexity: O(1)
HeapNode *MergeSameDegree(HeapNode *node1, HeapNode *node2)
{
	if(node1->key < node2->key)
	{
		addSubTree(node1, node2);
		return node1;
	}
	else if(node1->key > node2->key)
	{
		addSubTree(node2, node1);
		return node2;
	}
	// equal
	else
	{
		addSubTree(node1, node2);
		return node1;
	}
}

// time complexity: O(log(m+n))
// Merge T1 and T2 into a new binomial heap and return that heap
BinomialHeap *Merge(BinomialHeap *T1, BinomialHeap *T2)
{
	if(T1 == NULL || T1->head == NULL)
	{
		return T2;
	}
	if(T2 == NULL || T2->head == NULL)
	{
		return T1;
	}
	BinomialHeap *res = newHeap();
	res->size = T1->size + T2->size;
	HeapNode *node1 = T1->head;
	HeapNode *node2 = T2->head;
	HeapNode *node3 = NULL;
	HeapNode *preNode = NULL;
	// merge heap1 and heap2 into heap3
	while(node1 != NULL && node2 != NULL)
	{
		HeapNode *tempNode;
		if(node1->degree < node2->degree)
		{
			// add node1
			tempNode = node1;
			node1 = node1->sibling;
		}
		else if(node1->degree > node2->degree)
		{
			// next pointer
			tempNode = node2;
			node2 = node2->sibling;
		}
		else
		{
			// merge two node
			HeapNode *n1 = node1;
			HeapNode *n2 = node2;
			node1 = node1->sibling;
			node2 = node2->sibling;
			tempNode = MergeSameDegree(n1, n2);
		}
		if(res->head == NULL)
		{
			// add at the head
			res->head = tempNode;
			node3 = tempNode;
			preNode = node3;
		}
		else
		{
			if(node3->degree == tempNode->degree)
			{
				// res has a same-degree node with the added node
				if(node3 == res->head)
				{
					// add as head
					res->head = MergeSameDegree(node3, tempNode);
					node3 = res->head;
					preNode = node3;
				}
				else
				{
					preNode->sibling = MergeSameDegree(node3, tempNode);
					preNode = node3;
					node3 = preNode->sibling;
				}
			}
			else
			{
				// directly add to back
				node3->sibling = tempNode;
				preNode = node3;
				node3 = tempNode;
			}
		}
	}
	// set list which has not finished
	if(node1 == NULL)
	{
		node1 = node2;
	}
	// add each node to the back
	while(node1 != NULL)
	{
		if(res->head == NULL)
		{
			// add as head
			res->head = node1;
			node1 = node1->sibling;
			node3 = res->head;
			preNode = node3;
		}
		else
		{
			// add to back
			if(node3->degree == node1->degree)
			{
				// res has a same-degree node with the added node
				if(res->head == node3)
				{
					// add as head
					HeapNode *tempNode = node1;
					node1 = node1->sibling;
					res->head = MergeSameDegree(node3, tempNode);
					node3 = res->head;
					preNode = node3;
				}
				else
				{
					// merge and add to back
					HeapNode *tempNode = node1;
					node1 = node1->sibling;
					preNode->sibling = MergeSameDegree(node3, tempNode);
					preNode = node3;
					node3 = preNode->sibling;
				}
			}
			else
			{
				// directly add to back
				node3->sibling = node1;
				preNode = node3;
				node3 = node1;
				node1 = node1->sibling;
			}
		}
	}
	return res;


}

// time comlexity: O(logn)
// init a single-node heap and merge it with T
// Merge() takes O(log(n+1)) time, which is equal to O(logn)    
void Insert(BinomialHeap *T, int k, int n, int c, int r, int d)
{ // k: key, n: task name, c: execution time, r: release time, d:deadline 
  // You don't need to check if this task already exists in T
	HeapNode *newNode = newHeapNode(k, n, c, r, d);
	// build a single-node heap
	BinomialHeap *tempHeap = newHeap();
	tempHeap->size = 1;
	tempHeap->head = newNode;
	// merge two heap and set T
	tempHeap = Merge(T, tempHeap);
	T->head = tempHeap->head;
	T->size = tempHeap->size;
}

// time complexity: O(logn)
// find the min node and set its child as the head of a new heap. Merge new heap and rest of T
// while loop takes O(logn) time
// reverse the child list takes O(logn) time
// Merge() takes O(log(n-1) time, which is equal to O(logn)
HeapNode *RemoveMin(BinomialHeap *T)
{
	if(T == NULL || T->head == NULL)
	{
		return NULL;
	}
	HeapNode *node = T->head;
	HeapNode *preNode = NULL;
	HeapNode *minNode = node;
	HeapNode *minPreNode = NULL;
	// find the node with min key
	while(node != NULL)
	{
		if(node->key < minNode->key)
		{
			minPreNode = preNode;
			minNode = node;
		}
		preNode = node;
		node = node->sibling;
	}
	// build a new heap
	BinomialHeap *tempHeap = newHeap();
	tempHeap->head = minNode->child;
	node = tempHeap->head;
	preNode = node;
	// reverse the list in ascending oreder
	while(node != NULL)
	{
		if(node == preNode)
		{
			HeapNode *tempNode = node;
			node = node->sibling;
			tempNode->sibling = NULL;
		}
		else
		{
			HeapNode *tempNode = node;
			node = node->sibling;
			tempNode->sibling = preNode;
			preNode = tempNode;
		}
		if(node == NULL)
		{
			tempHeap->head = preNode;
		}
	}
	tempHeap->size = ((int)pow(2, minNode->degree)) - 1;
	// reorganize the heap T
	if(minNode == T->head)
	{
		T->head = minNode->sibling;
	}
	else
	{
		minPreNode->sibling = minNode->sibling;
	}
	T->size = T->size - (int)pow(2, minNode->degree);
	// Merge two heap and set T
	tempHeap = Merge(T, tempHeap);
	T->head = tempHeap->head;
	T->size = tempHeap->size;
	return minNode;
}


// time comlexity: O(logn)
// return the min key of the heap. if empty, return -1
// while loop takes O(logn) time
int Min(BinomialHeap *T)
{
	if(T == NULL)
	{
		return -1;
	}
	HeapNode *node = T->head;
	if(node != NULL)
	{
		int minKey = node->key;
		while(node != NULL)
		{
			if(node->key < minKey)
			{
				minKey = node->key;
			}
			node = node->sibling;
		}
		return minKey;
	}
	else
	{
		return -1;
	}
}

// print the heap
void PrintHelper(HeapNode *node)
{
	if(node == NULL)
	{
		return;
	}
	printf("(degree: %d, key: %d, Name: %d)\n", node->degree, node->key, node->TaskName);
	PrintHelper(node->child);
	PrintHelper(node->sibling);
}

//print the heap
void PrintHeap(BinomialHeap *T)
{
	HeapNode *node = T->head;
	PrintHelper(node);
}

// time complexity: O(nlogn)
// two while loops take O(nlogn) time and one while loop takes O(mlogn) time
// as m is smaller than n, the time complexity is O(nlogn)
int TaskScheduler(char *f1, char *f2, int m )
{
	int res = 1;
	FILE *fp, *fp2;
	fp = fopen(f1, "r");
	if(fp == NULL)
	{
		printf("Cannot open file or file does not exist.\n");
		exit(0);
	}
	fp2 = fopen(f2, "w");
	if(fp2 == NULL)
	{
		printf("Cannot open file or file does not exist.\n");
		exit(0);
	}
	BinomialHeap *RHeap = newHeap();
	BinomialHeap *DHeap = newHeap();
	BinomialHeap *CoreHeap = newHeap();
	Task t;
	t.name = -1;
	int flag = 0;
	// read file and init Release time heap takes O(nlogn) time
	while(1)
	{
		flag = fscanf(fp, "%d %d %d %d", &(t.name), &(t.Etime), &(t.Rtime), &(t.Dline));
		if(flag == EOF)
		{
			break;
		}
		if(flag != 4 || t.Etime < 0 || t.Rtime < 0 || t.Dline < 0)
		{
			if(t.name == -1)
			{
				printf("input error when reading the attribute of the first task\n");
			}
			else
			{
				printf("input error when reading the attribute of the task %d\n", t.name);
			}
			exit(0);
		}
		//printf("name: %d, Etime: %d, Rtime: %d, Dline: %d\n", t.name, t.Etime, t.Rtime, t.Dline);	
		Insert(RHeap, t.Rtime, t.name, t.Etime, t.Rtime, t.Dline);
	}
	// init core heap, set key as the ready time of that core
	// This step takes O(mlogm)
	for(int i = 0; i < m; i++)
	{
		Insert(CoreHeap, 0, i + 1, 0, 0, 0);
	}
	// O(nlogn)
	// initial RHeap is full and initial DHeap is empty
	// every time we get a schedule point from coreHeap and move ready taskt from RHeap to DHeap
	// get the min node from RHeap based on EDF and reset the core ready time
	// This loop takes O(nlogn + nlogn + kmlogm) time
	// As we can assume m is smaller than n.
	// Time complexity of this while loop is O(nlogn).
	while(1)
	{
		if(DHeap->size == 0 && RHeap->size == 0)
		{
			break;
		}
		int schedulePoint;
		int coreName;
		HeapNode *minCoreNode = RemoveMin(CoreHeap);
		schedulePoint = minCoreNode->key;
		coreName = minCoreNode->TaskName;
		if(Min(RHeap) <= schedulePoint && Min(RHeap) >= 0)
		{
			while(Min(RHeap) <= schedulePoint && Min(RHeap) >= 0)
			{
				HeapNode *tempNode = RemoveMin(RHeap);
				Insert(DHeap, tempNode->Dline, tempNode->TaskName, tempNode->Etime, tempNode->Rtime, tempNode->Dline);
			}
			HeapNode *planNode = RemoveMin(DHeap);
			if(schedulePoint + planNode->Etime > planNode->Dline)
			{
				res = 0;
			}
			fprintf(fp2, "%d Core%d %d\n", planNode->TaskName, coreName, schedulePoint);
			Insert(CoreHeap, schedulePoint + planNode->Etime, coreName, 0, 0, 0);		
		}
		else
		{

			if(DHeap->size == 0)
			{
				Insert(CoreHeap, Min(RHeap), coreName, 0, 0, 0);
			}
			else
			{
				HeapNode *planNode = RemoveMin(DHeap);
				if(schedulePoint + planNode->Etime > planNode->Dline)
				{
					res = 0;
				}
				fprintf(fp2, "%d Core%d %d\n", planNode->TaskName, coreName, schedulePoint);
				Insert(CoreHeap, schedulePoint + planNode->Etime, coreName, 0, 0, 0);	
			}
		}
	}
	return res;
}

int main() //sample main for testing 
{ int i;
  i=TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4);
  if (i==0) printf("No feasible schedule!\n");
  /* There is a feasible schedule on 4 cores */
  i=TaskScheduler("samplefile1.txt", "feasibleschedule2.txt", 3);
  if (i==0) printf("No feasible schedule!\n");
  /* There is no feasible schedule on 3 cores */
  i=TaskScheduler("samplefile2.txt", "feasibleschedule3.txt", 5);
  if (i==0) printf("No feasible schedule!\n");
  /* There is a feasible schedule on 5 cores */
  i=TaskScheduler("samplefile2.txt", "feasibleschedule4.txt", 4);
  if (i==0) printf("No feasible schedule!\n");
  /* There is no feasible schedule on 4 cores */
  i=TaskScheduler("samplefile3.txt", "feasibleschedule5.txt", 2);
  if (i==0) printf("No feasible schedule!\n");
  /* There is no feasible schedule on 2 cores */
  i=TaskScheduler("samplefile4.txt", "feasibleschedule6.txt", 2);
  if (i==0) printf("No feasible schedule!\n");
  /* There is a feasible schedule on 2 cores */
 return 0; 
}
