#ifndef PROCQUEUE_h
#define PROCQUEUE_h

struct Queue
{
	int head;
	int end;
	int size;
};

void initFCFS(struct Queue* queueInfo, int *que, int size);

void initSRTF(struct Queue* queueInfo, int *que, int size, int arraySize);

void insertEnd(struct Queue* queueInfo, int *queue, int size, int procID);

void insertPriority(struct Queue* queueInfo, int *queue, int procID);

int removeFront(struct Queue* queueInfo, int *queue);

int removeSize(struct Queue* queueInfo, int *queue);

#endif
