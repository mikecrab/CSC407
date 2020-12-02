/*--------------------------------------------------------------------------*
 *----									----*
 *----		mergeSort.c						----*
 *----									----*
 *----	    This file defines a function that implements merge-sort on	----*
 *----	a linked-list of integers.		       			----*
 *----									----*
 *----	----	----	----	----	----	----	----	----	----*
 *----									----*
 *----	Version 1a		2020 January 5		Joseph Phillips	----*
 *----									----*
 *--------------------------------------------------------------------------*/

#include	"headers.h"


//  PURPOSE:  To sort the linked-list of nodes pointed to by 'nodePtr' using
//	the merge sort algorithm.  Returns the first node of the sorted	list.
struct Node*	mergeSort	(struct Node*	nodePtr
				)
{

  if  ( (nodePtr == NULL) || (nodePtr->nextPtr_ == NULL) )
  {
    return(nodePtr);
  }

  struct Node*	run;
  struct Node*	run2;
  struct Node*	lastPtr	= NULL;

  for  ( run = run2 = nodePtr;
         (run2 != NULL) && (run2->nextPtr_ != NULL);
	 lastPtr = run, run = run->nextPtr_,  run2 = run2->nextPtr_->nextPtr_
       );

  lastPtr->nextPtr_	= NULL;
  run2			= mergeSort(run);
  run			= mergeSort(nodePtr);

  nodePtr	= NULL;
  lastPtr	= NULL;

  while  ( (run != NULL) && (run2 != NULL) )
  {
    if  (run->value_ < run2->value_)
    {
      if  (nodePtr == NULL)
      {
        nodePtr = lastPtr	= run;
      }
      else
      {
	lastPtr	= lastPtr->nextPtr_	= run;
      }

      run	= run->nextPtr_;
    }
    else
    {
      if  (nodePtr == NULL)
      {
        nodePtr = lastPtr	= run2;
      }
      else
      {
	lastPtr	= lastPtr->nextPtr_	= run2;
      }

      run2	= run2->nextPtr_;
    }
  }

  if  (run == NULL)
  {
    if  (lastPtr == NULL)
    {
      nodePtr		= run2;
    }
    else
    {
      lastPtr->nextPtr_	= run2;
    }
  }
  else
  {
    if  (lastPtr == NULL)
    {
      nodePtr		= run;
    }
    else
    {
      lastPtr->nextPtr_	= run;
    }
  }

  return(nodePtr);
}

struct Node*	mergeSortWrapper(struct Node*	nodePtr
				)
{
  nodePtr	= mergeSort(nodePtr);

  print(nodePtr);
  return(nodePtr);
}	  
