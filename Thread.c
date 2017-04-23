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
  double* waitTime = (double *) ptr;

  clock_t start = clock();

  while ((clock() - start) / CLOCKS_PER_SEC < (*waitTime));

  interuptHandler(SEND_INTERRUPT, 0); //TODO 0 is where process ID will be stored

  pthread_exit(0);
}

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
void interuptHandler(InterruptCodes type, int procID)
{
  int increment = 0;
  static Boolean interruptFlag = FALSE;
  static int interuptQueue[200] = {-1};

  switch(type)
  {

    case CHECK_INTERRUPT:
      if(interruptFlag == TRUE)
      {
        while(increment < 200)
        {
          if(interuptQueue[increment] == increment)
          {
            //TODO HANDLE INTERRUPT
          }
          increment++;
        }
        interruptFlag = FALSE;
      }
      break;

    case SEND_INTERRUPT:
      interruptFlag = TRUE;
      interuptQueue[procID] = procID;
      break;
  }
}

#endif
