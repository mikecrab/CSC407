/*-------------------------------------------------------------------------*
 *---									---*
 *---		decoderServer1.cpp					---*
 *---									---*
 *---	    This file defines a function that:				---*
 *---	(1) Make two pipes:						---*
 *---	    (a) the first is for the parent process to talk to the	---*
 *---		child process running 'DECODER_PROG_NAME'		---*
 *---	    (b) the second is for the child process running		---*
 *---		'DECODER_PROG_NAME' to talk to the parent process.	---*
 *---									---*
 *---	(2) Make the child process and puts its process id in global	---*
 *---	    variable 'decoderPid'.  The this child process should:	---*
 *---		dup2(somePipeFileDescriptor,STDIN_FILENO)		---*
 *---	    to get its input from the "to decoder" pipe, and should:	---*
 *---		dup2(somePipeFileDescriptor,STDOUT_FILENO)		---*
 *---	    to send its output to the "to parent" pipe.			---*
 *---	    It should close() all unneeded pipe file descriptors and	---*
 *---	    execute DECODER_PROG_NAME (a string constant defined in	---*
 *---	     the header file).						---*
 *---									---*
 *---	    Meanwhile, the parent should close() the file descriptors	---*
 *---	    only used by the child, set 'toDecodeFd' to the pipe file	---*
 *---	    descriptor for talking to DECODER_PROG_NAME, and set	---*
 *---	    'toParentFd', to the pipe file descriptor for talking to	---*
 *---	    the parent.							---*
 *---									---*
 *---	(3) Return true (a C++ constant) on success or false otherwise.	---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a		2020 March 15		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

#include	"decoderServer.h"

//---		Definition of global vars:				---//

//  PURPOSE:  To hold the file descriptor for talking to the child process
//	running 'DECODER0_PROG_NAME'.
int		toDecodeFd	= ERROR_FD;

//  PURPOSE:  To hold the file descriptor for talking to the parent process.
int		toParentFd	= ERROR_FD;

//  PURPOSE:  To hold the process ID of the child process running the
//	'decoder0' program.
pid_t		decoderPid	= ERROR_PID;


//---		Definition of functions:				---//

//  PURPOSE:  To:
//	(1) Make two pipes:
//	    (a) the first is for the parent process to talk to the
//		child process running 'DECODER_PROG_NAME'
//	    (b) the second is for the child process running
//		'DECODER_PROG_NAME' to talk to the parent process.
//
//	(2) Make the child process and puts its process id in global
//	    variable 'decoderPid'.  The this child process should:
//		dup2(somePipeFileDescriptor,STDIN_FILENO)
//	    to get its input from the "to decoder" pipe, and should:
//		dup2(somePipeFileDescriptor,STDOUT_FILENO)
//	    to send its output to the "to parent" pipe.
//	    It should close() all unneeded pipe file descriptors and
//	    execute DECODER_PROG_NAME (a string constant defined in
//	     the header file).
//
//	    Meanwhile, the parent should close() the file descriptors
//	    only used by the child, set 'toDecodeFd' to the pipe file
//	    descriptor for talking to DECODER_PROG_NAME, and set 'toParentFd',
//	    to the pipe file descriptor for talking to the parent.
//
//	(3) Return true (a C++ constant) on success or false otherwise.
bool		didStartDecoders()
{
  //  I.  Application validity check:
  //  II.  Start decoder:
  int	toDecode[2];
  int	toParent[2];
  
  //  YOUR CODE HERE
  if  ((pipe(toDecode)  < 0) || (pipe(toParent)< 0)) {
    return(false);
  }

  decoderPid = fork();
  if(decoderPid == 0) {
    close(toDecode[1]);
    close(toParent[0]);
    
    dup2(toDecode[0], STDIN_FILENO);
    dup2(toParent[1], STDOUT_FILENO);
    
    execl(DECODER_PROG_NAME, DECODER_PROG_NAME, NULL);
  } else if(decoderPid > 0) {
    close(toDecode[0]); 
    close(toParent[1]);
 
    toDecodeFd = toDecode[1];   
    toParentFd = toParent[0];
  } else {
    return(false);
  }

  //  III.  Finished:
  return(true);
}
