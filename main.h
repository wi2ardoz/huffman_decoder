/*
	Title:		    Assignment #4
	Date:		    09.06.18
	Author:		    Liad Oz
	Description:
					FUNCTIONS
					---------
					This program creates huffman tree by given database of chars and
					is capable of doing few acts on it:
					option 1 => encode text using the Huffman Tree
					option 2 => bit-encode text using the Huffman Tree
					option 3 => decode using the Huffman Tree
					option 4 => bit-decode using the Huffman Tree
					option 5 => serialize the Huffman Tree / printing the leaves

					NOTES
					-----
					1) Tree memory must contain '@' char.
					2) At the end of each encoded string must be '@' char.
					3) Bit encode won't encode if one or more chars will be encoded to null.
 */

// include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef MAIN_H
#define MAIN_H

// define
#define _CRT_SECURE_NO_WARNINGS			
#define TRUE 1									// boolean definition 
#define FALSE 0	
#define _MAX_BITS 8								// maximum number of bits in each group
#define _STRING_LENGTH 10000 * _MAX_BITS		// string default size


// structs
typedef struct HuffmanTree {
	int freq;
	char ch;
	struct HuffmanTree* parent;
	struct HuffmanTree* left;
	struct HuffmanTree* right;
} HuffmanTree;
typedef struct LinkedListNode {
	HuffmanTree *tree;
	struct LinkedListNode *next;
} LinkedListNode;

// prototypes

// main
HuffmanTree * createHuffmanTreeByHuffmanList(LinkedListNode *);
HuffmanTree * createHuffmanTreeByString(char[]);
char * encodeStringWithHuffmanTree(HuffmanTree *, char[]);
char * bitEncodeStringWithHuffmanTree(HuffmanTree *, char[]);
char * decodeStringWithHuffmanTree(HuffmanTree *, char[]);
char * bitDecodeStringWithHuffmanTree(HuffmanTree *, char[]);

// utilities
HuffmanTree * searchLeafInHuffmanTreeByChar(HuffmanTree *, char);
void getRouteWithHuffmanTree(HuffmanTree *, char[]);
char decodeCharWithHuffmanTree(HuffmanTree *, char *);
HuffmanTree * prioritizeHuffmanTrees(HuffmanTree *, HuffmanTree *);
void sortHuffmanList(LinkedListNode **);
void lstappend(LinkedListNode **, LinkedListNode *);
char convertBinToAscii(char[]);
char * convertAsciiToBin(char);
int removeCharFromString(char[], char);

// struct
HuffmanTree * initHuffmanTreeNode();
HuffmanTree * initHuffmanTreeNodeByValues(int, char, HuffmanTree *, HuffmanTree *, HuffmanTree *);
HuffmanTree * initHuffmanTreeNodeBySerial(char[]);
void freeHuffmanTree(HuffmanTree *);
LinkedListNode * initHuffmanListNode();
LinkedListNode * initHuffmanListNodeByValues(HuffmanTree *, LinkedListNode *);
void freeHuffmanList(LinkedListNode *);
void destructHuffmanList(LinkedListNode *);

// handling
void printFirstMenu();
void printSecondMenu();
void serializeHuffmanTree(HuffmanTree *);
void printError(int);

#endif  /* for MAIN_H */
