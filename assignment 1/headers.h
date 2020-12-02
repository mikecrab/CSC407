/*--------------------------------------------------------------------------*
 *----									----*
 *----		headers.h						----*
 *----									----*
 *----	    This file declares common headers used through-out the	----*
 *----	the singly-linked list sorting program.				----*
 *----									----*
 *----	----	----	----	----	----	----	----	----	----*
 *----									----*
 *----	Version 1a		2020 January 5		Joseph Phillips	----*
 *----									----*
 *--------------------------------------------------------------------------*/

#include	<stdlib.h>
#include	<stdio.h>
#include	"node.h"

struct Node* insertionSort(struct Node*);
struct Node* mergeSort(struct Node*);
void print(const struct Node*);
