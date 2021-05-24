/********************************************
*
*	structure.h
*
*	This H file defines the structure used
*	in the navigation system project
*
*	Created by Nattapong(Milo)	ID: 63070503415
*	19 May 2021
*
*
********************************************/



#define MAX_ALIAS 10

typedef struct _edge
	{
	void *pVertex;			//Point to adjacent vertex
	struct _edge *next;		//Next edge of that location
	float distance;
	char streetName[64];
	char direction[2];
	}EDGE_T;
	
	
typedef struct _vertex
	{
	char aliasName[MAX_ALIAS][128];
	int color;				//For dijkstra algorithm
	int aliasCount;			//At least 1 alias which is itsself
	float dValue;			//For dijkstra algorithm
	struct _vertex *pFrom;	//For display shortest path
	EDGE_T *adjacentHead;	//Head edge
	EDGE_T *adjacentTail;	//Tail edge
	}VERTEX_T;
	

typedef struct _treeNode
	{
	char key[128];			//Location name
	VERTEX_T *location;		//Point to vertex of that location in network
	struct _treeNode *pLeft;	//Left child
	struct _treeNode *pRight;	//Right child
	}TREE_NODE_T;
