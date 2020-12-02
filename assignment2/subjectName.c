/*-------------------------------------------------------------------------*
 *---									---*
 *---		subjectName.c						---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a		2020 February 2		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

#include	"sleepyProfHeaders.h"


//  PURPOSE:  To hold the names of the academic subjects references by
//	'subject_ty'.
const char*	subjectName[NUM_SUBJECTS]		=
      		{
		  "",
		  "Physics",
		  "Chemistry",
		  "Biology"
		};


//  PURPOSE:  To return the corresponding 'subject_ty' value of the subject
//	named 'subjectNameCPtr', or to return 'NO_SUBJECT' if no match is found.
subject_ty	getSubjectFromName	(const char*	subjectNameCPtr
					)
{
  int i;

  for  (i = (int)MIN_SUBJECT;  i <= (int)MAX_SUBJECT;  i++)
  {
    if  (strcmp(subjectNameCPtr,subjectName[i]) == 0)
    {
      return((subject_ty)i);
    }
  }

  return(NO_SUBJECT);
}
