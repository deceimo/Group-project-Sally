/*******************************************************************
*
*	buildStructure.c
*
*	This C file have couples of functions that create 
*	main data structures of this program which are 
*	network which its key is linked by sorted binary tree
*
*	Created by Nattapong(Milo)	ID: 63070503415
*	16 April 2021
*
********************************************************************/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"structure.h"

#include"buildStructure.h"


/********************************************
*
*	Private function
*
********************************************/

/*
This function is used to store the node into sorted
binary tree by alphabetizing using recursion itself.
So left child will have key name before parent,
and right child will have key name after parent.
ARGUMENT
TREE_NODE_T *currentNode	- Current parent node
TREE_NODE_T *pNewNode		- Node added to the tree
*/
void insertNode(TREE_NODE_T *currentNode,TREE_NODE_T *pNewNode)
{
	if(strcasecmp(pNewNode->key,currentNode->key) < 0)	//If newnode has name
		{												//before pRoot, go left child
		if(currentNode->pLeft==NULL)	//If there is no left child, put it be left child
			{
			currentNode->pLeft=pNewNode;
			}
		else	//If left child has been created, recursive it
			{
			insertNode(currentNode->pLeft,pNewNode);
			}
		}
	else if(strcasecmp(pNewNode->key,currentNode->key) > 0)	//If newnode has name
		{													//after pRoot, go right child
		if(currentNode->pRight==NULL)	//If there is no right child, put it be right child
			{
			currentNode->pRight=pNewNode;
			}
		else	//If right child has been created, recursive it
			{
			insertNode(currentNode->pRight,pNewNode);
			}
		}
}


/*
This function is used to find whether input location name
exists in the sorted binary tree or not by using recursion itself.
It will return address of a node that holds that input location name
if it is found. Otherwise, it will return NULL.
ARGUMENT
char *keyName				- Input location name
TREE_NODE_T *currentNode	- Root node/ Current node
*/
TREE_NODE_T *findNode(char *keyName,TREE_NODE_T *currentNode)
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
			pFound=findNode(keyName,currentNode->pLeft);
			}
		//If alphabet of name user type is after the current node, go right child
		else if(strcasecmp(keyName,currentNode->key)>0)
			{
			pFound=findNode(keyName,currentNode->pRight);
			}
		}
	return pFound;
}




/********************************************
*
*	Public function
*
********************************************/


