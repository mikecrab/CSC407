/*-------------------------------------------------------------------------*
 *---									---*
 *---		decoderClient.cpp					---*
 *---									---*
 *---	    This file defines a C program that gets file-sys commands	---*
 *---	from the user, and sends them to a server via a socket, waits	---*
 *---	for a reply, and outputs the response to the user.		---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a					Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

//	Compile with:
//	$ g++ decoderClient.cpp -o decoderClient

//---		Header file inclusion					---//

#include	"decoderServer.h"


//---		Definition of constants:				---//

#define		DEFAULT_HOSTNAME	"localhost"

const char*	ENCRYPTED_TEXT_ARRAY[]
		= { "=<40glyrF#prui#hidv#|dwv#rw#zrK\n",
		    "######|owqhxthui#vgqdk#kvdZ#,4+\n",
		    "#|dzd#,whhi#6+#uhwhp#4#|dwV#,5+\n",
		    "zreoh#hkw#rwql#h}hhqv2kjxrF#,6+\n",
		    "#####hfdi#uxr|#kfxrw#wrq#rG#,7+\n",
		    "#hprk#|dwv#/gde#ohhi#xr|#iL#,8+\n"
		  };
//     		= { "How to stay safe from Covid-19:\n",
//		    "(1) Wash hands frequently      \n",
//		    "(2) Stay 1 meter (3 feet) away \n",
//		    "(3) Cough/sneeze into the elbow\n",
//		    "(4) Do not touch your face     \n",
//		    "(5) If you feel bad, stay home \n"
//		  };

const int	TEXT_ARRAY_LEN	 = sizeof(ENCRYPTED_TEXT_ARRAY)
				   / sizeof(const char*);


//---		Definition of functions:				---//

//  PURPOSE:  To ask the user for the name and the port of the server.  The
//	server name is returned in 'url' up to length 'urlLen'.  The port
//	number is returned in '*portPtr'.  No return value.
void	obtainUrlAndPort	(int		urlLen,
				 char*		url,
				 int*		portPtr
				)
{
  //  I.  Application validity check:
  if  ( (url == NULL)  ||  (portPtr == NULL) )
  {
    fprintf(stderr,"BUG: NULL ptr to obtainUrlAndPort()\n");
    exit(EXIT_FAILURE);
  }

  if   (urlLen <= 1)
  {
    fprintf(stderr,"BUG: Bad string length to obtainUrlAndPort()\n");
    exit(EXIT_FAILURE);
  }

  //  II.  Get server name and port number:
  //  II.A.  Get server name:
  printf("Machine name [%s]? ",DEFAULT_HOSTNAME);
  fgets(url,urlLen,stdin);

  char*	cPtr	= strchr(url,'\n');

  if  (cPtr != NULL)
    *cPtr = '\0';

  if  (url[0] == '\0')
    strncpy(url,DEFAULT_HOSTNAME,urlLen);

  //  II.B.  Get port numbe:
  char	buffer[BUFFER_LEN];

  printf("Port number? ");
  fgets(buffer,BUFFER_LEN,stdin);

  *portPtr = strtol(buffer,NULL,10);

  //  III.  Finished:
}


//  PURPOSE:  To attempt to connect to the server named 'url' at port 'port'.
//	Returns file-descriptor of socket if successful, or '-1' otherwise.
int	attemptToConnectToServer	(const char*	url,
					 int		port
					)
{
  //  I.  Application validity check:
  if  (url == NULL)
  {
    fprintf(stderr,"BUG: NULL ptr to attemptToConnectToServer()\n");
    exit(EXIT_FAILURE);
  }


  //  II.  Attempt to connect to server:
  //  II.A.  Create a socket:
  int socketDescriptor = socket(AF_INET, // AF_INET domain
				SOCK_STREAM, // Reliable TCP
				0);

  //  II.B.  Ask DNS about 'url':
  struct addrinfo* hostPtr;
  int status = getaddrinfo(url,NULL,NULL,&hostPtr);

  if (status != 0)
  {
    fprintf(stderr,gai_strerror(status));
    return(-1);
  }

  //  II.C.  Attempt to connect to server:
  struct sockaddr_in server;
  // Clear server datastruct
  memset(&server, 0, sizeof(server));

  // Use TCP/IP
  server.sin_family = AF_INET;

  // Tell port # in proper network byte order
  server.sin_port = htons(port);

  // Copy connectivity info from info on server ("hostPtr")
  server.sin_addr.s_addr =
	((struct sockaddr_in*)hostPtr->ai_addr)->sin_addr.s_addr;

  status = connect(socketDescriptor,(struct sockaddr*)&server,sizeof(server));

  if  (status < 0)
  {
    fprintf(stderr,"Could not connect %s:%d\n",url,port);
    return(-1);
  }

  //  III.  Finished:
  return(socketDescriptor);
}


//  PURPOSE:  To do the work of the application.  Gets letter from user, sends
//	it to server over file-descriptor 'socketFd', and prints returned text.
//	No return value.
void		communicateWithServer
				(int		socketFd
				)
{
  //  I.  Application validity check:

  //  II.  Do work of application:
  //  II.A.  Send text to server:
  int textInd;

  for  (textInd = 0;  textInd < TEXT_ARRAY_LEN;  textInd++)
  {
    const char*	textPtr	= ENCRYPTED_TEXT_ARRAY[textInd];

    write(socketFd,textPtr,strlen(textPtr));
    printf("Sending: %s",textPtr);
  }

  //  II.B.  Get response from server:
  char	buffer[BUFFER_LEN+1];
  int	numBytes;
  int	numNewlines	= 0;
  char*	bufferPtr	= buffer;

  for  (textInd = 0;  textInd < TEXT_ARRAY_LEN;  textInd++)
  {
    numBytes	 = read(socketFd,buffer,BUFFER_LEN);

    buffer[numBytes]	 = '\0';
    printf("%s",buffer);
  }

  //  III.  Finished:
}


//  PURPOSE:  To do the work of the client.  Ignores command line parameters.
//	Returns 'EXIT_SUCCESS' to OS on success or 'EXIT_FAILURE' otherwise.
int	main	()
{
  char		url[BUFFER_LEN];
  int		port;
  int		socketFd;

  obtainUrlAndPort(BUFFER_LEN,url,&port);
  socketFd	= attemptToConnectToServer(url,port);

  if  (socketFd < 0)
    exit(EXIT_FAILURE);

  communicateWithServer(socketFd);
  close(socketFd);
  return(EXIT_SUCCESS);
}
