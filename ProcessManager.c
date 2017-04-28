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
#ifndef PROCESS_MANAGER_c
#define PROCESS_MANAGER_c

// Header files ////////////////////////////////////////////////////////////////////////
#include "ProcessManager.h"

//Functions/////////////////////////////////////////////////////////////////////////////

/*
* ===  FUNCTION  ======================================================================
*         Name:  processFCFS_SRTF
*  Description: Handles processes and their meta data
*
*  Parameters : config - the structure that contains the configuration file
*               metaData - The meta data for the  process
                PCB - Process Control Block
                totalProcesses - numebr of processes in file
* =====================================================================================
*/
void processFCFS_SRTF(struct CONFIGFILE *config, struct METAFILE *metaData, struct PROCESSES *PCB, int totalProcesses)
{
  //Variables///////////////////////////////////////////////////////////////////
  int completedProcesses = 0;
  struct METAFILE *currentMetaData = metaData; //The current meta data operation
  struct METAFILE *headMetaData = metaData;
  double timeForCycle;  //The cycle time for meta data OP * processor/io time
  double timeOfDay;     //The timer for the simulator
  struct PROCESSES *headPCB = PCB; //Start of processes for the PCB
  struct PROCESSES *currentPCB = PCB; //Current process for the PCB
  //struct THREAD_INFO *currentThread = malloc(sizeof(struct THREAD_INFO)); //Current thread
  FILE* logToFile = fopen(config->logFile ,"w"); //The log file handle to write to
  pthread_t threadIO; //IO Thread
  //pthread_t threadP; //Processor Thread
  struct THREAD_INFO *currentThread[totalProcesses];
  //initialize the array of thread structs
  for (int index = 0; index < totalProcesses; index++)
  {
    currentThread[index] = malloc(sizeof(struct THREAD_INFO));
  }

  //Intialize the time and start the system
  timeOfDay = 0.000000;
  printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 0);

  //Create the process
  timeOfDay += 0.000005;
  printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 1);


  //Set Processes to new state
  timeOfDay += 0.000004;
  while(currentPCB != NULL)
  {
    currentPCB->state = NEW;
    currentPCB = currentPCB->nextProcess;
  }
  currentPCB = headPCB;
  //Print the result of setting processes to new state
  printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 2);


  //Set Processes to Ready state
  timeOfDay += 0.000002;

  while(currentPCB != NULL)
  {
    currentPCB->state = READY;
    currentPCB->currentCMD = currentPCB->appStart;
    currentPCB = currentPCB->nextProcess;
  }
  currentPCB = headPCB;
  //Print the result of setting processes to ready state
  printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 3);

  //Set a Process to Running state
  timeOfDay += 0.000006;
  currentPCB->state = RUNNING;
  printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 4);

  currentMetaData = waitTillCMD(headMetaData, currentPCB->appStart);
  int processReturning;
  while(completedProcesses < totalProcesses)
  {
    currentPCB = headPCB;
    while(currentPCB->state == BLOCKED || currentPCB->state == EXIT)
    {
      currentPCB = currentPCB->nextProcess;
      if (currentPCB == NULL)
      {
        //continuously check for interrupts
        processReturning = interuptHandler(CHECK_INTERRUPT, 0, totalProcesses);
        while(processReturning == -1)
        {
            //IDLE and check for interupts
            processReturning = interuptHandler(CHECK_INTERRUPT, 0, totalProcesses);
        }

        //empty out interruptQueue
        while(processReturning != -1)
        {
          //go to process and set to ready
          currentPCB = setProcessToReady(headPCB, processReturning);
          currentPCB->state = READY;
          processReturning = interuptHandler(CHECK_INTERRUPT, 0, totalProcesses);
        }
        currentPCB = headPCB;
      }
      else
      {
        //nothing
      }
    }
    //If scheduling == SRTF -> Prioritize Processes
    if(strcmp(config->CPUSched, "SRTF-P") == 0)
    {
      //Prioritize Processes
      currentPCB = prioritizeProcesses(headPCB,totalProcesses);
      printf("Shortest: %d\n", currentPCB->totalRunTime);
    }
    //get next process
    currentMetaData = waitTillCMD(headMetaData, currentPCB->currentCMD);
    currentPCB->currentCMD = currentPCB->currentCMD + 1;
    currentPCB->state = RUNNING;

    //Handle I/O operation
    if(currentMetaData->cmdLetter == 'O' || currentMetaData->cmdLetter == 'I')
    {
      //Print out start of Output Operation
      if(currentMetaData->cmdLetter == 'O')
      {
        printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 5);
      }

      //Print out start of Input Operation
      else if(currentMetaData->cmdLetter == 'I')
      {
        printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 6);
      }

      //block current process
      printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 15);
      currentPCB->state = BLOCKED;

      /* Get the time to wait for based on meta data operation cycle time * the
         config file's I/O cycle time */
      timeForCycle = ((double) currentMetaData->cycleTime * (double) config->ioTime) / 100000;

      currentPCB->totalRunTime = currentPCB->totalRunTime - (timeForCycle * 100000);

      currentThread[currentPCB->processNumber]->procID = currentPCB->processNumber;
      currentThread[currentPCB->processNumber]->totalProcesses = totalProcesses;
      currentThread[currentPCB->processNumber]->cmdLetter = currentMetaData->cmdLetter;
      currentThread[currentPCB->processNumber]->descriptor = currentMetaData->descriptor;
      currentThread[currentPCB->processNumber]->waitTime = timeForCycle;
      currentThread[currentPCB->processNumber]->currentTime = timeOfDay;
      currentThread[currentPCB->processNumber]->logTo = config->logTo;

      //Wait for time found from cycle time * I/O cycle time
      pthread_create(&threadIO, NULL, (void *) &pause, (void *) currentThread[currentPCB->processNumber]);
      timeOfDay += timeForCycle;
    }

    //Handle Processor operation
    else if(currentMetaData->cmdLetter == 'P')
    {
      //Print out start for processor operation
      printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 9);

      //Get the cycle time from operation cycle time * config file's processor time
      timeForCycle = ((double) currentMetaData->cycleTime * (double) config->processTime) / 100000;
      currentPCB->totalRunTime = currentPCB->totalRunTime - (timeForCycle * 100000);
      clock_t start = clock();

      while ((clock() - start) / CLOCKS_PER_SEC < (timeForCycle));

      timeOfDay += timeForCycle + 1;

      //Print out end for processor operation
        printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 11);
    }

    //Handle Memory Allocation
    else if(currentMetaData->cmdLetter == 'M')
    {
      //Grab the time for printing out memory management
      timeOfDay += 0.000001;

      //Print out the time for Memory management action
      printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 12);
    }

  else if(currentMetaData->cmdLetter == 'A'
     && strcmp(currentMetaData->descriptor, "end") == 0
     && currentPCB->state != BLOCKED)
  {
    currentPCB->state = EXIT;
    printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 14);
    completedProcesses++;
    if(completedProcesses == totalProcesses) {break;}
  }
  processReturning = interuptHandler(CHECK_INTERRUPT, 0, totalProcesses);
  while(processReturning != -1)
  {
    //go to process and set to ready
    currentPCB = setProcessToReady(headPCB, processReturning);
    currentPCB->state = READY;
    processReturning = interuptHandler(CHECK_INTERRUPT, 0, totalProcesses);
  }
 }

   //System stop
   timeOfDay += 0.000010;
   printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 13);
   fclose(logToFile);
}

