#include "proactor.h"

// starts new proactor and returns proactor thread id.
pthread_t startProactor(int *sockfd, proactorFunc threadFunc)
{
    pthread_t tid = 0;

    if (pthread_create(&tid, NULL, threadFunc, (void *)sockfd) != 0)
    {
        perror("pthread_create");
        exit(1);
    }

    return tid;
}
// stops proactor by threadid
int stopProactor(pthread_t tid)
{
    if (pthread_kill(tid, SIGINT) != 0)
    {
        perror("pthread_kill");
        // exit(1);
    }

    return 0;
}