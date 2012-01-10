//Linked List node - used in the tree's nodes to
//keep track of which lines the word shows up on
typedef struct llnode {
	//next node
	struct llnode *next;
	//a line that the word shows up on
	int line;
} LLNode;

//Creates a new node with the specified line number
LLNode *llNew( int line );
//Recursively print out the data from the LL,
//starting with node
void llTraverse( LLNode *node );

//--------------------------------

//AVL Tree node - used to keep track
//of words. AVL was chosen because the words
//have to show up in alphabetical order and it
//runs in O(log n)  time.
typedef struct avlnode {
	//left child
	struct avlnode *left;
	//right child
	struct avlnode *right;
	//word this node is storing data for
	char *data;
	//beginning of the linked list of lines
	//this word shows up on - used for
	//printing the data
	LLNode *head;
	//end of the linked list of lines
	//this word shows up on - used for
	//insertion
	LLNode *tail;
	//height of the subtree that starts
	//at this node - used for AVL balance
	int height;
} AvlNode;

//Finds the right spot for the word in question
//and adds a node to the tree if necessary.
//Returns the new tree from this node down.
//Also sets the variable bstFindAddReturn to
//the word's corresponding node.
AvlNode *avlFindAdd( AvlNode *node, char *data );
//Constructor for a node for the given word
AvlNode *avlNew( char *newData );
//Prints out the data in the tree, one line
//for each word and the lines it appears on
void avlTraverse( AvlNode *node );
//Stores the line in the LinkedList for the
//node for the specified word, creating the
//node if necessary. Does not store duplicate
//line data.
AvlNode *addLine( AvlNode *root, char *word, int line );
//Computes the height for the subtree starting
//at an AVL node and stores it
void avlSetHeight( AvlNode *node );
//Computes the balance factor of the subtree
//starting at this node
int avlBalance( AvlNode *node );
//Recursively prints the tree - used for debugging
void printTree( AvlNode *node, int depth, int left );

