#pragma once

#include <poll.h>
#include <stdio.h>

#include "pollserver.h"

#define MAX_NO_OF_HANDLES 32

typedef char *(*reactorFunc)(char *input, void **graph, int *edge_counter, int *n, int *m);
typedef struct _eventHandler
{
	struct pollfd pollfd;
	reactorFunc handleEvent;
	int isUsed;
} eventHandler;

typedef struct _reactor
{
	size_t size;
	size_t capacity;
	eventHandler *handlers;
} reactor;

// starts new reactor and returns pointer to it
void *startReactor();

// adds fd to Reactor (for reading) ; returns 0 on success.
int addFdToReactor(void *reactor, int fd, reactorFunc func);

// removes fd from reactor
int removeFdFromReactor(void *reactor, int fd);

// stops reactor
int stopReactor(void *reactor);

size_t buildPollArray(void *reactor, struct pollfd *pollArray);
