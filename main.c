/*
    Title:		Huffman Decoder
    Date:		09.06.18
    Author:		Liad Oz
    Description:
                    FUNCTIONS
                    ---------
                    This program creates huffman tree by given database of chars and
                    it capable of doing few acts on it:
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

// define
#define _CRT_SECURE_NO_WARNINGS
#define TRUE 1									// boolean definition 
#define FALSE 0
#define _MAX_BITS 8								// maximum number of bits in each group
#define _STRING_LENGTH 10000 * _MAX_BITS		// string default size

// include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

/* 
	This is the main function which handles client's input for menu's option and sends it to the suitable method.
 	Program ends only when client's option is 3 in the first menu or memory	doesn't allocated properly.											
*/
int main()
{

	// variables
	
	char str[_STRING_LENGTH];			// client's input for the string built for huffman tree
	int option;							// client's input for menu's option
	int isValid;						// flag for client's input

	HuffmanTree * root = NULL;			// ready huffman tree saved in memory
	char * modifiedStr = NULL;			// the output of the function working on saved huffmanT

	LinkedListNode * head = NULL;		// using *p to serialize huffman tree (menu 1/option 2)
	LinkedListNode * lstNode = NULL;
	HuffmanTree * treeNode = NULL;
	

	while (1){

		do{
			isValid = TRUE;

			printFirstMenu();

			gets(str); // expecting for integer between 1 to 3
			option = atoi(str);

			switch (option){

				// option 1 - create a Huffman Tree
				case 1:

					gets(str); // expecting string with maximum 10000 chars ending with '@'
					
					root = createHuffmanTreeByString(str);
					if (!root) exit(1);

					break;

				// option 2 - deserialize a Huffman Tree
				case 2:

					gets(str); // expecting serialized string 'char:freq'
					if (str[0] == '\0') continue; 

					treeNode = initHuffmanTreeNodeBySerial(str);
					if (!treeNode) exit(1);
					
					head = initHuffmanListNodeByValues(treeNode, NULL);
					if (!head){ 
						free(treeNode); 
						exit(1); 
					}

					while (1){

						gets(str);
						if (str[0] == '\0') break;

						treeNode = initHuffmanTreeNodeBySerial(str);
						if (!treeNode){ 
							destructHuffmanList(head); 
							exit(1); 
						}
						
						lstNode = initHuffmanListNodeByValues(treeNode, head);
						if (!lstNode){ 
							freeHuffmanTree(treeNode); 
							destructHuffmanList(head);
							exit(1); 
						}

						head = lstNode;
					}

					root = createHuffmanTreeByHuffmanList(head);
					if (!root){ 
						destructHuffmanList(head); 
						exit(1); 
					}

					break;

				// option 3 - exit
				case 3:
					break;

				default:
					// wrong input
					isValid = FALSE;

			} // end switch

		} while (isValid != TRUE);


		// break main loop to end program
		if (option == 3) break; 


		while (1){
		
			do{
				isValid = TRUE;

				printSecondMenu();

				gets(str); // expecting for integer between 1 to 6
				option = atoi(str);

				switch (option){

					// option 1 - encode text using the Huffman Tree
					case 1:

						gets(str); // expecting string which it's chars are inside the allocated huffman tree
						
						modifiedStr = encodeStringWithHuffmanTree(root, str);
						if (!modifiedStr){ 
							freeHuffmanTree(root); 
							exit(1); 
						}

						puts(modifiedStr);
						free(modifiedStr); 
						break;

					// option 2 - bit-encode text using the Huffman Tree
					case 2:

						gets(str); // expecting string which it's chars inside the allocated huffman tree
						
						modifiedStr = bitEncodeStringWithHuffmanTree(root, str);
						if (!modifiedStr){ 
							freeHuffmanTree(root); 
							exit(1);
						}

						puts(modifiedStr);
						free(modifiedStr); 
						break;

					// option 3 - decode using the Huffman Tree
					case 3:

						gets(str); // expecting binary sequence multiple of _MAX_BITS

						modifiedStr = decodeStringWithHuffmanTree(root, str);
						if (!modifiedStr){ 
							freeHuffmanTree(root); 
							exit(1); 
						}

						puts(modifiedStr);
						free(modifiedStr); 
						break;

					// option 4 - bit-decode using the Huffman Tree
					case 4:

						gets(str); // expecting string of ascii values

						modifiedStr = bitDecodeStringWithHuffmanTree(root, str);
						if (!modifiedStr){ 
							freeHuffmanTree(root); 
							exit(1); 
						}
						
						puts(modifiedStr);
						free(modifiedStr); 
						break;

					// option 5 - serialize the Huffman Tree
					case 5:
						serializeHuffmanTree(root); 
						puts("");
						break;

					// option 6 - free the in-memory Huffman Tree
					case 6:
						freeHuffmanTree(root);
						break;

					// default - wrong input
					default:
						isValid = FALSE;

				} // end switch

			} while (isValid != TRUE);


			// exit second menu
			if (option == 6) break;


		}// end while


	}// end while


	return 0;
}

