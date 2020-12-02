#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<signal.h>	// For waitpid(), opendir()

const int	BUFFER_LEN	= 1024;

const int	KEY		= 3;

int		shouldRun	= 1;

void		sigIntHandler	(int		sigNum
				)
{
  shouldRun	= 0;
}


void  		swap		(char*		cPtr0,
				 char*		cPtr1
				)
{
  char	c	= *cPtr0;

  *cPtr0	= *cPtr1;
  *cPtr1	= c;
}


int		main		(int		argc,
				 char*		argv[]
				)
{
  struct sigaction	act;

  memset(&act,'\0',sizeof(act));
  act.sa_handler	= sigIntHandler;
  sigaction(SIGINT,&act,NULL);

  while  (shouldRun)
  {
    char		line[BUFFER_LEN];
    unsigned char*	cPtr;
    char*    		begin;
    char*    		end;
    int			haveNewline	= 0;
    int			numBytes;

    if  (fgets(line,BUFFER_LEN,stdin) == NULL)
      break;

    for  (cPtr = (unsigned char*)line;  *cPtr != '\0' && *cPtr != '\n';  cPtr++)
    {
      int		chr	= *cPtr;

      *cPtr = (chr - KEY) % 256;
    }

    for  (end = line;  *end != '\0' && *end != '\n';  end++);

    if  (*end == '\n')
    {
      haveNewline	= 1;
      end--;
    }

    for  (begin = line;  begin < end;  begin++, end--)
    {
      swap(begin,end);
    }

    numBytes	= printf( (haveNewline ? "%s" : "%s\n"),line);
    fflush(stdout);
  }

  return(EXIT_SUCCESS);
}
