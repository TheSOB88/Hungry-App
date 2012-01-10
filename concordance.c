#include "avl.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

//Prints out which lines the words in a document
//appear on. Takes the filename of the document
//as the first argument. A word is considered any
//consecutive string of letters or apostrophes.
//Doesn't work with words over 49 characters - God
//help you if you have a word that long.
int main( int argc, char **argv ) {
	//root of the tree
	AvlNode *root = NULL;
	//file to read words from
	FILE *file;
	//buffer current word being read
	char word[50];
	//position in the buffer
	int wordPos;
	//current line
	int line;
	//last character read
	char read;

	//open the file
	file = fopen( argv[1], "r" );

	//die if can't open file
	if( !file ) {
		printf( "Can't open file %s. Goodnight.\n", argv[1] );
		exit(1);
	}

	wordPos = 0;
	line = 1;
	//read a char until end of file reached
	while( ( read = fgetc( file ) ) != EOF ) {
		//if the last char read is a character of a word,
		//add to the buffer. If it's uppercase, make it lowercase
		if( ( read >= 'a' && read <= 'z' )
		 || ( read >= 'A' && read <= 'Z' )
		 || ( read == '\'' ) ) {
			//ensure lowercasosity
			if( read >= 'A' && read <= 'Z' ) {
				read += 'a' - 'A';
			}
			word[wordPos++] = read;
			//if the word is almost full, pack up and go home
			if( wordPos == 49 ) {
				word[wordPos] = '\0';
				root = addLine( root, word, line );
				wordPos = 0;
			}
		//if the last character wasn't part of a word,
		//and the word buffer has data, add the line
		//to a node for the word in the BST
		} else {
			word[wordPos] = '\0';
			if( wordPos > 0 ) {
				root = addLine( root, word, line );
			}
			wordPos = 0;
			if( read == '\n' ) {
				line++;
			}
		}
	}

	//print out the data
	avlTraverse( root );
}
