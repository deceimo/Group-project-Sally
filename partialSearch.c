/*************************************************************************
*
*	partialSearch.c
*
*   This C file contains functions that will find whether name that user
*   type exists in the network or not. Both in partial name
*   and in full name.
*
*	created by 	Preutta Woratol     ID: 63070503433
*				Mayson Ross Kieng   ID: 63070503445
*
*		Date 21 April 2021
*
*
**************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"structure.h"

#include"partialSearch.h"

/************************************
*
*	Private function
*
*************************************/

/*
 * This function will find the full name of the places first before doing
 * the partial search in another function.
 * 'keyName' is a input from user
 * 'CurrentNode' will a address of each node
*/
TREE_NODE_T *findFullName(char *keyName,TREE_NODE_T *currentNode)
{
	TREE_NODE_T *pFound = NULL;

	if(currentNode!=NULL)
		{
		//If it is found, set pFound to be the exist name address
		if(strcasecmp(keyName,currentNode->key)==0)
			{
			pFound = currentNode;
			}
		//If alphabet of name user type is before the current node, go left child
		else if(strcasecmp(keyName,currentNode->key)<0)
			{
			pFound = findFullName(keyName,currentNode->pLeft);
			}
		//If alphabet of name user type is after the current node, go right child
		else if(strcasecmp(keyName,currentNode->key)>0)
			{
			pFound = findFullName(keyName,currentNode->pRight);
			}
		}
	return pFound;
}

/* After doing the full name search an if it's doesn't found we will do the partial search
 * 'keyName' is a input from user*
 * 'CurrentNode' will a address of each node
*/
TREE_NODE_T *partialSearch(char *keyName,TREE_NODE_T *currentNode)
{
	TREE_NODE_T *pFound = NULL;
	int i = 0;
	int count = 0;
	if(currentNode != NULL)
		{
		//If it is found, set pFound to be the exist name address
		if(strncasecmp(keyName,currentNode->key,strlen(keyName)) == 0)
			{
			printf("%s\n",currentNode->key);
			pFound = partialSearch(keyName,currentNode->pLeft);
			pFound = partialSearch(keyName,currentNode->pRight);
			pFound = currentNode;
			}
		//If alphabet of name user type is before the current node, go left child
		else if(strncasecmp(keyName,currentNode->key,strlen(keyName)) < 0)
			{
			pFound = partialSearch(keyName,currentNode->pLeft);
			}
		//If alphabet of name user type is after the current node, go right child
		else if(strncasecmp(keyName,currentNode->key,strlen(keyName)) > 0)
			{
			pFound = partialSearch(keyName,currentNode->pRight);
			}
		}
	return pFound;
}

/*******************************
*
*	Public function
*
********************************/

/*
 * This function will ask for user which location they want to search
 * If the user type in location name it will find all of the name that 
 * have partial same character, also it will print the alias name of 
 * the locaton.
*/
void searchLocation(TREE_NODE_T *pRoot)
{
	char location[64]; //location name for searching//
	char input[64]; //received input from user//
	TREE_NODE_T *fullName = NULL;
	TREE_NODE_T *partialName = NULL;
	int i = 0;

	while (1)
		{
		memset(location,0,sizeof(location));
		printf("*******************************************\n");
		printf("Enter location (ENTER to stop): ");
		fgets(input,sizeof(input),stdin);
		sscanf(input,"%[^\n]",location);
		if (strlen(location) == 0)
			{
			printf("return to main menu\n\n");
			break;
			}
		fullName = findFullName(location,pRoot);
		if(fullName == NULL)
			{
			partialName = partialSearch(location,pRoot);
            
			if(partialName == NULL)
				{
				printf("Partial Search not found\n");
				}
			}
		else
			{
			printf("Location : %s exist!\n",fullName->key);
			if(fullName->location->aliasCount != 1)
				{
				printf("Alias name of %s\n",fullName->key);
				for(i=0;i<fullName->location->aliasCount;i++)
					{
					if(strcasecmp(fullName->key,fullName->location->aliasName[i]) != 0)
						printf("- %s\n",fullName->location->aliasName[i]);
					}
				}
			}
		}
}