/*
	This function builds Huffman tree by given Huffman list.
*/
HuffmanTree * createHuffmanTreeByHuffmanList(LinkedListNode * head)
{
	LinkedListNode * lstNode, *temp;
	HuffmanTree * treeNode;

	while (head->next)
	{
		// sort lst by huffman priority definition
		sortHuffmanList(&head);

		// init & setting a new tree node by huffman tree definition
		treeNode = initHuffmanTreeNode();
		if (!treeNode) { 
			destructHuffmanList(head); 
			return NULL; 
		}

		treeNode->left = head->tree;
		head->tree->parent = treeNode;

		treeNode->right = head->next->tree;
		head->next->tree->parent = treeNode;

		treeNode->freq = treeNode->left->freq + treeNode->right->freq;
		treeNode->ch = treeNode->left->ch;

		// init list node containing the new tree node and append to list's tale
		lstNode = initHuffmanListNodeByValues(treeNode, NULL);
		if (!lstNode) { 
			destructHuffmanList(head); 
			return NULL; 
		}

		lstappend(&head, lstNode);

		// isolate then dealloc the two used list nodes (don't use destructHuffmanList func)
		temp = head;
		head = head->next->next; // promote list head
		temp->next->next = NULL;
		freeHuffmanList(temp);
	}

	// free the last list node contains the final huffman tree (don't use destructHuffmanList func)
	treeNode = head->tree;
	freeHuffmanList(head);

	return treeNode;
}
/*
	This function builds Huffman tree by given string.
*/
HuffmanTree * createHuffmanTreeByString(char str[])
{
	LinkedListNode * head = NULL, *lstNode = NULL;
	HuffmanTree * treeNode = NULL;
	char ch = str[0];

	treeNode = initHuffmanTreeNodeByValues(removeCharFromString(str, ch), ch, NULL, NULL, NULL);
	if (!treeNode) return NULL;

	head = initHuffmanListNodeByValues(treeNode, NULL);
	if (!head){ 
		free(treeNode); 
		return NULL; 
	}

	// building huffman list for str (built inverse, later been sorted)
	while (str[0] != '\0'){

		ch = str[0];

		treeNode = initHuffmanTreeNodeByValues(removeCharFromString(str, ch), ch, NULL, NULL, NULL);
		if (!treeNode){ 
			destructHuffmanList(head); 
			return NULL; 
		}

		lstNode = initHuffmanListNodeByValues(treeNode, head);
		if (!lstNode){ 
			freeHuffmanTree(treeNode); 
			destructHuffmanList(head); 
			return NULL; 
		}

		head = lstNode;
	}

	treeNode = createHuffmanTreeByHuffmanList(head);
	if (!treeNode){ 
		destructHuffmanList(head); 
		return NULL; 
	}

	return treeNode;
}
/*
	This function encodes string via Huffman tree depending on the route.
*/
char * encodeStringWithHuffmanTree(HuffmanTree * root, char str[])
{
	char * encodedStr, encodedChar[4 * _MAX_BITS] = { 0 };
	char temp[_STRING_LENGTH] = { 0 };
	int length, extraBits;
	int i;

	// encoding
	for (i = 0; str[i] != '\0'; i++){
		getRouteWithHuffmanTree(searchLeafInHuffmanTreeByChar(root, str[i]), encodedChar);
		strcat(temp, encodedChar);
	}

	// complete to _MAX_BITS
	length = (int)strlen(temp);

	if (length%_MAX_BITS != 0){
		extraBits = (((length / _MAX_BITS) + 1)*_MAX_BITS) - length;
		for (i = 0; i < extraBits; i++)
			strcat(temp, "0");
	}

	encodedStr = (char *)malloc((strlen(temp) + 1)*sizeof(char));
	if (!encodedStr) return NULL;
	strcpy(encodedStr, temp);

	return encodedStr;
}
/*
	This function bit encodes string via Huffman tree.
*/
char * bitEncodeStringWithHuffmanTree(HuffmanTree * root, char str[])
{
	char * bitEncodedStr, *encodedStr = NULL;
	char temp[_MAX_BITS + 1] = { 0 };
	char asciiVal;
	int maxBitsGroups;
	int i, j, k;

	// encode string
	encodedStr = encodeStringWithHuffmanTree(root, str);
	if (!encodedStr) return NULL;

	maxBitsGroups = (int)strlen(encodedStr) / _MAX_BITS;

	bitEncodedStr = (char *)malloc((maxBitsGroups + 1) * sizeof(*bitEncodedStr));
	if (!bitEncodedStr) { 
		free(encodedStr); 
		return NULL; 
	}

	strcpy(bitEncodedStr, "");

	// convert encoded string to ascii values
	for (i = 0; i < maxBitsGroups; i++){
		for (k = 0, j = i * _MAX_BITS; j < (i + 1)*_MAX_BITS; j++, k++)
			temp[k] = *(encodedStr + j);

		asciiVal = convertBinToAscii(temp);

		// if ascii val null than string overflow, print error
		if (asciiVal == '\0'){
			free(encodedStr);
			free(bitEncodedStr);
			printError(2);
			return NULL;
		}

		*(bitEncodedStr + i) = asciiVal;
	}
	*(bitEncodedStr + i) = '\0';

	free(encodedStr);

	return bitEncodedStr;
}
/*
	This function decodes encoded string via Huffman tree.
*/
char * decodeStringWithHuffmanTree(HuffmanTree * root, char str[])
{
	char * decodeStr;
	char temp[_STRING_LENGTH] = { 0 };
	int i;

	for (i = 0; str[0]; i++){
		temp[i] = decodeCharWithHuffmanTree(root, str);
		if (temp[i] == '@') break;
	}

	decodeStr = (char *)malloc((strlen(temp) + 1)*sizeof(*decodeStr));
	if (!decodeStr) return NULL;
	strcpy(decodeStr, temp);

	return decodeStr;
}
/*
	This function bit decodes string via Huffman tree.
*/
char * bitDecodeStringWithHuffmanTree(HuffmanTree * root, char str[])
{
	char * bitDecodedStr, *binVal;
	char temp[_STRING_LENGTH] = { 0 };
	int i;

	for (i = 0; str[i]; i++){
		binVal = convertAsciiToBin(str[i]);
		if (!binVal) return NULL;

		strcat(temp, binVal);
		free(binVal);
		if (str[i] == '@') break;
	}

	bitDecodedStr = decodeStringWithHuffmanTree(root, temp);
	if (!bitDecodedStr) return NULL;

	return bitDecodedStr;
}


