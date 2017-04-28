/*
 * =====================================================================================
 *
 *       Filename:  Thread.c
 *
 *    Description:  Handles thread operations for the operating system simulator.
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
#ifndef THREAD_C
#define THREAD_C

// Header files ///////////////////////////////////////////////////////////////////////
#include "Thread.h"

/*
 * Name       : pause
 * Description: pauses the program given the time in microseconds (0.000000)
 *              based off of the current time of the timer.
 * Parameters : waitTime - The time to wait for (microseconds).
 *              currentTime - The current time of the timer.
 * Note       : Uses pthreads to manage function
 */
void* pause(void *ptr)
{
  int returnVal;
  struct THREAD_INFO* thread = (struct THREAD_INFO*) ptr;
  double waitTime = thread->waitTime;

  clock_t start = clock();

  while ((clock() - start) / CLOCKS_PER_SEC < (waitTime));

  returnVal = interuptHandler(SEND_INTERRUPT, thread->procID, thread->totalProcesses);
  returnVal++; // Prevents compiler warning

  printThread(thread);

  pthread_exit(0);
}

/********** UPDATED FUNCTION FOR SIM05 (# 1)***********/
/*
 * Name       : interuptHandler
 * Description: Handles interupts based on the code sent to the function,
 *              whether its checking or setting an interupt.
 * Parameters : type     - The code what type of event to handle with the interupts
 *              procID   - The process ID to handle the interupt for.
 * Note       : SEND_INTERRUPT - Is to set the interupt flag, while
 *              CHECK_INTERRUPT - Is to check if the interupt flag has been set,
 *              and handles the interupts.
 */
int interuptHandler(InterruptCodes type, int procID, int totalProcesses)
{
  int increment = 0;
  static Boolean interruptFlag = FALSE;
  static int interuptQueue[50] = {-1};

  switch(type)
  {

    case CHECK_INTERRUPT:

      if(interruptFlag == TRUE)
      {
        for(increment = 0; increment < totalProcesses; increment++)
        {
          if(increment == interuptQueue[increment])
          {
            interuptQueue[increment] = -1;
            return increment;
          }
          else if(increment == totalProcesses-1)
          {
            interruptFlag = FALSE;
          }
        }
      }
      else // Interupt flag is set to false
      {
        return -1;
      }
      break;

    case SEND_INTERRUPT:
      interruptFlag = TRUE;
      interuptQueue[procID] = procID;
      printf("Process %d Set in Interupt Queue\n", interuptQueue[procID]);
      return -1;
      break;
  }

  return -1;
}

void printThread(struct THREAD_INFO *thread)
{
  double timeOfDay = thread->currentTime + thread->waitTime;
  int processID = thread->procID;
  char* descriptor = thread->descriptor;

  //Print out Output operation end
  if(thread->cmdLetter == 'O')
  {
      printf("Time: %.6lf, Process %d %s output end\n", timeOfDay, processID, descriptor);
  }

  //Print out Input operation end
  else if(thread->cmdLetter == 'I')
  {
      printf("Time: %.6lf, Process %d %s input end\n", timeOfDay, processID, descriptor);
  }
}


#endif
