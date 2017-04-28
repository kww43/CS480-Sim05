/*
 * =====================================================================================
 *
 *       Filename:  UploadFile.h
 *
 *    Description:  Header for UploadFile.c
 *
 *        Version:  2.5
 *        Created:  01/29/2017 10:21:38 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 *
 * =====================================================================================
 */

 // Precompiler directives ////////////////////////////////////////////////////////////
 #ifndef UPLOAD_FILE_h
 #define UPLOAD_FILE_h

// Header files ///////////////////////////////////////////////////////////////////////
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "Main.h"

//STRUCTRES DEFINED////////////////////////////////////////////////////////////////////

struct CONFIGFILE
{
    int version;          //The current version of the configuration file
    char *filePath;       //The file path containing the config file
    char *CPUSched;       //The CPU scheduling
    int quantumTime;      //Quantum time
    int memoryAvailable;   //Memory Avalible
    int processTime;      //The Processor cycle time
    int ioTime;           //IO cycle time
    char *logTo;          //log to
    char *logFile;        //The log's file path
};

struct METAFILE
{
  char cmdLetter;         //The command letter
  char *descriptor;       //The command descriptor
  int cycleTime;          //The cycle time
  int cmdOrder;
  struct METAFILE *nextCMD;  //The next command contained in the meta data file
};

struct PROCESSES //The Process control block
{
  int appStart;                  //process start in meta data file
  int currentCMD;
  struct PROCESSES *nextProcess; //The process that follows after it
  int processNumber;             //The ID number of the process
  int totalRunTime;             //The total cycle time for that process
  enum ProcesState {NEW, READY, RUNNING, BLOCKED, EXIT} state; //the state of the process
  int totalProcesses;
  enum PrioritizedState {YES, NO} prioritized;     //Used to if prioritizeProcesses function
  int priority;
};

//FUNCTION PROTOTYPES/////////////////////////////////////////////////////////////////////

/*Main functions*/
void uploadConfig(struct CONFIGFILE *config, FILE *file);
struct METAFILE *uploadMetaData(FILE *file, struct PROCESSES **PCB, int *totalProcesses,
                                struct CONFIGFILE *config);

/*Helper functions*/
int handleInteger(FILE *file);
char* handleString(FILE *file);
char* checkCPUSched(char* CPUSched, FILE *file);
char* checkLogTo(char* logTo, FILE *file);
void displayConfigFile(struct CONFIGFILE *config);
void displayMetaDataFile(struct METAFILE *metaData);
void reverseList(struct METAFILE **metaData);
struct PROCESSES* sortPCB(struct PROCESSES *PCB);
void bubbleSort(struct PROCESSES **start);
void swapNodes(struct PROCESSES **head_ref, int x, int y);


#endif //end define for UPLOAD_FILE_h
