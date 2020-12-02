/*--------------------------------------------------------------------------*
 *----									----*
 *----		main.c							----*
 *----									----*
 *----	    This file defines the main functions for a program that	----*
 *----	sorts a linked list of randomly-generated integers.		----*
 *----									----*
 *----	----	----	----	----	----	----	----	----	----*
 *----									----*
 *----	Version 1a		2020 January 5		Joseph Phillips	----*
 *----									----*
 *--------------------------------------------------------------------------*/

#include	"headers.h"

#define		TEXT_LEN	256
#define		NUM_NUMBERS	(2*65536)

int	numNumbers	= NUM_NUMBERS;

//  PURPOSE:  To create and return the address of the first node of a linked
//	list of 'length' struct Node instances, each with a randomly-generated
//	integer in its 'value_' member variable.
struct Node*	createList	(int	length
       				)
{
  if  (length == 0)
  {
    return(NULL);
  }

  struct Node*	startPtr	= (struct Node*)malloc(sizeof(struct Node));
  struct Node*	endPtr		= startPtr;

  startPtr->value_		= rand() % 4096;
  startPtr->nextPtr_		= NULL;

  for  (length--;  length > 0;  length--)
  {
    endPtr->nextPtr_		= (struct Node*)malloc(sizeof(struct Node));
    endPtr->nextPtr_->value_	= rand() % 4096;
    endPtr->nextPtr_->nextPtr_	= NULL;
    endPtr			= endPtr->nextPtr_;
  }

  return(startPtr);
}


//  PURPOSE:  To print integer values in the linked list pointed to by
//	'nodePtr'.  No return value.
void		print		(const struct Node*	nodePtr
				)
{
  // printf("%d",nodePtr->value_);

  for  ( ;  nodePtr->nextPtr_ != NULL;  nodePtr = nodePtr->nextPtr_)
  {
    printf("%d ",nodePtr->value_);
  }
  printf("\n");
}


//  PURPOSE:  To 'free()' the 'struct Node' instances of the linked list
//	pointed to by 'nodePtr'.  No return value.
void		freeList	(struct Node*	nodePtr
				)
{
  struct Node*	nextPtr;

  for  ( ;  nodePtr != NULL;  nodePtr = nextPtr)
  {
    nextPtr	= nodePtr->nextPtr_;
    free(nodePtr);
  }

}


//  PURPOSE:  To run this program.  Ignores command line arguments.  Returns
//	'EXIT_SUCCESS' to OS.
int		main		()
{
  int		choice;
  struct Node*	nodePtr	= createList(numNumbers);

  print(nodePtr);

  do
  {
    char	text[TEXT_LEN];

    printf
	("How do you want to sort %d numbers?\n"
	 "(1) Insertion sort\n"
	 "(2) Merge sort\n"
	 "Your choice (1 or 2)? ",
	 NUM_NUMBERS
	);
    fgets(text,TEXT_LEN,stdin);
    choice = strtol(text,NULL,10);
  }
  while  ( (choice < 1) || (choice > 2) );
  switch  (choice)
  {
  case 1 :
    nodePtr	= insertionSort(nodePtr);
    break;
  case 2 :
    nodePtr	= mergeSortWrapper(nodePtr);
    break;
  }

  freeList(nodePtr);
  return(EXIT_SUCCESS);
}
