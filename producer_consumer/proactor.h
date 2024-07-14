#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

typedef struct _pThread
{
    int sockfd;
    pthread_t tid;
} pThread;

typedef void *(*proactorFunc)(void *args);
// starts new proactor and returns proactor thread id.
pthread_t startProactor(int *sockfd, proactorFunc threadFunc);
// stops proactor by threadid
int stopProactor(pthread_t tid);