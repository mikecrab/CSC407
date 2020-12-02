/*-------------------------------------------------------------------------*
 *---									---*
 *---		sleepyProfHeaders.h					---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
*---	Version 1a		2020 February 2		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<signal.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/wait.h>


//  PURPOSE:  To tell the length string arrays.
#define		MAX_LINE				64

//  PURPOSE:  To tell the name of the professor.
#define		PROFS_NAME				"Prof Narcolepsy"

//  PURPOSE:  To tell the name of the program the professor process should run.
#define		PROF_PROGRAM				"prof"

//  PURPOSE:  To tell the name of the program the student process should run.
#define		STUDENT_PROGRAM				"student"


//  PURPOSE:  To help determine how long a class period is.
#define		NUM_MOMENTS_IN_CLASS			90


//  PURPOSE:  To help determine how long a class period is.
#define		SECONDS_PER_MOMENT			1

//  PURPOSE:  To tell the probability that the professor will fall asleep in a
//	moment.
#define		PROF_FALL_ASLEEP_PROB			0.125


//  PURPOSE:  To tell the number of sleeping moments that must occur before the
//	student complains.
#define		NUM_SNORES_BEFORE_STUDENT_COMPLAINS	3


//  PURPOSE:  To tell the number of complaints to the professor that must occur
//	before the student complains to the dean.
#define		NUM_COMPLAINTS_TO_PROF_BEFORE_COMPLAIN_TO_DEAN	\
							3

//  PURPOSE:  What the professor sends to the student to teach him or her.
#define		PROF_TEACH_SIGNAL			SIGUSR1


//  PURPOSE:  What the professor sends to the student to say that he or she is
//	sleeping.
#define		PROF_SNORE_SIGNAL			SIGUSR2


//  PURPOSE:  What the student sends to the professor and dean to complain about
//	the sleeping prof.
#define		COMPLAIN_SIGNAL				SIGUSR1


//  PURPOSE:  What the dean sends to the professor, and the professor to the
//	student, to say the class is over.
#define		CLASS_DISMISSED_SIGNAL			SIGINT


//  PURPOSE:  To distinguish among the academic topics the professor can teach.
typedef		enum
		{
		  NO_SUBJECT,
		  PHYSICS_SUBJECT,
		  CHEMISTRY_SUBJECT,
		  BIOLOGY_SUBJECT,

		  MIN_SUBJECT	= PHYSICS_SUBJECT,
		  MAX_SUBJECT	= BIOLOGY_SUBJECT,

		  NUM_SUBJECTS
		}
		subject_ty;


//  PURPOSE:  To hold the names of the academic subjects references by
//	'subject_ty'.
extern
const char*	subjectName[NUM_SUBJECTS];


//  PURPOSE:  To return the corresponding 'subject_ty' value of the subject
//	named 'subjectNameCPtr', or to return 'NO_SUBJECT' if no match is found.
extern
subject_ty	getSubjectFromName	(const char*	subjectNameCPtr
					);
