/*-------------------------------------------------------------------------*
 *---									---*
 *---		decoderServer2.cpp					---*
 *---									---*
 *---	    This file defines a function run by the child threads that	---*
 *---	take turns getting cipher text from the client, decrypting it	---*
 *---	with the decoding program, and storing it in the Message   	---*
 *---	instance.							---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a		2020 March 15		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

#include	"decoderServer.h"

//  PURPOSE:  To keep track of whose turn it is (which thread).
static
int			turn		= 0;


//  PURPOSE:  To hold the lock for determining which thread can use the
//	decoding processes.
pthread_mutex_t		lock;


//  PURPOSE:  To hold the conditions for determining which thread can use the
//	decoding processes.
pthread_cond_t		threadCond[NUM_THREADS];


//  PURPOSE:  To be run by 'NUM_THREAD' different threads.
//	In a loop all threads:
//	-(1) get the lock
//	-(2) wait for it to be their turn
//	-(3) get 'BUFFER_LEN' bytes from the client
//	    (using messagePtr->getClientFd())
//	(4) send that text to the decoding program
//	(5) receive 'BUFFER_LEN' bytes of decoded response
//	-(6) add a '\0' at the end
//	(7) add the text to the Message instance pointed to by 'messagePtr'
//	    using the 'append()' method
//	(8) makes it the other threads turn, use:
//		    turn	= 1-threadNumber;
//	(9) signals the other thread and unlocks the critical section.
//
//	Returns 'NULL' at the end.
void*	      	handleClient	(void*		vPtr
				)
{
  //  I.  Application validity check:

  //  II.  Handle client:
  char			text[BUFFER_LEN+1];
  int			numBytes;
  ForThreadInfo*	threadInfoPtr	= (ForThreadInfo*)vPtr;
  Message*		messagePtr	= threadInfoPtr->messagePtr_;
  int			threadNumber	= threadInfoPtr->threadNum_;

  for  (int textNum = 0;  textNum < NUM_TEXTS_PER_THREAD;  textNum++)
  {
    //  YOUR CODE HERE
    pthread_mutex_lock(&lock);
    
    if(threadNumber != turn) {
      pthread_cond_wait(&threadCond[threadNumber],&lock);
    }
    
    read(messagePtr->getClientFd(), text, BUFFER_LEN);
    printf("Thread %d: cipher text: %s",threadNumber,text);
    write(toDecodeFd, text, BUFFER_LEN);
    
    
    //  YOUR CODE HERE
    read(toParentFd, text, BUFFER_LEN);
    printf("Thread %d: plain text:  %s",threadNumber,text);
    text[BUFFER_LEN+1] = '\0';
    messagePtr->append(text);
    
    //  YOUR CODE HERE
    turn = 1-threadNumber;
    pthread_cond_signal(&threadCond[turn]);
    pthread_mutex_unlock(&lock);
  }

  //  III.  Finished:
  return(NULL);
}
