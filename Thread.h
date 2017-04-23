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

/* These structure need to also be defined here for program to compile and run
   with no errors. */
struct PROCESSES;
struct CONFIGFILE;
struct METAFILE;


//FUNCTION PROTOTYPES/////////////////////////////////////////////////////////////////////
void * pause(void *ptr);
void interuptHandler(InterruptCodes type, int procID);


#endif //End if THREAD_H
