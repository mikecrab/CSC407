/*-------------------------------------------------------------------------*
 *---									---*
 *---		decoderServer4.cpp					---*
 *---									---*
 *---	    This file defines the high-level work of a C++ program that	---*
 *---	gets cipher text from a client over a socket, decrypts it,	---*
 *---	and sends it back to the client.				---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a		2020 March 15		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

//---		Header file inclusion					---//

#include	"decoderServer.h"


//---		Definition of functions:				---//

//  PURPOSE:  To decide a port number, either from the command line arguments
//	'argc' and 'argv[]', or by asking the user.  Returns port number.
int		getPortNum	(int	argc,
				 char*	argv[]
				)
{
  //  I.  Application validity check:

  //  II.  Get listening socket:
  int	portNum;

  if  (argc >= 2)
    portNum	= strtol(argv[1],NULL,0);
  else
  {
    char	buffer[BUFFER_LEN];

    printf("Port number to monopolize? ");
    fgets(buffer,BUFFER_LEN,stdin);
    portNum	= strtol(buffer,NULL,0);
  }

  //  III.  Finished:  
  return(portNum);
}


//  PURPOSE:  To attempt to create and return a file-descriptor for listening
//	to the OS telling this server when a client process has connect()-ed
//	to 'port'.  Returns that file-descriptor, or 'ERROR_FD' on failure.
int		getServerFileDescriptor
				(int		port
				)
{
  //  I.  Application validity check:

  //  II.  Attempt to get socket file descriptor and bind it to 'port':
  //  II.A.  Create a socket
  int socketDescriptor = socket(AF_INET, // AF_INET domain
			        SOCK_STREAM, // Reliable TCP
			        0);

  if  (socketDescriptor < 0)
  {
    perror("socket");
    return(ERROR_FD);
  }

  //  II.B.  Attempt to bind 'socketDescriptor' to 'port':
  //  II.B.1.  We'll fill in this datastruct
  struct sockaddr_in socketInfo;

  //  II.B.2.  Fill socketInfo with 0's
  memset(&socketInfo,'\0',sizeof(socketInfo));

  //  II.B.3.  Use TCP/IP:
  socketInfo.sin_family = AF_INET;

  //  II.B.4.  Tell port in network endian with htons()
  socketInfo.sin_port = htons(port);

  //  II.B.5.  Allow machine to connect to this service
  socketInfo.sin_addr.s_addr = INADDR_ANY;

  //  II.B.6.  Try to bind socket with port and other specifications
  int status = bind(socketDescriptor, // from socket()
		    (struct sockaddr*)&socketInfo,
		    sizeof(socketInfo)
		   );

  if  (status < 0)
  {
    perror("bind");
    return(ERROR_FD);
  }

  //  II.B.6.  Set OS queue length:
  listen(socketDescriptor,5);

  //  III.  Finished:
  return(socketDescriptor);
}


//  PURPOSE:  To clean up *everything* that ought to be cleaned up.
//	'socketFd' and 'clientFd' are two file descriptors that are no longer
//	needed.  No return value.
void		cleanUp		(int		socketFd,
				 int		clientFd
				)
{
  //  I.  Application validity check:

  //  II.  Clean up:

  //  YOUR CODE HERE
  close(socketFd);
  close(clientFd);
  close(toParentFd);
  close(toDecodeFd);
  pthread_mutex_destroy(&lock);
  pthread_cond_destroy(&threadCond[2]);
 
  //  III.  Finished:
}


int		main		(int	argc,
				 char*	argv[]
				)
{
  //  I.  Application validity check:

  //  II.  Do server:
  //  II.A.  Bind port and get listening socket fd:
  int	port		= getPortNum(argc,argv);
  int	listenFd	= getServerFileDescriptor(port);

  if   (listenFd < 0)
  {
    exit(EXIT_FAILURE);
  }

  //  II.B.  Attempt to start decoders:
  if  ( !didStartDecoders() )
  {
    close(listenFd);
    exit(EXIT_FAILURE);
  }

  //  II.C.  Set up for threads:
  pthread_mutex_init(&lock,NULL);

  for  (int threadNum = 0;  threadNum < NUM_THREADS;  threadNum++)
  {
    pthread_cond_init(&threadCond[threadNum],NULL);
  }

  //  II.D.  Get and decode message:
  pthread_t	threadId[NUM_THREADS];
  ForThreadInfo	threadInfoArray[NUM_THREADS];
  int		clientFd    = accept(listenFd,NULL,NULL);
  Message	message(clientFd);

  for  (int threadNum = 0;  threadNum < NUM_THREADS;  threadNum++)
  {
    threadInfoArray[threadNum].messagePtr_	= &message;
    threadInfoArray[threadNum].threadNum_	= threadNum;
    pthread_create
		(&threadId[threadNum],
		 NULL,
		 handleClient,
		 threadInfoArray+threadNum
		);
  }

  for  (int threadNum = 0;  threadNum < NUM_THREADS;  threadNum++)
  {
    pthread_join(threadId[threadNum],NULL);
  }

  message.send();

  //  II.E.  Clean up:
  cleanUp(clientFd,listenFd);

  //  III.  Finished:
  return(EXIT_SUCCESS);
}
