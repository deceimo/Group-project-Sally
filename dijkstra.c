/*************************************************************************
*
*	dijkstra.c
*
*	This C file is used to find the shortest in the network in the project.
*	Using Dijkstra's algorithm to be the main algorithm for finding it.
*	
*	created by 	Seiya Tajima 		ID: 63070503409
*				Preutta Woratol 	ID: 63070503433
*				Mayson Ross Kieng 	ID: 63070503445
*
*	14 April 2021
*
**************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"structure.h"
#include"minPriorityQueue.h" //From Demo code of lab10 created by Dr. Sally, used without modification//

#include"dijkstra.h"

#define HUGEVALUE 99999
#define WHITE 0
#define BLACK 1

typedef struct _arrange
	{
	VERTEX_T * pCurrent;
	struct _arrange * pNext;
	}ARRANGE_T;




/************************************
*
*	Private function
*
*************************************/

/*
 *	This function change format of the word
 *	first letter should be capital 
 *	the other letters should be lowercase
 *
 */
char* wordTransformer(char key[])
{
	int i = 0;
	if (strcasecmp(key,"kmutt") == 0)
		{
		strcpy(key,"KMUTT");
		return key;
		}
	for (i=0; i<strlen(key); i++)
		{
		if (i == 0)
			{
			key[i] = toupper(key[i]);
			}
		else if (key[i] == ' ')
			{
			key[i+1] = toupper(key[i+1]);
			i++;
			}
		else
			{
			key[i] = tolower(key[i]);
			}
		}
	return key;
}

/*
 * 	This function received a keyname and will find the name in the tree.
 *	It will return NULL if the keyName does not exist in the tree.
 *	Otherwise, it will return the address of that node.
 *	argument:
 * 	'keyName' is a input from user
 * 	'CurrentNode' will a address of each node
 */
TREE_NODE_T *find(char *keyName,TREE_NODE_T *currentNode)
{
	TREE_NODE_T *pFound=NULL;

	if(currentNode!=NULL)
		{
		//If it is found, set pFound to be the exist name address
		if(strcasecmp(keyName,currentNode->key)==0)
			{
			pFound=currentNode;
			}
		//If alphabet of name user type is before the current node, go left child
		else if(strcasecmp(keyName,currentNode->key)<0)
			{
			pFound=find(keyName,currentNode->pLeft);
			}
		//If alphabet of name user type is after the current node, go right child
		else if(strcasecmp(keyName,currentNode->key)>0)
			{
			pFound=find(keyName,currentNode->pRight);
			}
		}
	return pFound;
}

/*
 * This function will change all nodes's color to white
 * and also change all the nodes value to be a hugevalue
 * and will count how many nodes are there by counting when changing color to white
 * and also set pFrom to be null
 * 'CurrentNode' will a address of each node
 */
int clearVertices(TREE_NODE_T *currentNode)
{
	int count1 = 0;
	int count2 = 0;
	VERTEX_T *currentVertex=NULL;
	if(currentNode!=NULL)
		{
		if (currentNode->pLeft != NULL)
			{
			count1 = clearVertices(currentNode->pLeft);
			count1++;
			}
		if (currentNode->pRight != NULL)
			{
			count2 = clearVertices(currentNode->pRight);
			count2++;
			}
		currentVertex=currentNode->location;
		currentVertex->color = WHITE;
		currentVertex->dValue = HUGEVALUE;
		currentVertex->pFrom = NULL;
		return count1+count2; //Doesn't count pRoot
		}
}

/*
 * This function will compare the dValue of 2 Vertex
 * return 1 when dValue of first vertex is greater than the second vertex
 * return -1 when dValue of second vertex is greater than the first vertex
 * return 0 when it doesn't go into both case
 * 'v1' is the first vertex
 * 'v2' is the second vertex
 *  Based on code by Sally Goldin from [simpleNetwork.c]
 *  Used with permission.
 */
