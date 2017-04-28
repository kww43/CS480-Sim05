/*
 * =====================================================================================
 *
 *       Filename:  ProcessManager.c
 *
 *    Description:  Process manager for the operating system simulator
 *
 *        Version:  2.0
 *        Created:  02/4/2017 09:30:50 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */


// Precompiler directives //////////////////////////////////////////////////////////////
#ifndef NON_P_c
#define NON_P_c

// Header files ////////////////////////////////////////////////////////////////////////
#include "NonPremptive.h"

//Functions/////////////////////////////////////////////////////////////////////////////

/*
* ===  FUNCTION  ======================================================================
*         Name:  processManager
*  Description: Handles processes and their meta data (operating system simulator)
*
*  Parameters : config - the structure that contains the configuration file
*               metaData - The meta data for the  process
*               PCB - the process control block (contains all the processes and their data)
*               totalProcesses - The total number of processes in the PCB
* ======================================================================================
*/
void processManager(struct CONFIGFILE *config, struct METAFILE *metaData, struct PROCESSES *PCB, int totalProcesses)
{
  //Variables///////////////////////////////////////////////////////////////////
  struct METAFILE *currentMetaData = metaData; //The current meta data operation
  struct METAFILE *headMetaData = metaData;
  double timeForCycle;  //The cycle time for meta data OP * processor/io time
  double timeOfDay;     //The timer for the simulator
  struct PROCESSES *headPCB = PCB; //Start of processes for the PCB
  struct PROCESSES *currentPCB = PCB; //Current process for the PCB
  FILE* logToFile = fopen(config->logFile ,"w"); //The log file handle to write to
  pthread_t threadIO; //IO Thread
  pthread_t threadP; //Processor Thread

  //Intialize the time and start the system
  timeOfDay = 0.000000;
  if(strcmp(config->logTo, "Monitor") == 0)
  {
    printMonitor(timeOfDay, 0, currentMetaData, 0);
  }
  else if(strcmp(config->logTo, "File") == 0)
  {
    printFile(logToFile, timeOfDay, 0, currentMetaData, 0);
  }
  else if(strcmp(config->logTo, "Both") == 0)
  {
    printMonitor(timeOfDay, 0, currentMetaData, 0);
    printFile(logToFile, timeOfDay, 0, currentMetaData, 0);
  }

  //Create the process
  timeOfDay += 0.000005;
  if(strcmp(config->logTo, "Monitor") == 0)
  {
    printMonitor(timeOfDay, 0, currentMetaData, 1);
  }
  else if(strcmp(config->logTo, "File") == 0)
  {
    printFile(logToFile, timeOfDay, 0, currentMetaData, 1);
  }
  else if(strcmp(config->logTo, "Both") == 0)
  {
    printMonitor(timeOfDay, 0, currentMetaData, 1);
    printFile(logToFile, timeOfDay, 0, currentMetaData, 1);
  }


  //Set Processes to new state
  timeOfDay += 0.000004;
  while(currentPCB != NULL)
  {
    currentPCB->state = NEW;
    currentPCB = currentPCB->nextProcess;
  }
  currentPCB = headPCB;
  //Print the result of setting processes to new state
  if(strcmp(config->logTo, "Monitor") == 0)
  {
    printMonitor(timeOfDay, 0, currentMetaData, 2);
  }
  else if(strcmp(config->logTo, "File") == 0)
  {
    printFile(logToFile, timeOfDay, 0, currentMetaData, 2);
  }
  else if(strcmp(config->logTo, "Both") == 0)
  {
    printMonitor(timeOfDay, 0, currentMetaData, 2);
    printFile(logToFile, timeOfDay, 0, currentMetaData, 2);
  }



  //Set Processes to Ready state
  timeOfDay += 0.000002;
  while(currentPCB != NULL)
  {
    currentPCB->state = READY;
    currentPCB = currentPCB->nextProcess;
  }
  currentPCB = headPCB;
  //Print the result of setting processes to ready state
  if(strcmp(config->logTo, "Monitor") == 0)
  {
    printMonitor(timeOfDay, 0, currentMetaData, 3);
  }
  else if(strcmp(config->logTo, "File") == 0)
  {
    printFile(logToFile, timeOfDay, 0, currentMetaData, 3);
  }
  else if(strcmp(config->logTo, "Both") == 0)
  {
    printMonitor(timeOfDay, 0, currentMetaData, 3);
    printFile(logToFile, timeOfDay, 0, currentMetaData, 3);
  }


  //Set a Process to Running state
  timeOfDay += 0.000006;
  currentPCB->state = RUNNING;
  if(strcmp(config->logTo, "Monitor") == 0)
  {
    printMonitor(timeOfDay, 0, currentMetaData, 4);
  }
  else if(strcmp(config->logTo, "File") == 0)
  {
    printFile(logToFile, timeOfDay, 0, currentMetaData, 4);
  }
  else if(strcmp(config->logTo, "Both") == 0)
  {
    printMonitor(timeOfDay, 0, currentMetaData, 4);
    printFile(logToFile, timeOfDay, 0, currentMetaData, 4);
  }

  currentMetaData = waitTillCMD(headMetaData, currentPCB->appStart);
  //Handle meta data for process
  while(currentPCB != NULL)
  {
    //Handle I/O operation
    if(currentMetaData->cmdLetter == 'O' || currentMetaData->cmdLetter == 'I')
    {
      //Print out start of Output Operation
      if(currentMetaData->cmdLetter == 'O')
      {
        if(strcmp(config->logTo, "Monitor") == 0)
        {
          printMonitor(timeOfDay, currentPCB->processNumber, currentMetaData, 5);
        }
        else if(strcmp(config->logTo, "File") == 0)
        {
          printFile(logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 5);
        }
        else if(strcmp(config->logTo, "Both") == 0)
        {
          printMonitor(timeOfDay, currentPCB->processNumber, currentMetaData, 5);
          printFile(logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 5);
        }
      }

      //Print out start of Input Operation
      else if(currentMetaData->cmdLetter == 'I')
      {
        if(strcmp(config->logTo, "Monitor") == 0)
        {
          printMonitor(timeOfDay, currentPCB->processNumber, currentMetaData, 6);
        }
        else if(strcmp(config->logTo, "File") == 0)
        {
          printFile(logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 6);
        }
        else if(strcmp(config->logTo, "Both") == 0)
        {
          printMonitor(timeOfDay, currentPCB->processNumber, currentMetaData, 6);
          printFile(logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 6);
        }
      }

      /* Get the time to wait for based on meta data operation cycle time * the
         config file's I/O cycle time */
      timeForCycle = ((double) currentMetaData->cycleTime * (double) config->ioTime) / 100000;

      //Wait for time found from cycle time * I/O cycle time
      pthread_create(&threadIO, NULL, (void *) &wait, (void *) &timeForCycle);
      pthread_join(threadIO, NULL);

      timeOfDay += timeForCycle;

      //Print out Output operation end
      if(currentMetaData->cmdLetter == 'O')
      {
        if(strcmp(config->logTo, "Monitor") == 0)
        {
          printMonitor(timeOfDay, currentPCB->processNumber, currentMetaData, 7);
        }
        else if(strcmp(config->logTo, "File") == 0)
        {
          printFile(logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 7);
        }
        else if(strcmp(config->logTo, "Both") == 0)
        {
          printMonitor(timeOfDay, currentPCB->processNumber, currentMetaData, 7);
          printFile(logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 7);
        }
      }

      //Print out Input operation end
      else if(currentMetaData->cmdLetter == 'I')
      {
        if(strcmp(config->logTo, "Monitor") == 0)
        {
          printMonitor(timeOfDay, currentPCB->processNumber, currentMetaData, 8);
        }
        else if(strcmp(config->logTo, "File") == 0)
        {
          printFile(logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 8);
        }
        else if(strcmp(config->logTo, "Both") == 0)
        {
          printMonitor(timeOfDay, currentPCB->processNumber, currentMetaData, 8);
          printFile(logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 8);
        }
      }
    }

    //Handle Processor operation
    else if(currentMetaData->cmdLetter == 'P')
    {
      //Print out start for processor operation
      if(strcmp(config->logTo, "Monitor") == 0)
      {
        printMonitor(timeOfDay, currentPCB->processNumber, currentMetaData, 9);
      }
      else if(strcmp(config->logTo, "File") == 0)
      {
        printFile(logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 9);
      }
      else if(strcmp(config->logTo, "Both") == 0)
      {
        printMonitor(timeOfDay, currentPCB->processNumber, currentMetaData, 9);
        printFile(logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 9);
      }

      //Get the cycle time from operation cycle time * config file's processor time
      timeForCycle = ((double) currentMetaData->cycleTime * (double) config->processTime) / 100000;

      //Wait for the time found from the cycle time
      pthread_create(&threadP, NULL, (void *) &wait, (void *) &timeForCycle);
      pthread_join(threadP, NULL);

      timeOfDay += timeForCycle + 1;

      //Print out end for processor operation
      if(strcmp(config->logTo, "Monitor") == 0)
      {
        printMonitor(timeOfDay, currentPCB->processNumber, currentMetaData, 11);
      }
      else if(strcmp(config->logTo, "File") == 0)
      {
        printFile(logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 11);
      }
      else if(strcmp(config->logTo, "Both") == 0)
      {
        printMonitor(timeOfDay, currentPCB->processNumber, currentMetaData, 11);
        printFile(logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 11);
      }
    }

    //Handle Aplication Start and end
    else if(currentMetaData->cmdLetter == 'A')
    {
      //Wait for the time found from the cycle time
      timeOfDay += 0.000001;


      //Print out Start or End of Application
      if(strcmp(config->logTo, "Monitor") == 0)
      {
        printMonitor(timeOfDay, currentPCB->processNumber, currentMetaData, 10);
      }
      else if(strcmp(config->logTo, "File") == 0)
      {
        printFile(logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 10);
      }
      else if(strcmp(config->logTo, "Both") == 0)
      {
        printMonitor(timeOfDay, currentPCB->processNumber, currentMetaData, 10);
        printFile(logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 10);
      }
    }

    //Handle Memory Allocation
    else if(currentMetaData->cmdLetter == 'M')
    {
      //Grab the time for printing out memory management
      timeOfDay += 0.000001;

      //Print out the time for Memory management action
      if(strcmp(config->logTo, "Monitor") == 0)
      {
        printMonitor(timeOfDay, currentPCB->processNumber, currentMetaData, 12);
      }
      else if(strcmp(config->logTo, "File") == 0)
      {
        printFile(logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 12);
      }
      else if(strcmp(config->logTo, "Both") == 0)
      {
        printMonitor(timeOfDay, currentPCB->processNumber, currentMetaData, 12);
        printFile(logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 12);
      }
    }

    /* Adds a minor delay so that start operation is not the same of the last
       operation */
    timeOfDay += 0.000005;


    //Get the next meta data operation
    currentMetaData = currentMetaData->nextCMD;
    currentPCB->currentCMD = currentMetaData->cmdOrder;

    if(currentMetaData->cmdLetter == 'A' && strcmp(currentMetaData->descriptor, "end") == 0)
    {
      if(currentPCB->nextProcess == NULL)
      {
        printf("Time: %.6lf, Process %d set in Exit state.\n", timeOfDay, currentPCB->processNumber);
        break;
      }
      currentPCB->state = EXIT;
      printf("Time: %.6lf, Process %d set in Exit state.\n", timeOfDay, currentPCB->processNumber);
      currentPCB = currentPCB->nextProcess;
      currentMetaData = waitTillCMD(headMetaData, currentPCB->appStart);
      currentPCB->state = RUNNING;
      currentPCB->currentCMD = currentPCB->appStart;
      timeOfDay += 0.0000004;
      printf("Time: %.6lf, Process %d set in Running state.\n", timeOfDay, currentPCB->processNumber);
    }
  }

  //System stop
  timeOfDay += 0.000010;
  if(strcmp(config->logTo, "Monitor") == 0)
  {
    printMonitor(timeOfDay, 0, currentMetaData, 13);
  }
  else if(strcmp(config->logTo, "File") == 0)
  {
    printFile(logToFile, timeOfDay, 0, currentMetaData, 13);
  }
  else if(strcmp(config->logTo, "Both") == 0)
  {
    printMonitor(timeOfDay, 0, currentMetaData, 13);
    printFile(logToFile, timeOfDay, 0, currentMetaData, 13);
  }
  fclose(logToFile);
}

//Helper functions
/*
 * Name       : wait
 * Description: pauses the program given the time in microseconds (0.000000)
 *              based off of the current time of the timer.
 * Parameters : waitTime - The time to wait for (microseconds). (void * ptr)
 * Note       : Uses pthreads to manage function
 */
void* wait(void *ptr)
{
  double* waitTime = (double *) ptr;

  clock_t start = clock();

  while ((clock() - start) /CLOCKS_PER_SEC < (*waitTime));

  pthread_exit(0);
}

#endif
