/*-------------------------------------------------------------------------*
 *---									---*
 *---		dean.c							---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a		2020 February 2		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

#include	"sleepyProfHeaders.h"

//  PURPOSE:  To hold the process id of the professor (child) process.
pid_t		profPid;

//  PURPOSE:  To hold '1' while the professor process is running.
int		isProfRunning	= 0;


//  PURPOSE:  To ask the user which subject they want to study, and to return
//	the corresponding 'subject_ty' value.  No parameters.
subject_ty	getSubject	()
{
  int	i;
  int	subject;
  char	text[MAX_LINE];

  printf("Dean \"Welcome to the Giggles University!\"\n");

  do
  {
    printf("Dean \"What would you like to study?\"\n");

    for  (i = (int)MIN_SUBJECT;  i <= (int)MAX_SUBJECT;  i++)
    {
      printf("  %d: %s\n",i,subjectName[i]);
    }

    fgets(text,MAX_LINE,stdin);
    subject = strtol(text,NULL,0);
  }
  while  ( (subject < MIN_SUBJECT) || (subject > MAX_SUBJECT) );

  return((subject_ty)subject);
}


//  PURPOSE:  To tell the professor to wake up.  Ignores 'sigNum'.  No return
//	value.
void		complainSigHandler
				(int	sigNum
				)
{
  printf("Dean \"WAKE UP!\"\n");
  kill(profPid,COMPLAIN_SIGNAL);
}


//  PURPOSE:  To handle when the professor process finishes.  Ignores 'sigNum'.
//	No return value.
void		sigChildHandler	(int	sigNum
				)
{
  int	status;

  wait(&status);

  if  ( WIFEXITED(status) && (WEXITSTATUS(status) == EXIT_SUCCESS) )
  {
    printf("(The Dean turns of the lights and locks the classroom.)\n");
  }
  else
  {
    printf("Dean \"Whoops!  I will look into that.\"\n");
  }

  isProfRunning	= 0;
}


//  PURPOSE:  To install 'complainSigHandler()' as the signal handler to handle
//	'COMPLAIN_SIGNAL' coming to the dean.  No parameters.  No return value.
void		installSignalHandlers
				()
{
  struct sigaction	act;

  memset(&act,'\0',sizeof(act));
  act.sa_handler	= complainSigHandler;
  sigaction(COMPLAIN_SIGNAL,&act,NULL);

  act.sa_handler	= sigChildHandler;
  sigaction(SIGCHLD,&act,NULL);
}


//  PURPOSE:  To start the professor process (and initialize 'profPid'), and
//	to have the professor process teach 'subject'
void		startProf	(subject_ty	subject
				)
{
  char	text[MAX_LINE];

  printf("Dean: \"%s, today you will teach %s\"\n",
  	 PROFS_NAME,subjectName[subject]
	);

  isProfRunning	= 1;
  profPid	= fork();

  if  (profPid == 0)
  {
    execl(PROF_PROGRAM,PROF_PROGRAM,subjectName[subject],NULL);
    printf("Prof \"I cannot find my classroom!  I quit!\"\n");
    exit(EXIT_FAILURE);
  }
  
}


//  PURPOSE:  To wait for either the class to be over (more specifically, for
//	'NUM_MOMENTS_IN_CLASS' * 'SECONDS_PER_MOMENT' seconds to elapse), or
//	to be notified that the professor process no longer runs.  No
//	parameters.  No return value.
void		waitUntilClassPeriodIsOver
				()
{
  int	momentNum;

  for  ( momentNum = 0;  momentNum < NUM_MOMENTS_IN_CLASS;  momentNum++ )
  {
    sleep(SECONDS_PER_MOMENT);

    if  (!isProfRunning)
    {
      break;
    }

    if  ( (momentNum % 5) == 0 )
    {
      printf("(The Dean shuffles paper.)\n");
    }
  }

}


//  PURPOSE:  To inform the professor process that class is over, and to wait
//	to be notified by the OS that it has finished.  No parameters.  No
//	return value.
void		informProfClassIsOver	()
{
  printf("Dean \"Class is over!\"\n");
  kill(profPid,CLASS_DISMISSED_SIGNAL);

  while  (isProfRunning)
    sleep(1);
}


//  PURPOSE:  To run the program.  Ignores command line arguments.  Returns
//	'EXIT_SUCCESS' to OS.
int		main		()
{
  int	subject	= getSubject();

  installSignalHandlers();
  startProf(subject);
  waitUntilClassPeriodIsOver();

  if  (isProfRunning)
  {
    informProfClassIsOver();
  }

  return(EXIT_SUCCESS);
}
