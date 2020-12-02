/*--------------------------------------------------------------------------*
 *----									----*
 *----		insertionSort.c						----*
 *----									----*
 *----	    This file defines a function that implements insertion	----*
 *----	sort on a linked-list of integers.				----*
 *----									----*
 *----	----	----	----	----	----	----	----	----	----*
 *----									----*
 *----	Version 1a		2020 January 5		Joseph Phillips	----*
 *----									----*
 *--------------------------------------------------------------------------*/

#include	"headers.h"


//  PURPOSE:  To sort the linked-list of nodes pointed to by 'nodePtr' using
//	the insertion sort algorithm.  Returns the first node of the sorted
//	list.
struct Node*	insertionSort	(struct Node*	nodePtr
       				)
{
  struct Node*	startPtr	= NULL;
  struct Node*	endPtr		= NULL;
  struct Node*	lowestPtr;
  struct Node*	lowestPrevPtr;

  while  (nodePtr != NULL)
  {
    struct Node*	prevPtr;
    struct Node*	run;

    lowestPrevPtr	= NULL;
    lowestPtr		= nodePtr;

    for  (prevPtr = nodePtr, run  = nodePtr->nextPtr_;
			     run != NULL;
	  prevPtr = run,     run = run->nextPtr_
	 )
    {
      if  (lowestPtr->value_ > run->value_)
      {
	lowestPtr	= run;
	lowestPrevPtr	= prevPtr;
      }
    }

    if  (lowestPrevPtr == NULL)
    {
      if  (startPtr == NULL)
      {
	startPtr = endPtr	= lowestPtr;
      }
      else
      {
	endPtr->nextPtr_	= lowestPtr;
	endPtr			= endPtr->nextPtr_;
      }

      nodePtr		= nodePtr->nextPtr_;
      endPtr->nextPtr_	= NULL;
    }
    else
    {
      if  (startPtr == NULL)
      {
	startPtr = endPtr	= lowestPtr;
      }
      else
      {
	endPtr->nextPtr_	= lowestPtr;
	endPtr			= endPtr->nextPtr_;
      }

      lowestPrevPtr->nextPtr_	= lowestPtr->nextPtr_;
      endPtr->nextPtr_		= NULL;
    }
  }

  print(startPtr);
  return(startPtr);
}
