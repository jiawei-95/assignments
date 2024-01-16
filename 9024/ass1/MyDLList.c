#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like 

// data structures representing DLList

// data type for nodes
typedef struct DLListNode {
	int  value;  // value (int) of this list item 
	struct DLListNode *prev;
	// pointer previous node in list
	struct DLListNode *next;
	// pointer to next node in list
} DLListNode;

//data type for doubly linked lists
typedef struct DLList{
	int  size;      // count of items in list
	DLListNode *first; // first node in list
	DLListNode *last;  // last node in list
} DLList;

// create a new DLListNode
DLListNode *newDLListNode(int it)
{
	DLListNode *new;
	new = malloc(sizeof(DLListNode));
	assert(new != NULL);
	new->value = it;
	new->prev = new->next = NULL;
	return new;
}

// create a new empty DLList
DLList *newDLList()
{
	DLList *L;

	L = malloc(sizeof (struct DLList));
	assert (L != NULL);
	L->size = 0;
	L->first = NULL;
	L->last = NULL;
	return L;
}

// append a node to the rear
// time complexity: O(1)
int addToBack(DLList *L, int v)
{
	if(L == NULL)
	{
		return 0;
	}
	//empty list
	if(L->size == 0)
	{
		DLListNode *node = newDLListNode(v);
		L->first = node;
		L->last = node;
		(L->size)++;
	}
	//unempty list
	else
	{
		DLListNode *last = L->last;
		DLListNode *node = newDLListNode(v);
		last->next = node;
		node->prev = last;
		L->last = node;
		(L->size)++;
	}
	return 1;
}

// check if a value v is in the list
// time complexity: O(n) -- n-element DLList traversal
int findValue(DLList *L, int v)
{
	if(L == NULL)
	{
		return 0;
	}
	DLListNode *node = L->first;
	while(node != NULL)
	{
		if(node->value == v)
		{
			return 1;
		}
		node = node->next;
	}
	return 0;
}

// create a DLList from a text file
// put your time complexity analysis for CreateDLListFromFileDlist() here
// time complexity: O(n) -- one while loop takes O(n) time
DLList *CreateDLListFromFileDlist(const char *filename)
{
	DLList *resultDLList = newDLList();
	// create a DLList from stdin
	if(strcmp(filename, "stdin") == 0)
	{
		int res = 1;
		char buff[50];
		int num;
		while(res == 1)
		{
			fgets(buff, sizeof(buff), stdin);
			res = sscanf(buff, "%d", &num);
			if(res == 1)
			{
				addToBack(resultDLList, num);
			}
		}
	}
	// create a DLList from a file
	else
	{
		FILE *fp;
		int num;
		fp = fopen(filename, "r");
		if(fp == NULL)
		{
			printf("Cannot open file or file does not exist.\n");
			exit(0);
		}
		while(fscanf(fp, "%d", &num) != EOF)
		{
			addToBack(resultDLList, num);
		}
	}
	return resultDLList;
}

// clone a DLList
// put your time complexity analysis for cloneList() here
// time complexity: O(n) -- one while loop takes O(n) time
DLList *cloneList(DLList *u)
{
	if(u == NULL)
	{
		return NULL;
	}
	DLList *resultDLList = newDLList();
	DLListNode *node = u->first;
	while(node != NULL)
	{
		addToBack(resultDLList, node->value);
		node = node->next;
	}
	return resultDLList;
  
}

// compute the union of two DLLists u and v
// time complexity: O(m*n) -- cloneList takes O(m) time, one while loop takes O(m*n) time
// because findValue inside the while loop takes O(n) time
DLList *setUnion(DLList *u, DLList *v)
{
	//add all elements in u first
	DLList *resultDLList = cloneList(u);
	DLListNode *node = v->first;
	while(node != NULL)
	{
		// add elements which are not in u but in v
		if(findValue(u, node->value) == 0)
		{
			addToBack(resultDLList, node->value);
		}
		node = node->next;
	}
	return resultDLList;
}

// compute the insection of two DLLists u and v
// put your time complexity analysis for intersection() here
// time complexity: O(m*n) -- one while loop takes O(m*n) time 
// because findValue inside the while loop takes O(n) time
DLList *setIntersection(DLList *u, DLList *v)
{
	DLList *resultDLList = newDLList();
	DLListNode *node = u->first;
	while(node != NULL)
	{
		// add elements which are not only in u but also in v
		if(findValue(v, node->value) == 1)
		{
			addToBack(resultDLList, node->value);
		}
		node = node->next;
	}
	return resultDLList;
}

// free up all space associated with list
// put your time complexity analysis for freeDLList() here
// time complexity: O(n) -- one while loop takes O(n) time
void freeDLList(DLList *L)
{
	if(L == NULL)
	{
		return;
	}
	DLListNode *node = L->first;
	DLListNode *nextNode;
	while(node != NULL)
	{
		nextNode = node->next;
		free(node);
		node = nextNode;
	}
	free(L);
	return;
}


// display items of a DLList
// put your time complexity analysis for printDDList() here
// time complexity: O(n) -- one while loop takes O(n) time
void printDLList(DLList *u)
{
	if(u == NULL)
	{
		return;
	}
	DLListNode *node = u->first;
	//printf("Elements in the DLList are: \n");
	while(node != NULL)
	{
		printf("%d\n", node->value);
		node = node->next;
	}
	return;
}

int main()
{
 DLList *list1, *list2, *list3, *list4;

 list1=CreateDLListFromFileDlist("File1.txt");
 printDLList(list1);

 list2=CreateDLListFromFileDlist("File2.txt");
 printDLList(list2);

 list3=setUnion(list1, list2);
 printDLList(list3);

 list4=setIntersection(list1, list2);
 printDLList(list4);

 freeDLList(list1);
 freeDLList(list2);
 freeDLList(list3);
 freeDLList(list4);
 
 printf("please type all the integers of list1\n");
 list1=CreateDLListFromFileDlist("stdin");

 printf("please type all the integers of list2\n");
 list2=CreateDLListFromFileDlist("stdin");

 list3=cloneList(list1);
 printDLList(list3);
 list4=cloneList(list2);
 printDLList(list4);

 freeDLList(list1);
 freeDLList(list2);
 freeDLList(list3);
 freeDLList(list4);

 return 0; 
}
