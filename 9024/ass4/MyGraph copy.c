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
	Vertex **q;
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
	struct VertexNode *nextVertex;
	struct VertexNode *preVertex;
	struct VertexNode *adjacentVertex;
	struct VertexNode *preAdjacentVertex;
	struct HeapNode *heapPointer;
	struct VertexNode *vertexPointer;
	struct VertexNode *edgePointer;
} VertexNode;

typedef struct HeapNode {
	int position;
	Vertex *v;
	float key;
	VertexNode *vn;
	struct HeapNode *pre;
} HeapNode;

typedef struct HeapHead {
	int last;
	HeapNode **h;
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

// Add the time complexity analysis of CreateEmptyGraph() here
Graph CreateEmptyGraph()
{
	Graph g = (Graph) malloc(sizeof(GraphRep));
	assert(g != NULL);
	g->nV = 0;
	g->nE = 0;
	g->vertices = NULL;
	return g;
}

// O(n)
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
// O(n)
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
// O(1)
Vertex *cloneVertex(Vertex *v)
{
	Vertex *res = (Vertex *) malloc(sizeof(Vertex));
	res->x = v->x;
	res->y = v->y;
	return res;
}
// O(n)
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
//O(m)
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

// O(1)
float getDistance(VertexNode *vn, Vertex *v)
{
	float res = sqrt(pow((vn->v->x - v->x), 2) + pow((vn->v->y - v->y), 2));
	return res;
}
// O(m)
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
// O(m + n)
// Add the time complexity analysis of InsertEdge() here
int InsertEdge(Graph g, Edge *e)
{
	Vertex *v1 = e->p1;
	Vertex *v2 = e->p2;
	VertexNode *vn1 = findVertex(g, v1);
	if(vn1 == NULL)
	{	
		vn1 = appendVertex(g, v1);
	}
	else
	{
		VertexNode *v3 = findEdge(vn1, v2);
		if(v3 != NULL)
		{
			return 0;
		}	
	}
	VertexNode *vn2 = findVertex(g, v2);
	if(vn2 == NULL)
	{	
		vn2 = appendVertex(g, v2);
	}
	VertexNode *v5 = appendEdge(vn1, v2);
	VertexNode *v6 = appendEdge(vn2, v1);
	v5->vertexPointer = vn2;
	v6->vertexPointer = vn1;
	v5->edgePointer = v6;
	v6->edgePointer = v5;
	return 1;
}

// O(n)
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

// O(m)
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
		// delete vertex
	}
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

// O(m + n)
// Add the time complexity analysis of DeleteEdge() here
void DeleteEdge(Graph g, Edge *e)
{
	VertexNode *vn1 = findVertex(g, e->p1);
	VertexNode *vn2 = findVertex(g, e->p2);

	if(vn1 == NULL || vn2 == NULL)
	{
		return;
	}
	DeleteEdgeFromList(vn1, e->p2);
	DeleteEdgeFromList(vn2, e->p1);
	if(vn1->adjacentVertex == NULL)
	{
		DeleteVertexFromList(g, vn1);
	}
	if(vn2->adjacentVertex == NULL)
	{
		DeleteVertexFromList(g, vn2);
	}
}

// O(1)
Queue *CreateQueue()
{
	Queue *q = (Queue *) malloc(sizeof(Queue));
	q->front = 0;
	q->last = 0;
	q->q = malloc(MAXSIZE * sizeof(Vertex *));
	return q;
}

// O(1)
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

// O(1)
void enqueue(Queue *q, Vertex *v)
{
	Vertex *v1 = cloneVertex(v);
	(q->q)[q->last] = v1;
	(q->last)++;
}
// O(1)
Vertex *dequeue(Queue *q)
{
	if(q->last == q->front)
	{
		return NULL;
	}
	Vertex *res = (q->q)[q->front];
	(q->front)++;
	return res;
}

// O(m + n)
void initVertexState(Graph g)
{
	VertexNode *vn = g->vertices;
	while(vn != NULL)
	{
		vn->state = 0;
		vn = vn->nextVertex;
	}
}

