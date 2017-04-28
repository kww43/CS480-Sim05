/*
 * =====================================================================================
 *
 *       Filename:  NonPremptive.h
 *
 *    Description:  Header file
 *
 *        Version:  2.0
 *        Created:  02/04/2017 09:27:30 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */

// Precompiler directives //////////////////////////////////////////////////////////////
#ifndef NON_P_h
#define NON_P_h

// Header files ///////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include "Main.h"
#include "UploadFile.h"
#include "Thread.h"
#include "ProcessManager.h"

//STRUCTRES DEFINED////////////////////////////////////////////////////////////////////

/* These structure need to also be defined here for program to compile and run
   with no errors. */
struct PROCESSES;
struct CONFIGFILE;
struct METAFILE;

//FUNCTION PROTOTYPES/////////////////////////////////////////////////////////////////////

/* Main functions for ProcessManager.c/h */
void processManager(struct CONFIGFILE *config, struct METAFILE *metaData, struct PROCESSES *PCB, int totalProcesses);

//Helper functions
void* wait(void *ptr);

#endif //end define for PROCESS_MANAGER_h