<<<<<<< HEAD
=======
void processRR_P(struct CONFIGFILE *config, struct METAFILE *metaData, struct PROCESSES *PCB, int totalProcesses)
{
  //Variables///////////////////////////////////////////////////////////////////
  int completedProcesses = 0;
  int increment;
  double currentQuantumTime = 0;
  struct METAFILE *currentMetaData = metaData; //The current meta data operation
  struct METAFILE *headMetaData = metaData;
  double timeForCycle;  //The cycle time for meta data OP * processor/io time
  double timeOfDay;     //The timer for the simulator
  struct PROCESSES *headPCB = PCB; //Start of processes for the PCB
  struct PROCESSES *currentPCB = PCB; //Current process for the PCB
  FILE* logToFile = fopen(config->logFile ,"w"); //The log file handle to write to
  pthread_t threadIO; //IO Thread
  //initRR(totalProcesses, headLink);
  struct THREAD_INFO *currentThread[totalProcesses];
  //initialize the array of thread structs
  for (int index = 0; index < totalProcesses; index++)
  {
    currentThread[index] = malloc(sizeof(struct THREAD_INFO));
  }

  //Intialize the time and start the system
  timeOfDay = 0.000000;
  printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 0);

  //Create the process
  timeOfDay += 0.000005;
  printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 1);


  //Set Processes to new state
  timeOfDay += 0.000004;
  while(currentPCB != NULL)
  {
    currentPCB->state = NEW;
    currentPCB = currentPCB->nextProcess;
  }
  currentPCB = headPCB;
  //Print the result of setting processes to new state
  printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 2);


  //Set Processes to Ready state
  timeOfDay += 0.000002;

  while(currentPCB != NULL)
  {
    currentPCB->state = READY;
    currentPCB->currentCMD = currentPCB->appStart;
    currentPCB = currentPCB->nextProcess;
  }
  currentPCB = headPCB;
  //Print the result of setting processes to ready state
  printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 3);

  //Set a Process to Running state
  timeOfDay += 0.000006;
  currentPCB->state = RUNNING;
  printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 4);

  currentMetaData = waitTillCMD(headMetaData, currentPCB->appStart);
  int processReturning;
  currentPCB = headPCB;
  while(completedProcesses < totalProcesses)
  {
    if(currentQuantumTime >= config->quantumTime)
    {
      currentPCB = currentPCB->nextProcess;
      currentQuantumTime = 0;
    }

    while(currentPCB->state == BLOCKED || currentPCB->state == EXIT)
    {
      currentPCB = currentPCB->nextProcess;
      if (currentPCB == NULL)
      {
        //continuously check for interrupts
        processReturning = interuptHandler(CHECK_INTERRUPT, 0, totalProcesses);
        while(processReturning == -1)
        {
            //IDLE and check for interupts
            processReturning = interuptHandler(CHECK_INTERRUPT, 0, totalProcesses);
        }

        //empty out interruptQueue
        while(processReturning != -1)
        {
          //go to process and set to ready
          currentPCB = setProcessToReady(headPCB, processReturning);
          currentPCB->state = READY;
          processReturning = interuptHandler(CHECK_INTERRUPT, 0, totalProcesses);
        }
        currentPCB = headPCB;
        currentQuantumTime = 0;
      }
    }
    //get next process
    currentMetaData = waitTillCMD(headMetaData, currentPCB->currentCMD);
    currentPCB->currentCMD = currentPCB->currentCMD + 1;
    currentPCB->state = RUNNING;

    //Handle I/O operation
    if(currentMetaData->cmdLetter == 'O' || currentMetaData->cmdLetter == 'I')
    {
      //Print out start of Output Operation
      if(currentMetaData->cmdLetter == 'O')
      {
        printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 5);
      }

      //Print out start of Input Operation
      else if(currentMetaData->cmdLetter == 'I')
      {
        printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 6);
      }

      //block current process
      printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 15);
      currentPCB->state = BLOCKED;

      /* Get the time to wait for based on meta data operation cycle time * the
         config file's I/O cycle time */
      timeForCycle = ((double) currentMetaData->cycleTime * (double) config->ioTime) / 100000;
      currentQuantumTime += timeForCycle;

      currentThread[currentPCB->processNumber]->procID = currentPCB->processNumber;
      currentThread[currentPCB->processNumber]->totalProcesses = totalProcesses;
      currentThread[currentPCB->processNumber]->cmdLetter = currentMetaData->cmdLetter;
      currentThread[currentPCB->processNumber]->descriptor = currentMetaData->descriptor;
      currentThread[currentPCB->processNumber]->waitTime = timeForCycle;
      currentThread[currentPCB->processNumber]->currentTime = timeOfDay;
      currentThread[currentPCB->processNumber]->logTo = config->logTo;

      //Wait for time found from cycle time * I/O cycle time
      pthread_create(&threadIO, NULL, (void *) &pause, (void *) currentThread[currentPCB->processNumber]);

      timeOfDay += timeForCycle;
    }

    //Handle Processor operation
    else if(currentMetaData->cmdLetter == 'P')
    {
      //Print out start for processor operation
      printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 9);

      //Get the cycle time from operation cycle time * config file's processor time
      timeForCycle = ((double) currentMetaData->cycleTime * (double) config->processTime) / 100000;
      currentQuantumTime += timeForCycle;

      clock_t start = clock();

      while ((clock() - start) / CLOCKS_PER_SEC < (timeForCycle));

      timeOfDay += timeForCycle + 1;

      //Print out end for processor operation
        printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 11);
    }

    //Handle Memory Allocation
    else if(currentMetaData->cmdLetter == 'M')
    {
      //Grab the time for printing out memory management
      timeOfDay += 0.000001;

      //Print out the time for Memory management action
      printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 12);
    }

  else if(currentMetaData->cmdLetter == 'A'
     && strcmp(currentMetaData->descriptor, "end") == 0
     && currentPCB->state != BLOCKED)
  {
    currentPCB->state = EXIT;
    printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 14);
    completedProcesses++;
    if(completedProcesses == totalProcesses) {break;}
  }

  processReturning = interuptHandler(CHECK_INTERRUPT, 0, totalProcesses);
  while(processReturning != -1)
  {
    //go to process and set to ready
    currentPCB = setProcessToReady(headPCB, processReturning);
    currentPCB->state = READY;
    processReturning = interuptHandler(CHECK_INTERRUPT, 0, totalProcesses);
  }
 }


   //System stop
   timeOfDay += 0.000010;
   printTo(config, logToFile, timeOfDay, currentPCB->processNumber, currentMetaData, 13);
   fclose(logToFile);
}

