#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#define MAXSIZE 100
#define MYINFINITY 99999
// A vertex is a 2D point
typedef struct Vertex { int x; // x-coordinate
	int y; // y-coordinate
} Vertex;

typedef struct Queue { 
	int front; // x-coordinate
	int last; // y-coordinate
	struct VertexNode **q;
} Queue;

// each edge is a pair of vertices (end-points)
typedef struct Edge {
	Vertex *p1; // first end point
	Vertex *p2; // second end point
} Edge;

typedef struct VertexNode {
	Vertex *v;
	int state;
	float distance;
	struct VertexNode *nextVertex; // pointer to the next vertex
	struct VertexNode *preVertex; // pointer to the previous vertex
	struct VertexNode *adjacentVertex; // pointer to the next value in the edge linked list
	struct VertexNode *preAdjacentVertex; // pointer to the privious value in the edge linked list
	struct HeapNode *heapPointer; // pointer to the corresponding node in the heap
	struct VertexNode *vertexPointer; // pointer to its corresponding vertex node for edge node
	struct VertexNode *edgePointer; // pointer to its coreesponding edge node for edge node
} VertexNode;

typedef struct HeapNode {
	int position; // index of the heap array
	Vertex *v; // information about this vertex
	float key; // distance from u
	VertexNode *vn; // pointer to the corrsponding vertex which has the same vertex information
	struct HeapNode *pre; // pointer to anther heapnode for printing the path
} HeapNode;

typedef struct HeapHead {
	int last; // index of the last element
	HeapNode **h; // heap array
} HeapHead;

typedef struct GraphRep *Graph;
typedef struct GraphRep { // graph header
	VertexNode *vertices; // an array of vertices or a linked list of vertices  
	int nV; // #vertices
	int nE; // #edges
} GraphRep;

// A vertex node stores a vertex and other information, and you need to expand this type

//The above types serve as a starting point only. You need to expand them and add more types. 
// Watch the lecture video between 7:50pm-8:20 or so for my discussion about this assignment    

// time comlexity: O(1)
// create an empty graph
Graph CreateEmptyGraph()
{
	Graph g = (Graph) malloc(sizeof(GraphRep));
	assert(g != NULL);
	g->nV = 0;
	g->nE = 0;
	g->vertices = NULL;
	return g;
}

