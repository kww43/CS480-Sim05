/*
 * =====================================================================================
 *
 *       Filename:  PrintOperations.h
 *
 *    Description:  Header file for PrintOperations.c
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
#ifndef PRINT_h
#define PRINT_h

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

/* Helper Function for ProcessManager.c/h */
void printTo(struct CONFIGFILE *config, FILE* logToFile, double timeOfDay,
             int processNum, struct METAFILE *currentMetaData, int stringType);
void printFile(FILE* logTo, double timeOfDay, int processNum, struct METAFILE *metaData, int stringType);
void printMonitor(double timeOfDay, int processNum, struct METAFILE *metaData, int stringType);

#endif
