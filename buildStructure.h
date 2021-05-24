/*******************************************************************
*
*	buildStructure.h
*
*	This h file defines the functions for creating network that links
*   by using sorted binary tree, and freeing the network
*
*	Created by Nattapong(Milo)	ID: 63070503415
*	16 April 2021
*
********************************************************************/




/*
This function is used to build main data structure of this program
which is a sorted binary tree mixed with network. Every location name
will have its own node although some nodes are an alias to each other.
But the alias will hold the same vertex so that it will be the same place
in network. Each vertex will have edge to some vertices with the distance,
street name ,and direction. It will return address of root node of the binary tree.
NO ARGUMENT
*/
TREE_NODE_T *makeNetwork();



/*
This function will free data structure of binary tree
and network
ARGUMENT
TREE_NODE_T *currentNode	- Root node
*/
void freeNetwork(TREE_NODE_T *currentNode);
