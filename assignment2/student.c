/*-------------------------------------------------------------------------*
 *---									---*
 *---		student.c						---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a		2020 February 2	     Michael Crabtree	---*
 *---									---*
 *-------------------------------------------------------------------------*/

#include	"sleepyProfHeaders.h"

// Hold the number of professor snores
int numSnores = 0;


// Hold the number of student complaints
int numComplaints = 0;

// Var to end process
int isStillClassTime = 1;

// Hold the pid of the dean
pid_t deanPid;

//  PURPOSE:  To make this process stop by setting 'isStillClassTime = 0',
//	and to tell the student process to stop by sending it
//	CLASS_DISMISSED_SIGNAL.  Also prints:
//		"Prof \"Class dismissed!\"\n"
void classDismissedHandler (int sigNum)
{
  printf("Student \"Time for lunch!\"\n");
  isStillClassTime = 0;
}

//  PURPOSE:  To print the lesson, and to send 'PROF_TEACH_SIGNAL' to the
//	student.
void teachHandler (int sigNum)
{
  printf("(Student scribbles notes.)\n");
}


//  PURPOSE:  To send the 'PROF_SNORE_SIGNAL' to the student.
void snoreHandler (int sigNum)
{
  numSnores++;
  if (numSnores < NUM_SNORES_BEFORE_STUDENT_COMPLAINS) {
    printf("Student \"%s?\"\n",PROFS_NAME);
  } else {
    numSnores = 0;
    numComplaints++;

    if (numComplaints < NUM_COMPLAINTS_TO_PROF_BEFORE_COMPLAIN_TO_DEAN) {
      printf("Student \"%s!\"\n",PROFS_NAME);
      kill(getppid(), COMPLAIN_SIGNAL);
    } else {
      numComplaints = 0;
      printf("Student \"I have had enough!  I'm calling the Dean.\"\n");

      kill(deanPid, COMPLAIN_SIGNAL);
    }

   
  }
}

void installSignalHandlers()
{
  struct sigaction act;

  memset(&act,'\0',sizeof(act));
  act.sa_flags = SA_RESTART;
  act.sa_handler= snoreHandler;
  sigaction(PROF_SNORE_SIGNAL,&act,NULL);

  act.sa_handler= teachHandler;
  sigaction(PROF_TEACH_SIGNAL,&act,NULL);

  act.sa_handler= classDismissedHandler;
  sigaction(CLASS_DISMISSED_SIGNAL,&act,NULL);
}

//  PURPOSE:  To do the work of the professor.  Returns 'EXIT_SUCCESS'.
int main (int argc,
	  char*	argv[]
	 )
{
  installSignalHandlers();
 
  if  (argc < 2) {
      printf("Student \"Hey!  You did not tell me the Dean's number so I can complain if I need to\"\n");
      exit(EXIT_FAILURE);
  }
  deanPid = atoi(argv[1]);
   
  while  (isStillClassTime)
  {
    sleep(1);
  }

  return(EXIT_SUCCESS);
}     