/*
	This function gets unsorted Huffman list and bubble sort it by Huffman trees prioritize definition.
*/
void sortHuffmanList(LinkedListNode ** head)
{
	LinkedListNode * temp, *tempLen;
	HuffmanTree * tree1, *tree2;
	int isSorted = FALSE;
	int lstlen;
	int i, j;

	// get list length
	for (i = 0, tempLen = *head; tempLen; i++, tempLen = tempLen->next);
	lstlen = i;

	// sort list using bubble sort
	for (i = lstlen - 1; i > 0 && isSorted == FALSE; i--){

		temp = *head;
		isSorted = TRUE;

		for (j = 0; j < i; j++)
		{
			tree1 = temp->tree;
			tree2 = temp->next->tree;

			// in case true swap tree1 with tree2
			if (prioritizeHuffmanTrees(tree1, tree2) == tree1){

				temp->tree = tree2;
				temp->next->tree = tree1;

				isSorted = FALSE;
			}
			temp = temp->next;
		}
	}
}
/*
	This function appends list with another list or node.
*/
void lstappend(LinkedListNode ** head, LinkedListNode * node)
{
	LinkedListNode * temp;

	if (!*head) *head = node;

	for (temp = *head; temp->next; temp = temp->next);
	temp->next = node;
}
/*
	This function determine which Huffman tree is prior to the other one and returns the prior.
*/
HuffmanTree * prioritizeHuffmanTrees(HuffmanTree * tree1, HuffmanTree * tree2)
{
	unsigned char asciiVal1, asciiVal2;

	if (tree2->freq > tree1->freq)
		return tree2;
	if (tree2->freq < tree1->freq)
		return tree1;

	// case freq equals check ascii of chars
	asciiVal1 = (unsigned char)tree1->ch;
	asciiVal2 = (unsigned char)tree2->ch;

	if (asciiVal2 >= asciiVal1)
		return tree2;

	return tree1;
}
/*
	This recursive function search inside an unsorted binary tree.
*/
HuffmanTree * searchLeafInHuffmanTreeByChar(HuffmanTree * root, char ch)
{
	HuffmanTree * temp;

	if (!root) return NULL;
	if (root->ch == ch && root->left == NULL) return root;

	temp = searchLeafInHuffmanTreeByChar(root->left, ch);
	if (temp) return temp;

	return searchLeafInHuffmanTreeByChar(root->right, ch);
}
/*
	This function encoding char via using the parents of the leaf adding to the string '0'/'1' if the leaves comes form left/right.
*/
void getRouteWithHuffmanTree(HuffmanTree * leaf, char encodedStr[])
{
	HuffmanTree * temp = leaf;
	HuffmanTree * tempParent = NULL;
	int i;
	int depth;

	if (!leaf) return;

	// get the depth of the leaf
	for (i = 0, tempParent = leaf; tempParent->parent; i++, tempParent = tempParent->parent);
	depth = i;

	encodedStr[i] = '\0';

	for (i = depth - 1; temp->parent; i--){
		tempParent = temp->parent;
		if (tempParent->left == temp)
			encodedStr[i] = '0';
		else
			encodedStr[i] = '1';
		temp = temp->parent;
	}

}
/*
	This function decodes by going thru the Huffman tree by the route of the encoded string to the leaf and returning it char value.
*/
char decodeCharWithHuffmanTree(HuffmanTree * root, char * str)
{
	int binVal;
	while (root && str[0]){
		binVal = str[0] - '0';
		switch (binVal){
		case 0:
			root = root->left;
			break;

		case 1:
			root = root->right;
			break;
		}
		strcpy(str, str + 1);
		if (!root->left) return root->ch;
	}

	return '\0';
}
/*
	This function convert binary to ascii char.
*/
char convertBinToAscii(char binSequence[])
{
	double asciiVal = 0;
	int bin;
	int i, j;

	// binary to decimal
	for (i = 0, j = strlen(binSequence) - 1; binSequence[i]; i++, j--){
		bin = binSequence[i] - '0';
		asciiVal += (bin * pow(2, j));
	}
	return (unsigned char)asciiVal;
}
/*
	This function convert ascii character to binary.
*/
char * convertAsciiToBin(char ch)
{
	char * binVal;
	char temp[4 * _MAX_BITS] = { 0 };
	int asciiVal = (unsigned char)ch;
	int i;

	// binary to decimal
	for (i = _MAX_BITS - 1; i >= 0; i--){
		if (asciiVal % 2 == 0)
			temp[i] = '0';
		else
			temp[i] = '1';
		asciiVal /= 2;
	}

	binVal = (char *)malloc((strlen(temp) + 1) * sizeof(*binVal));
	if (!binVal) return NULL;

	strcpy(binVal, temp);

	return binVal;
}
/*
	This function loops over a string and removes all the same chars equals	to the given char. The function returns the number of removes.
*/
int removeCharFromString(char str[], char ch)
{
	char strNoRepeats[_STRING_LENGTH];
	int count = 0;
	int i, j;

	for (i = 0, j = 0; str[i] != '\0'; i++){
		if (str[i] == ch)
			count++;
		else{
			strNoRepeats[j] = str[i];
			j++;
		}
	}

	strNoRepeats[j] = '\0';
	strcpy(str, strNoRepeats);

	return count;
}


