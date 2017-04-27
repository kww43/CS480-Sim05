/*
 * =====================================================================================
 *
 *       Filename:  Thread.
 *
 *    Description:  Header for Thread.c
 *
 *        Version:  4.0
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 *
 * =====================================================================================
 */

// Precompiler directives /////////////////////////////////////////////////////////////
#ifndef THREAD_H
#define THREAD_H

// Header files ///////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include "Main.h"
#include "UploadFile.h"

//Enumerations defined
typedef enum {SEND_INTERRUPT, CHECK_INTERRUPT} InterruptCodes;

//STRUCTRES DEFINED////////////////////////////////////////////////////////////////////

struct THREAD_INFO
{
  int procID; //The process ID
  double waitTime;
  char cmdLetter;
  char* descriptor;
  double currentTime;
  int totalProcesses;
  char* logTo;
};

/* These structure need to also be defined here for program to compile and run
   with no errors. */
struct PROCESSES;
struct CONFIGFILE;
struct METAFILE;


//FUNCTION PROTOTYPES/////////////////////////////////////////////////////////////////////
void *pause(void *ptr);
int interuptHandler(InterruptCodes type, int procID, int totalProcesses);
void printThread(struct THREAD_INFO *thread);


#endif //End if THREAD_H
