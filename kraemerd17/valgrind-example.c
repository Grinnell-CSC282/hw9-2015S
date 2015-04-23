/*
* Program to demonstrate valgrind's ability to spot memory errors. 
* From the Wikibook on C Programming, "Memory management",
* https://en.wikibooks.org/wiki/C_Programming/Memory_management.
*/
#include <stdlib.h>
#include <stdio.h>

typedef struct
BSTNode 
{
    int value;
    struct BSTNode * left;
    struct BSTNode * right;
} BSTNode;

int
main () 
{
    BSTNode * temp = (BSTNode *) calloc (1, sizeof (BSTNode));
    temp->left = (BSTNode*) calloc (1, sizeof (BSTNode));

    free(temp);
    return 0;
}
                                