/*
	This is Huffman tree initializer function.
*/
HuffmanTree * initHuffmanTreeNode()
{
	HuffmanTree * node = (HuffmanTree *)malloc(sizeof(HuffmanTree));
	if (!node) return NULL;

	node->freq = 0;
	node->ch = '\0';
	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;
	return node;
}
/*
	This is Huffman tree initializer function by values.
*/
HuffmanTree * initHuffmanTreeNodeByValues(int freq, char ch, HuffmanTree * parent, HuffmanTree * left, HuffmanTree * right)
{
	HuffmanTree * node = (HuffmanTree *)malloc(sizeof(HuffmanTree));
	if (!node) return NULL;

	node->freq = freq;
	node->ch = ch;
	node->parent = parent;
	node->left = left;
	node->right = right;
	return node;
}
/*
	This is Huffman Tree initializer function by serialized string.
*/
HuffmanTree * initHuffmanTreeNodeBySerial(char str[])
{
	HuffmanTree * node = (HuffmanTree *)malloc(sizeof(HuffmanTree));
	if (!node) return NULL;

	node->ch = str[0];
	node->freq = atoi(str + 2);
	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;
	return node;
}
/*
	This is Huffman tree deallocate function by given root.
*/
void freeHuffmanTree(HuffmanTree * root)
{
	if (!root)
		return;
	
	freeHuffmanTree(root->left);
	freeHuffmanTree(root->right);

	free(root); 
}
/*
	This is Huffman list initializer function.
*/
LinkedListNode * initHuffmanListNode()
{
	LinkedListNode * node = (LinkedListNode *)malloc(sizeof(LinkedListNode));
	if (!node) return NULL;

	node->tree = NULL;
	node->next = NULL;

	return node;
}
/*
	This is Huffman list initializer function by values.
*/
LinkedListNode * initHuffmanListNodeByValues(HuffmanTree * tree, LinkedListNode * next)
{
	LinkedListNode * node = (LinkedListNode *)malloc(sizeof(LinkedListNode));
	if (!node) return NULL;

	node->tree = tree;
	node->next = next;

	return node;
}
/*
	This is Huffman list deallocate function (doesn't deallocate the huffman trees inside).
*/
void freeHuffmanList(LinkedListNode * head)
{
	LinkedListNode * temp;

	while (head){
		temp = head;
		head = head->next;
		free(temp); 
	}
}
/*
	This is Huffman list deallocate function.
*/
void destructHuffmanList(LinkedListNode * head)
{
	LinkedListNode * temp;

	while (head){
		temp = head;
		head = head->next;

		freeHuffmanTree(temp->tree); 
		free(temp); 
	}
}