int vertexCompare(void* V1, void* V2)
{
	VERTEX_T * vertex1 = (VERTEX_T*) V1;
	VERTEX_T * vertex2 = (VERTEX_T*) V2;
	
	if (vertex1->dValue > vertex2->dValue)
		return 1;
	else if (vertex1->dValue < vertex2->dValue)
		return -1;
	else
		return 0;
}

/*
 * This function will enqueue node and will use enqueue min priority
 * 'CurrentNode' will a address of each node
 */
void enqueueNode(TREE_NODE_T *currentNode)
{
	VERTEX_T *currentVertex=NULL;
	if(currentNode!=NULL)
		{
		if (currentNode->pLeft != NULL)
			{
			enqueueNode(currentNode->pLeft);
			}
		if (currentNode->pRight != NULL)
			{
			enqueueNode(currentNode->pRight);
			}
		currentVertex=currentNode->location;
		enqueueMin(currentVertex);
		}
}

/* This function is use for free data
 */
void freeLink(ARRANGE_T * pKeep)
{
	ARRANGE_T * pDelete;
	
	while(pKeep != NULL)
		{
		pDelete = pKeep;
		pKeep = pKeep->pNext;
		free(pDelete);
		}
}


/*
 * This function will print the location path and the shortest path
 * 'startNode' is the start location
 * 'endNode' designated location
 */
void printPath(TREE_NODE_T *startNode, TREE_NODE_T *endNode,char key1[],char key2[])
{
	VERTEX_T * endVertex = NULL;
	VERTEX_T * currentVertex = NULL;
	ARRANGE_T * pHead = NULL;
	ARRANGE_T * pKeep;
	ARRANGE_T * pDummy = NULL;
	ARRANGE_T * pCalloc = NULL;
	EDGE_T * currentEdge = NULL;
	char direction[10]; 
	int i = 0;
	int count = 0;

	endVertex = endNode->location;
	currentVertex = endNode->location;
	printf("*******************************************\n");

	if((startNode->location) == (endNode->location))
		{
		printf("The starting location and the destination are the same place\n");
		}
	else if (endVertex->pFrom == NULL)
		{
		printf("location: '%s' is not reachable from location '%s'\n",endNode->key,startNode->key);
		}
	else
		{
		printf("Min total distance of shortest path: %.2f km\n",endVertex->dValue);
		while (1)
			{
			if (currentVertex->pFrom == NULL)
				{
				pCalloc = calloc(sizeof(ARRANGE_T),1);
				if (pCalloc == NULL)
					{
					printf("Calloc error!!\n");
					exit(0);
					}
				pHead = pCalloc;
				pHead->pCurrent = currentVertex;
				pHead->pNext = pDummy;
				break;
				}

			pCalloc = calloc(sizeof(ARRANGE_T),1);
			if (pCalloc == NULL)
				{
				printf("Calloc error!!\n");
				exit(0);
				}
			pHead = pCalloc;
			pHead->pCurrent = currentVertex;
			pHead->pNext = pDummy;
			pDummy = pHead;
			currentVertex = currentVertex->pFrom;
			}
		
		pKeep=pHead;
  		printf("Path:\n");
		while(1)
			{
			if (pHead->pNext == NULL) //check if the between two direction have only one edge
				{
				printf(" =>Arrive at the %s\n",key2);
				break;
				}
			currentEdge=pHead->pCurrent->adjacentHead;
			while(1)
				{
				if(currentEdge->pVertex == pHead->pNext->pCurrent)
					{
					break;
					}
				currentEdge=currentEdge->next;
				}
			if(currentEdge->direction[0] == 'N')  
				{
				strcpy(direction,"North");
				}
			else if(currentEdge->direction[0] == 'E')  
				{
				strcpy(direction,"East");
				}
			else if(currentEdge->direction[0] == 'S') 
				{
				strcpy(direction,"South");
				}
			else if(currentEdge->direction[0] == 'W')
				{
				strcpy(direction,"West");
				}
			if ((count == 0) && (pHead->pNext->pNext ==NULL))
				{
				printf("From %s go %s on %s for %.2f km to %s\n",key1,direction,currentEdge->streetName,currentEdge->distance,key2);
				count++;
				}
			else if (count == 0)
				{
				printf("From %s go %s on %s for %.2f km to %s\n",key1,direction,currentEdge->streetName,currentEdge->distance,pHead->pNext->pCurrent->aliasName[0]);
				count++;
				}
			else if (pHead->pNext->pNext == NULL)
				{
				printf("From %s go %s on %s for %.2f km to %s\n",pHead->pCurrent->aliasName[0],direction,currentEdge->streetName,currentEdge->distance,key2);
				count++;
				}
			else
				{
				printf("From %s go %s on %s for %.2f km to %s\n",pHead->pCurrent->aliasName[0],direction,currentEdge->streetName,currentEdge->distance,pHead->pNext->pCurrent->aliasName[0]);
				count++;
				}
			pHead = pHead->pNext;
			}
		freeLink(pKeep);
		}
}