// time comlexity: O(1)
// judge if two vertex has the same value
int sameVertex(Vertex *v1, Vertex *v2)
{
	if(v1->x == v2->x && v1->y == v2->y)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

// time comlexity: O(n)
// return the vertexNode storing vertex x
VertexNode *findVertex(Graph g, Vertex *v)
{
	VertexNode *node = g->vertices;
	while(node != NULL)
	{
		if(sameVertex(node->v, v))
		{
			return node;
		}
		node = node->nextVertex;
	}
	return NULL;
}

// time comlexity: O(1)
// clone a vertex
Vertex *cloneVertex(Vertex *v)
{
	Vertex *res = (Vertex *) malloc(sizeof(Vertex));
	res->x = v->x;
	res->y = v->y;
	return res;
}

// time comlexity: O(n)
// find the last position of the vertex linked list and add a new element to the back
VertexNode *appendVertex(Graph g, Vertex *v)
{
	// create a new node
	VertexNode *vn = (VertexNode *)malloc(sizeof(VertexNode));
	Vertex *tv = cloneVertex(v);
	vn->v = tv;
	vn->state = 0;
	vn->distance = 0;
	vn->nextVertex = NULL;
	vn->preVertex = NULL;
	vn->adjacentVertex = NULL;
	vn->preAdjacentVertex = NULL;
	vn->heapPointer = NULL;
	vn->vertexPointer = NULL;
	vn->edgePointer = NULL;
	// get the first vertex node in graph
	VertexNode *node = g->vertices;
	// there was no vertex node in graph
	if(node == NULL)
	{
		g->vertices = vn;
		return vn;
	}
	// find the last node
	while(node->nextVertex != NULL)
	{
		node = node->nextVertex;
	}
	node->nextVertex = vn;
	node->nextVertex->preVertex = node;
	return vn;
}

// time comlexity: O(m)
// find a edge in a linked list
VertexNode *findEdge(VertexNode *vn, Vertex *v)
{
	VertexNode *node = vn->adjacentVertex;
	while(node != NULL)
	{
		if(sameVertex(node->v, v))
		{
			return node;
		}
		node = node->adjacentVertex;
	}
	return NULL;
}

// time comlexity: O(1)
// compute the distance of two point
float getDistance(VertexNode *vn, Vertex *v)
{
	float res = sqrt(pow((vn->v->x - v->x), 2) + pow((vn->v->y - v->y), 2));
	return res;
}
// time comlexity: O(m)
// find the last positon of the edge linked list and append a new element
VertexNode *appendEdge(VertexNode *vn, Vertex *v)
{
	// create a new node
	VertexNode *vn1 = (VertexNode *)malloc(sizeof(VertexNode));
	Vertex *tv = cloneVertex(v);
	vn1->v = tv;
	vn1->state = 0;
	vn1->distance = getDistance(vn, v);
	vn1->nextVertex = NULL;
	vn1->preVertex = NULL;
	vn1->adjacentVertex = NULL;
	vn1->preAdjacentVertex = NULL;
	vn1->heapPointer = NULL;
	vn1->vertexPointer = NULL;
	vn1->edgePointer = NULL;
	// get the first edge node
	VertexNode *vn2 = vn->adjacentVertex;
	if(vn2 == NULL)
	{
		vn->adjacentVertex = vn1;
		return vn1;
	}
	// find the last node
	while(vn2->adjacentVertex != NULL)
	{
		vn2 = vn2->adjacentVertex;
	}	
	vn2->adjacentVertex = vn1;
	vn2->adjacentVertex->preAdjacentVertex = vn2;
	return vn1;	
}

// time comlexity: O(m + n), where m is the number of edges and n is the number of vertices
// findVertex() takes O(n) time and appendVertex() takes O(n) time
// findEdge() takes O(m) time and appendEdge() takes O(m) time
//
// add a new edge into the graph
int InsertEdge(Graph g, Edge *e)
{
	Vertex *v1 = e->p1;
	Vertex *v2 = e->p2;
	// find the vertex node storing v1
	VertexNode *vn1 = findVertex(g, v1);
	if(vn1 == NULL)
	{	
		vn1 = appendVertex(g, v1);
	}
	else
	{
		VertexNode *v3 = findEdge(vn1, v2);
		// edge already exists
		if(v3 != NULL)
		{
			return 0;
		}	
	}
	// find the vertex node storing v2
	VertexNode *vn2 = findVertex(g, v2);
	if(vn2 == NULL)
	{	
		vn2 = appendVertex(g, v2);
	}
	VertexNode *v5 = appendEdge(vn1, v2);
	VertexNode *v6 = appendEdge(vn2, v1);
	// connect the pointer
	v5->vertexPointer = vn2;
	v6->vertexPointer = vn1;
	v5->edgePointer = v6;
	v6->edgePointer = v5;
	return 1;
}

// time comlexity: O(n)
// remove a vertex from the vertices linked list
void DeleteVertexFromList(Graph g, VertexNode *vn)
{
	VertexNode *node = g->vertices;
	if(sameVertex(node->v, vn->v))
	{
		g->vertices = node->nextVertex;
		if(node->nextVertex != NULL)
		{
			node->nextVertex->preVertex = NULL;
		}
		free(node->v);
		free(node);
		return;
	}
	// find the position of vn and delete it
	while(node != NULL)
	{
		if(sameVertex(node->v, vn->v))
		{
			node->preVertex->nextVertex = node->nextVertex;
			if(node->nextVertex != NULL)
			{
				node->nextVertex->preVertex = node->preVertex;
			}
			free(node->v);
			free(node);
			return;
		}
		node = node->nextVertex;
	}
}

// time complexity: O(m)
// remove a edge from the edge linked list
void DeleteEdgeFromList(VertexNode *vn, Vertex *v)
{
	VertexNode *node = vn->adjacentVertex;
	if(sameVertex(node->v, v))
	{
		vn->adjacentVertex = node->adjacentVertex;
		if(node->adjacentVertex != NULL)
		{
			node->adjacentVertex->preAdjacentVertex = NULL;
		}
		free(node->v);
		free(node);
		return;
	}
	// find the position of v and delete it
	while(node != NULL)
	{
		if(sameVertex(v, node->v))
		{
			node->preAdjacentVertex->adjacentVertex = node->adjacentVertex;
			if(node->adjacentVertex != NULL)
			{	
				node->adjacentVertex->preAdjacentVertex = node->preAdjacentVertex;
			}
			free(node->v);
			free(node);
			return;
		}
		node = node->adjacentVertex;
	}
}

// time comlexity: O(m + n), where m is the number of edges and n is the number of vertices
// findVertex() takes O(n) time
// DeleteEdgeFromList() takes O(m) time
// DeleteVertexFromList() takes O(n) time
//
// delete a edge from the graph
void DeleteEdge(Graph g, Edge *e)
{
	// find the vertex node of the edge
	VertexNode *vn1 = findVertex(g, e->p1);
	VertexNode *vn2 = findVertex(g, e->p2);
	// edge not in graph
	if(vn1 == NULL || vn2 == NULL)
	{
		return;
	}
	// delete two edges
	DeleteEdgeFromList(vn1, e->p2);
	DeleteEdgeFromList(vn2, e->p1);
	// deal with isolated vertex
	if(vn1->adjacentVertex == NULL)
	{
		DeleteVertexFromList(g, vn1);
	}
	if(vn2->adjacentVertex == NULL)
	{
		DeleteVertexFromList(g, vn2);
	}
}

// time complexity: O(1)
// create an empty queque
Queue *CreateQueue()
{
	Queue *q = (Queue *) malloc(sizeof(Queue));
	q->front = 0;
	q->last = 0;
	q->q = malloc(MAXSIZE * sizeof(VertexNode *));
	return q;
}

// time complexity: O(1)
// judge if queue is empty
int isEmpty(Queue *q)
{
	if(q->front == q->last)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

// time comlexity: O(1)
// insert into the queue
void enqueue(Queue *q, VertexNode *vn)
{
	(q->q)[q->last] = vn;
	(q->last)++;
}

// time complexity: O(1)
// get and remove the first item in queue
VertexNode *dequeue(Queue *q)
{
	if(q->last == q->front)
	{
		return NULL;
	}
	VertexNode *res = (q->q)[q->front];
	(q->front)++;
	return res;
}

// time comlexity: O(n)
// set the state of each vertex to 0
void initVertexState(Graph g)
{
	VertexNode *vn = g->vertices;
	while(vn != NULL)
	{
		vn->state = 0;
		vn = vn->nextVertex;
	}
}

// time complexity: O(m + n), where m is the number of edges and n is the number of vertices
// findVertex() takes O(n) time
// initVertexState() takes O(n) time
// CreateQueue(), enqueue(), dequeue(), isEmpty() and sameVertex() takes O(1) time
// while loop takes O(m) time
// BFS visits each vertex once and each edge twice, so the time comlexity is O(m + n)
//
// use BFS and print all reachable vertices from v in the graph
void ReachableVertices(Graph g, Vertex *v)
{
	VertexNode *vn1 = findVertex(g, v);
	if(vn1 == NULL)
	{
		return;
	}
	// set the state of each vertex to 0
	initVertexState(g);
	Queue *q = CreateQueue();
	// enqueue v
	enqueue(q, vn1);
	vn1->state = 1;
	while(isEmpty(q) != 1)
	{
		VertexNode *vv = dequeue(q);
		if(sameVertex(vv->v, v) == 0)
		{
			printf("(%d,%d)", vv->v->x, vv->v->y);
		}

		VertexNode *vn = vv;
		vn = vn->adjacentVertex;
		// enqueue all connected vertex which is not visited
		while(vn != NULL)
		{
			if(vn->vertexPointer->state == 0)
			{
				enqueue(q, vn->vertexPointer);
				vn->vertexPointer->state = 1;
			}
			vn = vn->adjacentVertex;
		}
		if(isEmpty(q) == 0 && sameVertex(vv->v, v) == 0)
		{
			printf(",");
		}
	}
	// free queue
	free(q->q);
	free(q);
	printf("\n");
}

// time complexity: O(1)
// create and init an empty heap
HeapHead *CreateEmptyHeap()
{
	HeapHead *hh = (HeapHead *) malloc(sizeof(HeapHead));
	HeapNode **h = (HeapNode**) malloc(MAXSIZE * sizeof(HeapNode));
	for(int i = 0; i < MAXSIZE + 1; i++)
	{
		h[i] = NULL;
	}
	hh->h = h;
	hh->last = 1;
	return hh;
}

// time comlexity: O(1)
// swap two node in the heap with index i and index j
void swapNode(HeapHead *hh, int i, int j)
{
	// use inex 0 to swap two nodes
	hh->h[0] = hh->h[i];
	hh->h[i] = hh->h[j];
	hh->h[j] = hh->h[0];
	hh->h[i]->position = i;
	hh->h[j]->position = j;
	hh->h[0] = NULL; 		
}
// time comlexity: O(n)
// insert all verticies into the heap and put the node storing u at the root
void InitHeap(Graph g, HeapHead *hh, Vertex *u)
{
	VertexNode *vn = g->vertices;
	HeapNode **h = hh->h;
	// add all vertex node into heap
	while(vn != NULL)
	{
		HeapNode *hn = (HeapNode *)malloc(sizeof(HeapNode));
		// for u, we set the key to 0
		if(sameVertex(u, vn->v) == 1)
		{
			hn->key = 0;		
		}
		// for other nodes, we set the key to infinity
		else
		{
			hn->key = MYINFINITY;
		}
		hn->v = cloneVertex(vn->v);
		hn->vn = vn;
		hn->pre = NULL;	
		hn->position = hh->last;
		h[hh->last] = hn;
		(hh->last)++;
		vn->heapPointer = hn;
		vn = vn->nextVertex;	
	}
	// swap node to put u at the first place
	int index = 1;
	for(int i = 1; i < hh->last; i++)
	{
		if(hh->h[i]->key == 0)
		{
			index = i;
			break;
		}
	}
	if(hh->last > 1)
	{
		swapNode(hh, 1, index);
	}
}


// time comlexity: O(n)
// free the memory created by heap
void DestroyHeap(Graph g, HeapHead *hh)
{
	for(int i = 1; i < hh->last; i++)
	{
		free(hh->h[i]);
	}
	free(hh->h);
	free(hh);
	VertexNode *vn = g->vertices;
	while(vn != NULL)
	{
		vn->heapPointer = NULL;
		vn = vn->nextVertex;
	}
}

// time complexity: O(logn)
// downHeap from index i
void downHeap(HeapHead *hh, int i)
{
	// for array heap, lchild = 2 * parent and rchild = 2 * parent + 1
	int parent, lchild, rchild;
	if(i > hh->last - 1)
	{
		return;
	}
	parent = i;
	if(hh->h[2 * i] != NULL)
	{
		lchild = 2 * i;
	}
	else
	{
		lchild = -1;
	}
	if(hh->h[2 * i + 1] != NULL)
	{
		rchild = 2 * i + 1;
	}
	else
	{
		rchild = -1;
	}
	if(lchild == -1 && rchild == -1)
	{
		return;
	}
	else if(lchild != -1 && rchild == -1)
	{
		if(hh->h[parent]->key > hh->h[lchild]->key)
		{
			swapNode(hh, parent, lchild);
		}
		return;
	}
	else if(lchild != -1 && rchild != -1)
	{
		if(hh->h[parent]->key <= hh->h[lchild]->key && hh->h[parent]->key <= hh->h[rchild]->key)
		{
			return;
		}
		else
		{
			// swap and put the smaller child node to the parent
			if(hh->h[lchild]->key <= hh->h[rchild]->key)
			{
				swapNode(hh, parent, lchild);
				downHeap(hh, lchild);
			}
			else
			{
				swapNode(hh, parent, rchild);
				downHeap(hh, rchild);
			}
		}
	}
	else
	{
		return;
	}
}

// time comlexity: O(logn)
// return the root and restore the heap-order property
HeapNode *RemoveMin(HeapHead *hh)
{
	if(hh->last == 1)
	{
		return NULL;
	}
	HeapNode *res = hh->h[1];
	(hh->last)--;
	hh->h[1] = hh->h[hh->last];
	hh->h[1]->position = 1;
	hh->h[hh->last] = NULL;
	// restore the heap-order property
	downHeap(hh, 1);
	return res;
}

// time comlexity: O(logn)
// upheap from index i
void upHeap(HeapHead *hh, int i)
{
	int index = i;
	int parent = index / 2;
	while(index > 0 && parent > 0)
	{
		if(hh->h[parent]->key <= hh->h[index]->key)
		{
			break;
		}
		else
		{
			// swap and put the smaller node to the parent
			swapNode(hh, index, parent);
			index = parent;
			parent = index / 2;
		}
	}
}

// time comlexity: O(n)
// print all elements in the heap, for testing only
void PrintHeap(HeapHead *hh)
{
	printf("print heap:\n");
	for(int i = 1; i < hh->last; i++)
	{
		printf("%f,", hh->h[i]->key);
	}
	printf("\n");
}
// time comlexity: O((m+n)logn), where m is the number of edges and n is the number of vertices
// findVertex() takes O(n) time
// CreateEmptyHeap() takes O(1) time
// InitHeap() takes O(n) time
// RemoveMin() takes O(logn) time
// sameVertex() takes O(1) time
// upHeap() takes O(logn) time
// DestroyHeap() takes O(n) time
// for each while loop, we spend O(logn) to removeMin() and O(degree(u)logn) to perform the relaxation procedure
// so the overall running time is O((m + n)logn)
//
// use dijkstra and print the shortest path from u to v
void ShortestPath(Graph g, Vertex *u, Vertex *v)
{
	VertexNode *vn1 = findVertex(g, u);
	VertexNode *vn2 = findVertex(g, v);
	if(vn1 == NULL || vn2 == NULL)
	{
		return;
	}
	// create a heap storing all vertices and set the key of v as 0 and the key of other nodes as inifinity
	HeapHead *hh = CreateEmptyHeap();
	InitHeap(g, hh, v);
	HeapNode *res = NULL;
	while(hh->last != 1)
	{
		HeapNode *tempNode = RemoveMin(hh);
		if(sameVertex(tempNode->v, u))
		{
			res = tempNode;
			break;
		}
		if(tempNode->key > MYINFINITY - 1)
		{
			break;
		}
		VertexNode *tvn = tempNode->vn;
		tvn = tvn->adjacentVertex;
		while(tvn != NULL)
		{
			// Dijkstra algorithm
			// set D[z] = min{D[z], D[u] + w((u,z))}
			if(tempNode->key + tvn->distance < tvn->vertexPointer->heapPointer->key)
			{
				tvn->vertexPointer->heapPointer->key = tempNode->key + tvn->distance;
				tvn->vertexPointer->heapPointer->pre = tempNode;
				// after updating the key of a node in the heap, we need upHeap to restore heap-order property
				upHeap(hh, tvn->vertexPointer->heapPointer->position);
			}
			tvn = tvn->adjacentVertex;
		}
	}
	// print the shortest path
	if(res != NULL && res->key < MYINFINITY)
	{	
		printf("(%d,%d),", u->x, u->y);
		while(res != NULL)
		{
			if(sameVertex(u, res->v) || sameVertex(v, res->v))
			{
				res = res->pre;
				continue;
			}
			else
			{
				printf("(%d,%d),", res->v->x, res->v->y);
				res = res->pre;
			}	
		}
		printf("(%d,%d)\n", v->x, v->y);
	}
	DestroyHeap(g, hh);
}

// time comlexity: O(m + n)
// this function visits each vertex once and each edge twice
// so the running time is O(m + n)
//
// free all memory used in the graph
void FreeGraph(Graph g)
{
	VertexNode *vertices = g->vertices;
	VertexNode *node;
	while(vertices != NULL)
	{
		node = vertices->adjacentVertex;
		while(node != NULL)
		{
			VertexNode *tempNode = node;
			node = node->adjacentVertex;
			free(tempNode->v);
			free(tempNode);
		}
		VertexNode *tempVertices = vertices;
		vertices = vertices->nextVertex;
		free(tempVertices->v);
		free(tempVertices);
	}
	free(g);
}

// time comlexity: O(m + n)
// init states of all nodes for ShowGraph()
void initEdgeState(Graph g)
{
	// init all vertices and edges state to 0
	VertexNode *vn = g->vertices;
	while(vn != NULL)
	{
		vn->state = 0;
		VertexNode *vne = vn->adjacentVertex;
		while(vne != NULL)
		{
			vne->state = 0;
			vne = vne->adjacentVertex;
		}
		vn = vn->nextVertex;
	}
}

// time complexity: O(m + n)
// CreateQueue and enqueue(), dequeue and isEmpty() takes O(1) time
// this function visits each reachable vertex once and each reachable edge twice
// so the running time is O(m + n)
//
// use BFS to print all reachable edges from v
void BFSShowGraphHelper(Graph g, VertexNode *v)
{
	VertexNode *vn1 = v;
	Queue *q = CreateQueue();
	enqueue(q, vn1);
	vn1->state = 1;
	while(isEmpty(q) != 1)
	{
		VertexNode *vv = dequeue(q);		
		VertexNode *vn = vv;
		vn1 = vn->adjacentVertex;
		while(vn1 != NULL)
		{
			// if edge is not visited, print it
			if(vn1->state == 0)
			{
				printf("(%d,%d),(%d,%d) ", vn->v->x, vn->v->y, vn1->v->x, vn1->v->y);
				vn1->state = 1;
				vn1->edgePointer->state = 1;
				// if vertex is not visited, enqueue it
				if(vn1->vertexPointer->state == 0)
				{
					enqueue(q, vn1->vertexPointer);
					vn1->vertexPointer->state = 1;
				}
				
			}
			vn1 = vn1->adjacentVertex;
		}
	}
	free(q->q);
	free(q);
}

// time comlexity: O(m + n)
// initEdgeState() takes O(m + n) time
// BFSShowGraphHelper() helps this function to print all reachable edges from a vertex
// Overall, this function visits each vertex once and each edge twice
// so, the running time is O(m + n)
//
// show all edges in BFS order
void ShowGraph(Graph g)
{
	initEdgeState(g);
	VertexNode *vn = g->vertices;
	// for each unvisited vertex, call BFSShowGraphHelper() to print all information of the graph
	while(vn != NULL )
	{
		if(vn->state == 0)
		{
			BFSShowGraphHelper(g, vn);
		}
		vn = vn->nextVertex;
	}
	printf("\n");
}

int main() //sample main for testing 
{ 
 Graph g1;
 Edge *e_ptr; 
 Vertex *v1, *v2;
  
 // Create an empty graph g1;
 g1=CreateEmptyGraph();
  
 // Create first connected component 
 // Insert edge (0,0)-(0,10)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=0;
 v2->y=10;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (0,0)-(5,6)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=5;
 v2->y=6;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (0, 10)-(10, 10)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=10;
 v2->x=10;
 v2->y=10;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (0,10)-(5,6)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=10;
 v2->x=5;
 v2->y=6;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (0,0)-(5,4)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=5;
 v2->y=4;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (5, 4)-(10, 4)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=5;
 v1->y=4;
 v2->x=10;
 v2->y=4;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (5,6)-(10,6)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=5;
 v1->y=6;
 v2->x=10;
 v2->y=6;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (10,10)-(10,6)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=10;
 v1->y=10;
 v2->x=10;
 v2->y=6;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (10, 6)-(10, 4)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=10;
 v1->y=6;
 v2->x=10;
 v2->y=4;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Create second connected component
 // Insert edge (20,4)-(20,10)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=20;
 v1->y=4;
 v2->x=20;
 v2->y=10;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (20,10)-(30,10)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=20;
 v1->y=10;
 v2->x=30;
 v2->y=10;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (25,5)-(30,10) 	
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=25;
 v1->y=5;
 v2->x=30;
 v2->y=10;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n"); 
 
 //Display graph g1
 ShowGraph(g1);
	
 // Find the shortest path between (0,0) and (10,6) 
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=10;
 v2->y=6;
 ShortestPath(g1, v1, v2);
 free(v1);
 free(v2);
	  
 // Delete edge (0,0)-(5, 6)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=5;
 v2->y=6;
 e_ptr->p1=v1;
 e_ptr->p2=v2; 	 
 DeleteEdge(g1, e_ptr);
 free(e_ptr);
 free(v1);
 free(v2);
 	 
 // Display graph g1
 ShowGraph(g1);
	
 // Find the shortest path between (0,0) and (10,6) 
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=10;
 v2->y=6; 
 ShortestPath(g1, v1, v2);
 free(v1);
 free(v2);
 
 // Find the shortest path between (0,0) and (25,5)
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=25;
 v2->y=5;
 ShortestPath(g1, v1, v2);
 free(v1);
 free(v2);	
 
 // Find reachable vertices of (0,0)
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v1->x=0;
 v1->y=0;
 ReachableVertices(g1, v1);
 free(v1);
 
 // Find reachable vertices of (20,4)
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v1->x=20;
 v1->y=4;
 ReachableVertices(g1, v1);
 free(v1);
 
 // Free graph g1
 FreeGraph(g1);
 
 return 0; 
}
