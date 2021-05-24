/*************************************************************************
*
*	menu.c
*
*	This C file is the part showing the main menu to the user.
*	It will call function thet builds the structure of the data about
*	network mixed with tree, and let user type to what they want to do
*	in the main menu.
*
*	Created by Nattapong(Milo)	ID: 63070503415
*
*	18 April 2021
*
**************************************************************************/

#include<stdio.h>
#include<string.h>
#include"structure.h"
#include"buildStructure.h"
#include"dijkstra.h"
#include"partialSearch.h"


void interface();

int main()
{
	int choice;
	int number = 0;
	char input[64];
	char name[64];
	TREE_NODE_T *pRoot=NULL;
	
	pRoot=makeNetwork();
	
	while(1)
		{
		choice=0;
		interface();
		fgets(input,sizeof(input),stdin);
		sscanf(input,"%d",&choice);
		
		if(choice==1)
			{
			searchLocation(pRoot);
			}
		else if(choice==2)
			{
			askNavigation(pRoot);
			}
		else if(choice==3)
			{
			break;
			}
		else
			{
			printf("Please insert only number 1-3\n\n\n");
			}
		}
	freeNetwork(pRoot);
}




void interface()
{
	printf("\t\t******************************************************\n");
	printf("\t\t*                                                    *\n");
	printf("\t\t*              Navigation system program             *\n");
	printf("\t\t*                                                    *\n");
	printf("\t\t*            1 - Search for location                 *\n");
	printf("\t\t*            2 - Navigation direction                *\n");
	printf("\t\t*            3 - Exit the program                    *\n");
	printf("\t\t*                                                    *\n");
	printf("\t\t******************************************************\n\n");
	printf("What do you want to do? ");
}
