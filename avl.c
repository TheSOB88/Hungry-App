#include "avl.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>


LLNode *llNew( int line ) {
	LLNode *out = malloc( sizeof( LLNode ) );
	out->next = NULL;
	out->line = line;
}

void llTraverse( LLNode *node ) {
	if( node == NULL) {
		return;
	}
	printf( "%d ", node->line );
	llTraverse( node->next );
}

//-----------------------------------

//The corresponding node for the word in the
//last call to avlFindAdd
AvlNode *avlFindAddReturn;
//Whether a node was added in the last call
//to avlFindAdd - used for balance
int avlFindAddNodeAdded;

//Finds the right spot for the word in question
//and adds a node to the tree if necessary.
//Returns the new tree from this node down.
//Also sets the global avlFindAddReturn to
//the word's corresponding node.
AvlNode *avlFindAdd( AvlNode *from, char *newData ) {
	//If the current node is null, a new node
	//must be created and returned
	if( from == NULL ) {
		AvlNode *out = avlNew( newData );
		avlFindAddReturn = out;
		avlFindAddNodeAdded = 1;
		return out;
	}

	//Compare the word we're looking for to the word
	//at this node
	int comp;
	AvlNode *toAdd;

	comp = strcmp( newData, from->data );

	//Update the left or right subtree to ensure
	//it includes a node for this word. If the word
	//is in this node, set bstFindAddReturn to this node.
	if( comp < 0 ) {
		from->left = avlFindAdd( from->left, newData );
	} else if( comp > 0 ) {
		from->right = avlFindAdd( from->right, newData );
	} else {
		avlFindAddReturn = from;
	}

	//If a node was added, some things need to be done.
	if( avlFindAddNodeAdded ) {
		//Update the height
		avlSetHeight( from );

		int balance = avlBalance( from );

		//If the balance factor at this node is
		//2 or -2, a rebalance must be done to
		//conform to AVL.
		if( balance == -2 ) {
			AvlNode *node2, *node3;
			if( avlBalance( from->left ) == -1 ) {
				//left-left
				node2 = from->left;
				from->left = node2->right;
				node2->right = from;
				avlSetHeight( from );
				avlSetHeight( node2 );
				return node2;
			} else {
				//left-right
				node2 = from->left;
				node3 = node2->right;
				from->left = node3->right;
				node2->right = node3->left;
				node3->left = node2;
				node3->right = from;
				avlSetHeight( from );
				avlSetHeight( node2 );
				avlSetHeight( node3 );
				return node3;
			}
		} else if( balance == 2 ) {
			AvlNode *node2, *node3;
			if( avlBalance( from->right ) == 1 ) {
				//right-right
				node2 = from->right;
				from->right = node2->left;
				node2->left = from;
				avlSetHeight( from );
				avlSetHeight( node2 );
				return node2;
			} else {
				//right-left
				node2 = from->right;
				node3 = node2->left;
				from->right = node3->left;
				node2->left = node3->right;
				node3->right = node2;
				node3->left = from;
				avlSetHeight( from );
				avlSetHeight( node2 );
				avlSetHeight( node3 );
				return node3;
			}
		}
	}
	//if no balances occured, this node
	//remains the root of this subtree
	return from;
}

//Computes the height for the subtree starting
//at an AVL node and stores it
void avlSetHeight( AvlNode *node ) {
	if( node->left == NULL && node->right == NULL ) {
		node->height = 1;
	} else if( node->left == NULL ) {
		node->height =  node->right->height + 1;
	} else if( node->right == NULL ) {
		node->height =  node->left->height + 1;
	} else {
		node->height = ( node->left->height > node->right->height ?
			node->left->height : node->right->height ) + 1;
	}
}

//Computes the balance factor of the subtree
//starting at this node
int avlBalance( AvlNode *node ) {
	if( node->left == NULL && node->right == NULL ) {
		return 0;
	} else if( node->left == NULL ) {
		return node->right->height;
	} else if( node->right == NULL ) {
		return 0 - node->left->height;
	} else {
		return node->right->height - node->left->height;
	}
}

//Constructor for a node for the given word
AvlNode *avlNew( char *newData ) {
	AvlNode *out = malloc( sizeof( AvlNode ) );
	out->left = NULL;
	out->right = NULL;
	out->head = NULL;
	out->tail = NULL;
	out->height = 1;

	out->data = malloc( strlen( newData ) + 1 );
	strcpy( out->data, newData );
	return out;
}

//Prints out the data in the tree, one line
//for each word and the lines it appears on
void avlTraverse( AvlNode *node ) {
	//base case - return if null
	if( node == NULL ) {
		return;
	}
	//Recurse on left subtree
	avlTraverse( node->left );
	//Print data for this node
	printf( "%s ", node->data );
	llTraverse( node->head );
	printf( "\n" );
	//Recurse on right subtree
	avlTraverse( node->right );
}

//Stores the line in the LinkedList for the
//node for the specified word, creating the
//node if necessary. Does not store duplicate
//line data.
void avlAddLine( AvlNode *avlNode, int line ) {
	//create head node if head is null
	if( avlNode->head == NULL ) {
		avlNode->head = llNew( line );
		avlNode->tail = avlNode->head;
	//otherwise, make a new node, link the current
	//tail to it, and update the tail pointer
	} else {
		if( line != avlNode->tail->line ) {
			LLNode *newLL = llNew( line );
			avlNode->tail->next = newLL;
			avlNode->tail = newLL;
		}
	}
}

//Stores the line in the LinkedList for the
//node for the specified word, creating the
//node if necessary. Does not store duplicate
//line data.
AvlNode *addLine( AvlNode *root, char *word, int line ) {
	avlFindAddNodeAdded = 0;
	root = avlFindAdd( root, word );
	avlAddLine( avlFindAddReturn, line );

	return root;
}

//Recursively prints the tree - used for debugging
void printTree( AvlNode *node, int depth, int left ) {
	//base case
	if( node == NULL ) return;
	//print space so you can tell how deep this node is
	int i;
	for( i = 0; i < depth; i++ ) {
		printf( "  " );
	}
	//print whether this was a left or right subchild
	printf( left ? "L: " : "R: " );
	//print data for the node
	printf( "%s                   height:%d balance:%d\n", node->data, node->height, avlBalance( node ) );
	//recurse on children
	printTree( node->left, depth + 1, 1 );
	printTree( node->right, depth + 1, 0 );
}