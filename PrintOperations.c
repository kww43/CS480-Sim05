/*
 * =====================================================================================
 *
 *       Filename:  PrintOperations.h
 *
 *    Description:  Handles the printing for the operating system simulator
 *
 *        Version:  2.0
 *        Created:  02/04/2017 09:27:30 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */

 #ifndef PRINT_c
 #define PRINT_c

//Header Files////////////////////////////////////////////////////////////////////
#include "PrintOperations.h"

 /*
  * Name       : printTo
  * Description: Prints to the correct log path (monitor/file)
  * Parameters : config - The configuration file
  *              logToFile - the file handle to write to
  *              timeOfDay - the time to write out
  *              processNum - The process's ID number
  *              metaData   - The meta data file
  *              stringType - The type of string to write out to the file
  */
void printTo(struct CONFIGFILE *config, FILE* logToFile, double timeOfDay,
           int processNum, struct METAFILE *currentMetaData, int stringType)
{
if(strcmp(config->logTo, "Monitor") == 0)
{
  printMonitor(timeOfDay, processNum, currentMetaData, stringType);
}
else if(strcmp(config->logTo, "File") == 0)
{
  printFile(logToFile, timeOfDay, processNum, currentMetaData, stringType);
}
else if(strcmp(config->logTo, "Both") == 0)
{
  printMonitor(timeOfDay, processNum, currentMetaData, stringType);
  printFile(logToFile, timeOfDay, processNum, currentMetaData, stringType);
}
}

/*
 * Name       : printFile
 * Description: Prints to the log file when called.
 * Parameters : logTo - the file handle to write to
 *              timeOfDay - the time to write out
 *              processNum - The process's ID number
 *              metaData   - The meta data file
 *              stringType - The type of string to write out to the file
 */
void printFile(FILE* logTo, double timeOfDay, int processNum, struct METAFILE *metaData, int stringType)
{
  if(stringType == 0)
  {
    fprintf(logTo, "Time: %.6lf, System Start\n", timeOfDay);
  }
  else if(stringType == 1)
  {
    fprintf(logTo, "Time: %.6lf, Begin PCB Creation\n", timeOfDay);
  }
  else if(stringType == 2)
  {
    fprintf(logTo, "Time: %.6lf, All Processes set in New state\n", timeOfDay);
  }
  else if(stringType == 3)
  {
    fprintf(logTo, "Time: %.6lf, All Processes set in Ready state\n", timeOfDay);
  }
  else if(stringType == 4)
  {
    fprintf(logTo, "Time: %.6lf, Process %d set in Running state\n", timeOfDay, processNum);
  }
  else if(stringType == 5)
  {
    fprintf(logTo, "Time: %.6lf, Process %d %s output start\n", timeOfDay, processNum, metaData->descriptor);
  }
  else if (stringType == 6)
  {
    fprintf(logTo, "Time: %.6lf, Process %d %s input start\n", timeOfDay, processNum, metaData->descriptor);
  }
  else if(stringType == 7)
  {
    fprintf(logTo, "Time: %.6lf, Process %d %s output end\n", timeOfDay, processNum, metaData->descriptor);
  }
  else if(stringType == 8)
  {
   fprintf(logTo, "Time: %.6lf, Process %d %s input end\n", timeOfDay, processNum, metaData->descriptor);
  }
  else if(stringType == 9)
  {
    fprintf(logTo, "Time: %.6lf, Process %d Processer %s Start\n", timeOfDay, processNum, metaData->descriptor);
  }
  else if(stringType == 10)
  {
    fprintf(logTo,"Time: %.6lf, Process %d Application %s \n", timeOfDay, processNum,
          metaData->descriptor);
  }
  else if(stringType == 11)
  {
    fprintf(logTo, "Time: %.6lf, Process %d Processer %s End\n", timeOfDay, processNum, metaData->descriptor);
  }
  else if(stringType == 12)
  {
    fprintf(logTo, "Time: %.6lf, Process %d Memory management %s action\n", timeOfDay, processNum,
          metaData->descriptor);
  }
  else if(stringType == 13)
  {
    fprintf(logTo, "Time: %.6lf, System Stop\n", timeOfDay);
  }
  else if(stringType == 14)
  {
    fprintf(logTo, "Time: %.6lf, Process %d set in Exit state.\n", timeOfDay, processNum);
  }
  else if(stringType == 15)
  {
    fprintf(logTo, "Time: %.6lf, Process %d set in Blocked state.\n", timeOfDay, processNum);
  }
}

/*
 * Name       : printMonitor
 * Description: Prints out to the monitor.
 * Parameters : timeOfDay - the time to write out
 *              processNum - The process's ID number
 *              metaData   - The meta data file
 *              stringType - The type of string to print out to the monitor
 */
void printMonitor(double timeOfDay, int processNum, struct METAFILE *metaData, int stringType)
{
  if(stringType == 0)
  {
    printf("Time: %.6lf, System Start\n", timeOfDay);
  }
  else if(stringType == 1)
  {
    printf("Time: %.6lf, Begin PCB Creation\n", timeOfDay);
  }
  else if(stringType == 2)
  {
    printf("Time: %.6lf, All Processes set in New state\n", timeOfDay);
  }
  else if(stringType == 3)
  {
    printf("Time: %.6lf, All Processes set in Ready state\n", timeOfDay);
  }
  else if(stringType == 4)
  {
    printf("Time: %.6lf, Process %d set in Running state\n", timeOfDay, processNum);
  }
  else if(stringType == 5)
  {
    printf("Time: %.6lf, Process %d %s output start\n", timeOfDay, processNum, metaData->descriptor);
  }
  else if (stringType == 6)
  {
    printf("Time: %.6lf, Process %d %s input start\n", timeOfDay, processNum, metaData->descriptor);
  }
  else if(stringType == 7)
  {
    printf("Time: %.6lf, Process %d %s output end\n", timeOfDay, processNum, metaData->descriptor);
  }
  else if(stringType == 8)
  {
    printf("Time: %.6lf, Process %d %s input end\n", timeOfDay, processNum, metaData->descriptor);
  }
  else if(stringType == 9)
  {
    printf("Time: %.6lf, Process %d Processer %s Start\n", timeOfDay, processNum, metaData->descriptor);
  }
  else if(stringType == 10)
  {
    printf("Time: %.6lf, Process %d Application %s \n", timeOfDay, processNum,
          metaData->descriptor);
  }
  else if(stringType == 11)
  {
    printf("Time: %.6lf, Process %d Processer %s End\n", timeOfDay, processNum, metaData->descriptor);
  }
  else if(stringType == 12)
  {
    printf("Time: %.6lf, Process %d Memory management %s action\n", timeOfDay, processNum,
          metaData->descriptor);
  }
  else if(stringType == 13)
  {
    printf("Time: %.6lf, System Stop\n", timeOfDay);
  }
  else if(stringType == 14)
  {
    printf("Time: %.6lf, Process %d set in Exit state.\n", timeOfDay, processNum);
  }
  else if(stringType == 15)
  {
    printf("Time: %.6lf, Process %d set in Blocked state.\n", timeOfDay, processNum);
  }
}

#endif
