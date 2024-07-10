#include "reactor.h"

void *startReactor()
{
	printf("starting reactor\n");
	reactor *myReactor = malloc(sizeof(reactor)* 4);
	myReactor->handlers = malloc(sizeof(eventHandler) * MAX_NO_OF_HANDLES);

	// Set up and get a listening socket
	int listener = get_listener_socket();

	if (listener == -1)
	{
		fprintf(stderr, "error getting listening socket\n");
		exit(1);
	}
	// Add the listener to set
	myReactor->handlers[0].pollfd.fd = listener;
	myReactor->handlers[0].pollfd.events = POLLIN; // Report ready to read on incoming connection
	myReactor->handlers[0].isUsed = 1;

	myReactor->size = 1; // For the listener

	for (size_t i = 1; i < MAX_NO_OF_HANDLES; i++)
	{
		myReactor->handlers[i].isUsed = 1;
	}

	return myReactor;
}

int addFdToReactor(void *Reactor, int fd, reactorFunc func)
{
	reactor *myReactor = (reactor *)Reactor;
	if (myReactor->size == myReactor->capacity)
	{
		myReactor->capacity *= 2;
		myReactor->handlers = realloc(myReactor->handlers, sizeof(eventHandler) * myReactor->capacity);
		for (size_t i = myReactor->size; i < myReactor->capacity; i++)
		{
			myReactor->handlers[i].isUsed = 1;
		}
	}
	myReactor->handlers[myReactor->size].isUsed = 1;
	myReactor->handlers[myReactor->size].pollfd.fd = fd;
	myReactor->handlers[myReactor->size].pollfd.events = POLLIN;
	myReactor->handlers[myReactor->size].handleEvent = func;
	myReactor->size++;

	return 0;
}

int removeFdFromReactor(void *Reactor, int fd)
{
	reactor *myReactor = (reactor *)Reactor;
	size_t i;
	for (i = 0; i < myReactor->size; i++)
	{
		if (myReactor->handlers[i].pollfd.fd == fd)
		{
			break;
		}
	}

	if (i == myReactor->size)
	{
		return -1;
	}

	for (size_t j = i; j < myReactor->size - 1; j++)
	{
		myReactor->handlers[j] = myReactor->handlers[j + 1];
	}

	myReactor->size--;

	return 0;
}

int stopReactor(void *Reactor)
{
	reactor *myReactor = (reactor *)Reactor;
	free(myReactor->handlers);
	free(myReactor);
	return 0;
}

size_t buildPollArray(void *Reactor, struct pollfd *pollArray)
{
	reactor *myReactor = (reactor *)Reactor;
	size_t i = 0;
    size_t noOfCopiedHandles = 0;
	for (i = 0; i < myReactor->size; i++)
	{
		if (myReactor->handlers[i].isUsed)
		{
			pollArray[noOfCopiedHandles] = myReactor->handlers[i].pollfd;
			noOfCopiedHandles++;
		}
	}
	return noOfCopiedHandles;
}