/*
 * This function will find shortest path for designated location
 * 'startNode' is the start location
 * 'endNode' designated location
 * 'pRoot' is first node of the tree
 */
void findShortestPath(TREE_NODE_T *startNode, TREE_NODE_T *endNode,TREE_NODE_T *pRoot,char key1[],char key2[])
{
	int count = 0; //amount of node
	int i = 0;
	TREE_NODE_T *currentNode = NULL;
	VERTEX_T * currentVertex = NULL;
	EDGE_T * currentEdge = NULL;
	VERTEX_T * adjacentVertex = NULL;
	VERTEX_T * startVertex = NULL;

	startVertex = startNode->location;
	queueMinInit(&vertexCompare);
	count = clearVertices(pRoot) +1; //count all the node in the tree including pRoot
	startVertex->dValue = 0;
	currentVertex = startNode->location;
	enqueueNode(pRoot);

	while (queueMinSize() > 0)
		{
		currentVertex = dequeueMin();
		if (currentVertex->color == WHITE)
			{
			currentVertex->color = BLACK;
			currentEdge = currentVertex->adjacentHead;
			while(1)
				{
				if(currentEdge==NULL)
					{
					break;
					}
				adjacentVertex = currentEdge->pVertex;
				if (currentVertex->dValue + currentEdge->distance < adjacentVertex->dValue)
					{
					adjacentVertex->dValue = currentVertex->dValue + currentEdge->distance;
					adjacentVertex->pFrom = currentVertex;
					}
				currentEdge=currentEdge->next;
				}
			}
		}
	printPath(startNode,endNode,key1,key2);
}


/*******************************
*
*	Public function
*
********************************/


/* 
 * This function will ask for current location of the user
 * and also ask for designated location of the user
 * and after that will process to find if the location ecxist or not
*/
void askNavigation(TREE_NODE_T *pRoot)
{
	char key1[64]; //starting point
	char key2[64]; // destination point
	char name1[64];
	char name2[64];
	char input[64];
	TREE_NODE_T *startNode=NULL;
	TREE_NODE_T *endNode=NULL;

	while (1)
		{
		memset(key1,0,sizeof(key1));
		printf("*******************************************\n");
		printf("Enter key for starting point (ENTER to stop): ");
		fgets(input,sizeof(input),stdin);
		sscanf(input,"%[^\n]",key1);
		if (strlen(key1) == 0)
			{
			printf("return to main menu\n\n");
			break;
			}
		startNode = find(key1,pRoot); //find key1
		
		if (startNode != NULL)
			{
			printf("Enter key for destination point: ");
			fgets(input,sizeof(input),stdin);
			sscanf(input,"%[^\n]",key2);
			endNode = find(key2,pRoot);//find key2
			if (endNode != NULL)
				{
				strcpy(name1,wordTransformer(key1));
				strcpy(name2,wordTransformer(key2));
				findShortestPath(startNode,endNode,pRoot,name1,name2);
				}
			else
				{
				printf("location : %s does not exist!\n",key2);
				}
			}
		else
			{
			printf("location : %s does not exist!\n",key1);
			}
		}
}
