/*-------------------------------------------------------------------------*
 *---									---*
 *---		decoderServer.h						---*
 *---									---*
 *---	    This file declares C functions and constants common to both	---*
 *---	the client and server.						---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a					Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

//---		Header file inclusion					---//

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<unistd.h>	// For unlink()
#include	<sys/types.h>	// For waitpid(), opendir()
#include	<sys/wait.h>	// For waitpid()
#include	<dirent.h>	// For opendir(), readdir(), closedir()
#include	<sys/socket.h>	// For socket()
#include	<netinet/in.h>	// For sockaddr_in and htons()
#include	<netdb.h>	// For getaddrinfo()
#include	<errno.h>	// For errno var
#include	<sys/stat.h>	// For open(), read(),write(), stat()
#include	<fcntl.h>	// and close()
#include	<pthread.h>	// For pthread_create()


//---		Definition of constants:				---//

#define		STD_ERROR_MSG		"Error doing operation"

#define		DECODER_PROG_NAME	"./decoder"

const int	BUFFER_LEN		= 32;

const int	ERROR_FD		= -1;

const pid_t	ERROR_PID		= -1;

const int	NUM_TEXTS		= 6;

const int	NUM_THREADS		= 2;

const int	NUM_TEXTS_PER_THREAD	= NUM_TEXTS / NUM_THREADS;


//---		Declaration of global variables:			---//

//  PURPOSE:  To hold the file descriptor for talking to the child process
//	running 'DECODER_PROG_NAME'.  Is initialized to 'ERROR_PID'.
extern
int		toDecodeFd;


//  PURPOSE:  To hold the file descriptor for talking to the parent process.
//	Is initialized to 'ERROR_PID'.
extern
int		toParentFd;


//  PURPOSE:  To hold the process ID of the child process running the
//	'decoder' program.  Is initialized to 'ERROR_PID'.
extern
pid_t		decoderPid;


//  PURPOSE:  To hold the lock for determining which thread can use the
//	decoding processes.
extern
pthread_mutex_t		lock;


//  PURPOSE:  To hold the conditions for determining which thread can use the
//	decoding processes.
extern
pthread_cond_t		threadCond[NUM_THREADS];


//---		Declaration of classes:					---//

#include	"Message.h"


//---		Declaration of global functions:			---//

//  PURPOSE:  (See decoderServer1.cpp)
extern
bool		didStartDecoders();


//  PURPOSE:  (See decoderServer2.cpp)
extern
void*	      	handleClient	(void*		vPtr
				);