>>>>>>> 4068f8848c9d36b7d91b759beb0863cdd4509691
//HELPER FUNCTIONS//////////////////////////////////////////////////////////////////

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

/*
 * Name       : setProcessToReady
 * Description: Goes to the process in the PCB based off its numerical order.
 * Parameters : headPCB - The start of the meta data file.
 *              cmdNumber - The meta data operation to go to in the file.
 * return     : The meta data operation based off the number passed in.
 */
struct PROCESSES* setProcessToReady(struct PROCESSES *headPCB, int cmdNumber)
{
  struct PROCESSES *tempLink = headPCB;

  while(tempLink->processNumber != cmdNumber)
  {
    tempLink = tempLink->nextProcess;
  }

  tempLink->state = READY;
  printf("Process %d set in Ready State\n", tempLink->processNumber);

  return tempLink;
}


/*
 * Name       : prioritizeProcesses
 * Description: Returns shortest process to run
 * Parameters : headPCB - The start of the meta data file.
              : totalProcesses - Number of Process in file
 * return     : The shortest process to run
 */
struct PROCESSES* prioritizeProcesses(struct PROCESSES *headPCB, int totalProcesses)
{
  struct PROCESSES *currentPCB;
  struct PROCESSES *currentMinNode;
  double currentMin = 100000;

  currentPCB = headPCB;
  //lops until end of linked list of processes
  while (currentPCB != NULL)
  {
    //find shortest process
    if (currentPCB->state == READY && currentPCB->totalRunTime < currentMin)
    {
      currentMinNode = currentPCB;
      currentMin = currentPCB->totalRunTime;
    }
    else if (currentPCB->state == RUNNING && currentPCB->totalRunTime < currentMin)
    {
      currentMinNode = currentPCB;
      currentMin = currentPCB->totalRunTime;
    }
    //get next process
    currentPCB = currentPCB->nextProcess;
  }
  return currentMinNode;
}

/*
 * Name       : waitTillCMD
 * Description: Goes to the meta data operation in the file based off its numerical order.
 * Parameters : headMetaData - The start of the meta data file.
 *              cmdNumber - The meta data operation to go to in the file.
 * return     : The meta data operation based off the number passed in.
 */
struct METAFILE* waitTillCMD(struct METAFILE *headMetaData, int cmdNumber)
{
  struct METAFILE *tempLink = headMetaData;

  while(tempLink->cmdOrder != cmdNumber)
  {
    tempLink = tempLink->nextCMD;
  }

  return tempLink;
}

#endif //end define for PROCESS_MANAGER_c