/*
	This is printing function displaying the first menu.
*/
void printFirstMenu()
{
	puts("--- No Huffman Tree ---");
	puts("1. Create a Huffman Tree from text");
	puts("2. Deserialize a Huffman Tree");
	puts("3. Exit");
	printf("Enter you choice: ");
}
/*
	This is printing function displaying the second menu.
*/
void printSecondMenu()
{
	puts("*** Huffman Tree in-memory ***");
	puts("1. Encode text using the Huffman Tree");
	puts("2. Bit-encode text using the Huffman Tree");
	puts("3. Decode using the Huffman Tree");
	puts("4. Bit-decode using the Huffman Tree");
	puts("5. Serialize the Huffman Tree");
	puts("6. Free the in-memory Huffman Tree");
	printf("Enter you choice: ");
}
/*
	This is printing function displaying the leaves of given huffman tree root in serialized format "char:freq".
*/
void serializeHuffmanTree(HuffmanTree * root)
{
	if (!root) return;

	serializeHuffmanTree(root->left);
	if (!root->left)
		printf("%c:%d\n", root->ch, root->freq);
	serializeHuffmanTree(root->right);
}
/*
	This function prints error by error number:
	1 => malfunction in allocation.
	2 => one or more chars were encoded to null.
*/
void printError(int numOfError)
{
	char *errors[] = {  "Error #1: malfunction in allocation.",
						"Error #2: can't complete bit-encoding because one or more chars were encoded to null." };

	puts(errors[numOfError - 1]);
}