/*
This function is used to build main data structure of this program
which is a sorted binary tree mixed with network. Every location name
will have its own node although some nodes are an alias to each other.
But the alias will hold the same vertex so that it will be the same place
in network. Each vertex will have edge to some vertices with the distance,
street name ,and direction. It will return address of root node of the binary tree.
NO ARGUMENT
*/
TREE_NODE_T *makeNetwork()
{
	TREE_NODE_T *returnNode=NULL;
	TREE_NODE_T *pNewNode;
	TREE_NODE_T *tempNode;
	TREE_NODE_T *pStartNode;
	TREE_NODE_T *pEndNode;
	VERTEX_T *pNewVertex;
	EDGE_T *pNewEdge;
	FILE *pInput=NULL;
	char input[512];
	char tempKey[128];
	char tempName[MAX_ALIAS][128];
	char start[128];
	char end[128];
	char road[128];
	char direction[8];
	float length;
	int i;
	int count;
	int line=1;
	
	pInput=fopen("location.txt","r");
	
	//If text file does not exist, exit the program
	if(pInput==NULL)
		{
		printf("Error opening text file\n");
		exit(1);
		}
	
	//Otherwise, read data and build data structure
	while(fgets(input,sizeof(input),pInput)!=NULL)
		{
		memset(tempName,0,sizeof(tempName));
		//There will be three forms of text file
		//First, start with 'N', which stand for NODE
		//it will make node in sorted binary tree
		//Second, start with 'V', which stand for VERTEX
		//it will make vertex in network
		//Third, start with 'E', which stand for EDGE
		//it will make edge between two vertices in network
		if(input[0]=='N')
			{
			sscanf(input,"N %[^\n]",tempKey);
			
			//Create new node
			pNewNode=calloc(sizeof(TREE_NODE_T),1);
			
			//If allocating memory is not successful
			//Exit the program
			if(pNewNode==NULL)
				{
				printf("Error allocating memory\n");
				exit(2);
				}
			
			strcpy(pNewNode->key,tempKey);
			
			//If there is no root node
			//Set first node to be a root node
			if(returnNode==NULL)
				{
				returnNode=pNewNode;
				}
			else//Otherwise, put it into left/right child
				{
				insertNode(returnNode,pNewNode);
				}
			line++;
			}
		else if(input[0]=='V')
			{
			sscanf(input,"V %d",&count);
			
			//Loop to get name which is alias
			for(i=0;i<count;i++)
				{
				fgets(input,sizeof(input),pInput);
				sscanf(input,"%[^\n]",tempName[i]);
				}
			
			//Create new vertex
			pNewVertex=calloc(sizeof(VERTEX_T),1);
			
			//If allocating memory is not successful
			//Exit the program			
			if(pNewVertex==NULL)
				{
				printf("Error allocation memory\n");
				exit(3);
				}
			
			//Put the number of alias and their name
			//into the vertex
			(pNewVertex->aliasCount)=count;
			for(i=0;i<count;i++)
				{
				strcpy(pNewVertex->aliasName[i],tempName[i]);
				}
			
			//Find node of these alias
			//and put new vertex into the node
			for(i=0;i<count;i++)
				{
				tempNode=findNode(tempName[i],returnNode);
				//If node is not found
				//there must be an error in text file
				if(tempNode==NULL)
					{
					printf("Text format starting with 'V' errors in line %d\n",line);
					}
				else
					{
					(tempNode->location)=pNewVertex;
					}
				}
			line++;
			}
		else if(input[0]=='E')
			{
			sscanf(input,"E %[^|]|%[^|]|%f|%[^|]|%[^|]",
			start,end,&length,road,direction);
			
			//Find node of start and end vertex
			pStartNode=findNode(start,returnNode);
			pEndNode=findNode(end,returnNode);
			
			//If node is not found
			//there must be an error in text file
			if((pStartNode==NULL)||(pEndNode==NULL))
				{
				printf("Text format starting with 'E' errors in line %d\n",line);
				line++;
				continue;
				}
			
			//Create new edge
			pNewEdge=calloc(sizeof(EDGE_T),1);
			
			//Put end vertex, distance, street name,
			//and direction into new edge
			(pNewEdge->pVertex)=pEndNode->location;
			(pNewEdge->distance)=length;
			strcpy(pNewEdge->streetName,road);
			strcpy(pNewEdge->direction,direction);
			
			//Put new edge into start vertex
			//by using linked list
			if((pStartNode->location->adjacentHead)==NULL)
				{
				(pStartNode->location->adjacentHead)=pNewEdge;
				(pStartNode->location->adjacentTail)=pNewEdge;
				}
			else
				{
				(pStartNode->location->adjacentTail->next)=pNewEdge;
				(pStartNode->location->adjacentTail)=pNewEdge;
				}
			line++;
			}
		else//If it is not start with N,V,E , text file must be error
			{
			printf("Bad format of text file in line %d\n",line);
			line++;
			}
		}
		
	fclose(pInput);
	return returnNode;
}


/*
This function will free data structure of binary tree
and network
ARGUMENT
TREE_NODE_T *currentNode	- Root node
*/
void freeNetwork(TREE_NODE_T *currentNode)
{
	VERTEX_T *currentVertex;
	EDGE_T *currentEdge;
	EDGE_T *deleteEdge;
	if(currentNode!=NULL)
		{
		//If there is left child, go to left first
		if((currentNode->pLeft)!=NULL)
			{
			freeNetwork(currentNode->pLeft);
			}
		
		//If there is right child, go to right second
		if((currentNode->pRight)!=NULL)
			{
			freeNetwork(currentNode->pRight);
			}
		
		currentVertex = currentNode->location;
		currentEdge = currentVertex->adjacentHead;

		
		if((currentVertex->aliasCount) == 1)
			{
			while(currentEdge != NULL)
				{
				deleteEdge = currentEdge;
				currentEdge = currentEdge->next;
				free(deleteEdge);
				}
			free(currentVertex);
			}
		else
			{
			(currentVertex->aliasCount) = ((currentVertex->aliasCount)-1);
			}
		
		free(currentNode);
		}
}
