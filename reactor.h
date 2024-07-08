#pragma once

#include <poll.h>
#include <stdio.h>

typedef void* (*reactorFunc)(int fd);
typedef struct
{
	struct pollfd fd;
	reactorFunc handleEvent;
} eventHandler;

#define MAX_NO_OF_HANDLES 32

typedef struct reactor
{
	size_t size;
	eventHandler handlers[MAX_NO_OF_HANDLES];
};


// starts new reactor and returns pointer to it
void *startReactor();

// adds fd to Reactor (for reading) ; returns 0 on success.
int addFdToReactor(void *reactor, int fd, reactorFunc func);

// removes fd from reactor
int removeFdFromReactor(void *reactor, int fd);

// stops reactor
int stopReactor(void *reactor);