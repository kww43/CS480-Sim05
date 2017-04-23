//ProcessQueue.c
#ifndef PROCQUEUE_c
#define PROCQUEUE_c

#include "ProcessQueue.h"

void initFCFS(struct Queue* queueInfo, int *que, int size)
{
	queueInfo->size = size;
	queueInfo->head = 0;
	queueInfo->end = size-1;
	int counter;

	for(counter = 0; counter < size; counter++)
	{
		que[counter] = counter;
	}
}

void initSRTF(struct Queue* queueInfo, int *que, int size, int arraySize)
{
	queueInfo->size = size;
	queueInfo->head = 0;
	queueInfo->end = size-1;
	int counter;

	for(counter = 0; counter < size; counter++)
	{
		que[counter] = counter;
	}

}

void insertEnd(struct Queue* queueInfo, int *queue, int size, int procID)
{
	if(queueInfo->end == size-1)
  {
      queueInfo->end= -1;
  }

  queue[++queueInfo->end] = procID;
}

void insertPriority(struct Queue* queueInfo, int *queue, int procID)
{
	int index;
	queue[procID] = procID;
	if (procID < queueInfo->head)
	{
		queueInfo->head = procID;
	}
	for (index = 0; index < queueInfo->size; index ++)
	{
		if (queue[index] != -1)
		{
			queueInfo->end = index;
		}
	}
}

int removeFront(struct Queue* queueInfo, int *queue)
{
	int index;
	int headIndex;

	headIndex = queueInfo->head;
	queue[queueInfo->head] = -1; //removes head process from ready queue
	//find next head
	for (index = 0; index < queueInfo->size; index ++)
	{
		if (queue[index] != -1)
		{
			queueInfo->head = index;
			break;
		}
		else if (index == queueInfo->size - 1)
		{
			//queue is empty
			queueInfo->head = -1;
			queueInfo->end = -1;
		}
	}
	return headIndex;
}

int removeSize(struct Queue* queueInfo, int *queue)
{
		int index;
		int headIndex;

		headIndex = queueInfo->head;
		queue[queueInfo->head] = -1; //removes head process from ready queue
		//find next head
		for (index = 0; index < queueInfo->size; index ++)
		{
			if (queue[index] != -1)
			{
				queueInfo->head = index;
				break;
			}
			else if (index == queueInfo->size - 1)
			{
				//queue is empty
				queueInfo->head = -1;
				queueInfo->end = -1;
			}
		}
		return headIndex;
}

#endif
