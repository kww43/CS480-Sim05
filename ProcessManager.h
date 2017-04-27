/*
 * =====================================================================================
 *
 *       Filename:  ProcessManager.h
 *
 *    Description:  Header file for ProcessManager.c
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
#ifndef PROCESS_MANAGER_h
#define PROCESS_MANAGER_h

// Header files ///////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include "Main.h"
#include "UploadFile.h"
#include "Thread.h"

//STRUCTRES DEFINED////////////////////////////////////////////////////////////////////

/* These structure need to also be defined here for program to compile and run
   with no errors. */
struct PROCESSES;
struct CONFIGFILE;
struct METAFILE;

//FUNCTION PROTOTYPES/////////////////////////////////////////////////////////////////////

/* Main functions for ProcessManager.c/h */
void processManager(struct CONFIGFILE *config, struct METAFILE *metaData, struct PROCESSES *PCB, int totalProcesses);
void processFCFS_P(struct CONFIGFILE *config, struct METAFILE *metaData, struct PROCESSES *PCB, int totalProcesses);
void processSRTF_P(struct CONFIGFILE *config, struct METAFILE *metaData, struct PROCESSES *PCB, int totalProcesses);
void processRR_P(struct CONFIGFILE *config, struct METAFILE *metaData, struct PROCESSES *PCB, int totalProcesses);

/* Helper Function for ProcessManager.c/h */
void printTo(struct CONFIGFILE *config, FILE* logToFile, double timeOfDay,
             int processNum, struct METAFILE *currentMetaData, int stringType);
void printFile(FILE* logTo, double timeOfDay, int processNum, struct METAFILE *metaData, int stringType);
void printMonitor(double timeOfDay, int processNum, struct METAFILE *metaData, int stringType);
struct METAFILE* waitTillCMD(struct METAFILE *headMetaData, int cmdNumber);
struct PROCESSES* setProcessToReady(struct PROCESSES *headPCB, int cmdNumber);

#endif //end define for PROCESS_MANAGER_h