// O(m + n)
// Add the time complexity analysis of ReachableVertices() here
void ReachableVertices(Graph g, Vertex *v)
{
	printf("start reachable vertices\n");
	VertexNode *vn1 = findVertex(g, v);
	if(vn1 == NULL)
	{
		return;
	}
	initVertexState(g);
	Queue *q = CreateQueue();
	enqueue(q, vn1->v);
	vn1->state = 1;
	while(isEmpty(q) != 1)
	{
		Vertex *vv = dequeue(q);
		if(sameVertex(vv, v) == 0)
		{
			printf("(%d,%d)", vv->x, vv->y);
		}

		// O(n)
		VertexNode *vn = findVertex(g, vv);
		vn = vn->adjacentVertex;
		// O(m)
		while(vn != NULL)
		{
			if(vn->vertexPointer->state == 0)
			{
				enqueue(q, vn->vertexPointer->v);
				vn->vertexPointer->state = 1;
			}
			vn = vn->adjacentVertex;
		}
		if(isEmpty(q) == 0 && sameVertex(vv, v) == 0)
		{
			printf(",");
		}
	}
	free(q->q);
	free(q);
	printf("\n");
}

// O(1)
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

// O(1)
void swapNode(HeapHead *hh, int i, int j)
{
	hh->h[0] = hh->h[i];
	hh->h[i] = hh->h[j];
	hh->h[j] = hh->h[0];
	hh->h[i]->position = i;
	hh->h[j]->position = j;
	hh->h[0] = NULL; 		
}
// O(n)
void InitHeap(Graph g, HeapHead *hh, Vertex *u)
{
	VertexNode *vn = g->vertices;
	HeapNode **h = hh->h;

	while(vn != NULL)
	{
		HeapNode *hn = (HeapNode *)malloc(sizeof(HeapNode));
		if(sameVertex(u, vn->v) == 1)
		{
			hn->key = 0;		
		}
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


// O(n)
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

// O(logn)
void downHeap(HeapHead *hh, int i)
{
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
		printf("wrong !!\n");
	}
}

// O(logn)
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
	downHeap(hh, 1);
	return res;

}
// O(logn)
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
			swapNode(hh, index, parent);
			index = parent;
			parent = index / 2;
		}


	}
}

// O(n)
void PrintHeap(HeapHead *hh)
{
	printf("print heap:\n");
	for(int i = 1; i < hh->last; i++)
	{
		printf("%f,", hh->h[i]->key);
	}
	printf("\n");
}
// Add the time complexity analysis of ShortestPath() here
// O((m+n)logn) m-edge n-vertex
void ShortestPath(Graph g, Vertex *u, Vertex *v)
{
	printf("shortest path start\n");
	VertexNode *vn1 = findVertex(g, u);
	VertexNode *vn2 = findVertex(g, v);
	if(vn1 == NULL || vn2 == NULL)
	{
		return;
	}
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
			if(tempNode->key + tvn->distance < tvn->vertexPointer->heapPointer->key)
			{
				tvn->vertexPointer->heapPointer->key = tempNode->key + tvn->distance;
				tvn->vertexPointer->heapPointer->pre = tempNode;
				upHeap(hh, tvn->vertexPointer->heapPointer->position);
			}
			tvn = tvn->adjacentVertex;
		}
	}
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
	printf("shortest path finish\n");
}

// Add the time complexity analysis of FreeGraph() here
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



// O(m + n)
void initEdgeState(Graph g)
{
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

// O(m + n)
void BFSShowGraphHelper(Graph g, VertexNode *v)
{
	VertexNode *vn1 = v;
	Queue *q = CreateQueue();
	enqueue(q, vn1->v);
	vn1->state = 1;
	while(isEmpty(q) != 1)
	{
		Vertex *vv = dequeue(q);
		
		VertexNode *vn = findVertex(g, vv);
		vn1 = vn->adjacentVertex;
		while(vn1 != NULL)
		{
			if(vn1->state == 0)
			{
				printf("(%d,%d),(%d,%d) ", vn->v->x, vn->v->y, vn1->v->x, vn1->v->y);
				vn1->state = 1;
				vn1->edgePointer->state = 1;
				if(vn1->vertexPointer->state == 0)
				{
					enqueue(q, vn1->vertexPointer->v);
					vn1->vertexPointer->state = 1;
				}
				
			}
			vn1 = vn1->adjacentVertex;
		}
	}
	free(q->q);
	free(q);
}
// Add the time complexity analysis of ShowGraph() here
void ShowGraph(Graph g)
{
	printf("start show graph\n");
	initEdgeState(g);
	VertexNode *vn = g->vertices;
	while(vn != NULL )
	{
		if(vn->state == 0)
		{
			BFSShowGraphHelper(g, vn);
		}
		vn = vn->nextVertex;
	}
	
	printf("\n");
	printf("finish show graph\n");
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
 v1->x = 0;
 v1->y = 10;
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
 v1->x=5;
 v1->y=6;
 v2->x=0;
 v2->y=0; 
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
