#include "prof.h"

#ifdef TWO_DIM_ARR
#include "twoDimArr.h"
#endif // TWO_DIM_ARR

#ifdef DEQUE
#include "deque.h"
#endif // DEQUE

#include "pollserver.h"
#include "proactor.h"

#include <pthread.h>

#define MAX_NO_OF_THREADS 32

pthread_mutex_t lock;
Graph *myGraph = NULL;
pThread threads[MAX_NO_OF_THREADS];
struct pollfd *pfds;
int fd_count = 0;

struct arg_struct
{
    char *buffer;
    struct pollfd pfd;
    int i;
    char *result;
    int *edge_counter;
    int *n;
    int *m;
};

struct accept_args
{
    int listener;
    char *remoteIP;
    struct pollfd **pfds;
    int *fd_count;
    int *fd_size;
};

int words(char sentence[])
{
    int count = 1;
    for (int i = 0; sentence[i] != '\0'; ++i)
        if (sentence[i] == ' ')
            count++;
    return count;
}

void parseCommand(char *input, char **command, int word_count)
{
    char *token = strtok(input, " ");
    int i = 0;
    while (token != NULL)
    {
        command[i] = (char *)malloc(sizeof(char) * strlen(token));
        command[i++] = token;
        token = strtok(NULL, " ");
    }
    command[word_count] = NULL;
}

char *parse(char *input, int *edge_counter, int *n, int *m)
{
    pthread_mutex_lock(&lock);

    char *result = (char *)malloc(sizeof(char) * 256);
    if (input[strlen(input) - 1] == '\n')
        input[strlen(input) - 1] = '\0';
    int input_length = strlen(input);
    char *input_copy = (char *)malloc(sizeof(char) * input_length);
    strcpy(input_copy, input);
    int word_count = words(input);

    if (word_count > 1)
    {
        char **command = (char **)malloc(sizeof(char *) * (word_count + 1));
        parseCommand(input, command, word_count);

        if (input_length == 3 || !strcmp(command[0], "newgraph"))
        {
            if (input_length != 3)
            {
                *n = command[1][0] - '0', *m = command[1][2] - '0';
                myGraph = createGraph((int)*n);
            }
            else if (*edge_counter < *m)
            {
                addEdge(myGraph, input_copy[0] - '1', input_copy[2] - '1');
                (*edge_counter)++;
            }

            result = "newgraph\n";
        }
        else if (!strcmp(command[0], "newedge"))
            addEdge(myGraph, command[1][0] - '1', command[1][2] - '1');
        else if (!strcmp(command[0], "removeedge"))
            myGraph->G[command[1][0] - '1'][command[1][2] - '1'] = 0;
    }
    else if (!strcmp(input, "exit"))
    {
        result = "exit\n";
    }
    else if (!strcmp(input, "kosaraju"))
    {
        result = kosarajuSCC(myGraph);
    }

    pthread_mutex_unlock(&lock);
    return result;
}

void send_to_everyone(struct pollfd *pfds, char *result, int nbytes)
{
    for (int j = 1; j < fd_count; j++)
    {
        // Send to everyone!
        int dest_fd = pfds[j].fd;

        // Except the listener and ourselves
        if (send(dest_fd, result, nbytes, 0) == -1)
            perror("send");
    }
}

void *handle_client_thread(void *arguments)
{
    struct arg_struct *args = (struct arg_struct *)arguments;
    char *buf = args->buffer;
    struct pollfd pfd = args->pfd;
    char *result = args->result;

    int *edge_counter = args->edge_counter;
    int *n = args->n;
    int *m = args->m;

    for (int i = 0; i < 256; i++)
        buf[i] = 0;
    // If not the listener, we're just a regular client
    int nbytes = recv(pfd.fd, buf, 256, 0);

    int sender_fd = pfd.fd;

    if (nbytes <= 0)
    {
        // Got error or connection closed by client
        if (nbytes == 0) // Connection closed
            printf("pollserver: socket %d hung up\n", sender_fd);
        else
            perror("recv");

        close(pfd.fd); // Bye!

        // del_from_pfds(pfds, i, fd_count);
    }
    else
    {
        // We got some good data from a client
        result = parse(buf, edge_counter, n, m);
        if (!strcmp(result, "exit\n"))
        {
            close(pfd.fd);
            stopProactor(pthread_self());
        }

        printf("result is: %s", result);

        send_to_everyone(pfds, result, strlen(result));
    }
    return NULL;
}

void *handle_client_thread_wrapper(void *fd)
{
    int my_fd = *(int *)fd;

    char buf[256] = {0}; // Buffer for client data

    int edge_counter = 0, n = 0, m = 0;

    struct pollfd pfd;
    pfd.fd = my_fd;
    pfd.events = POLLIN; // Report ready to read on incoming connection

    struct arg_struct args;
    args.buffer = buf;
    args.pfd = pfd;
    args.i = 0;
    args.result = NULL;

    args.edge_counter = &edge_counter;
    args.n = &n;
    args.m = &m;

    while (poll(&pfd, 1, -1) > 0)
    {
        handle_client_thread((void *)&args);
    }

    return NULL;
}

int main()
{

    printf("welcome to our graph factory what would you like to do?\n");
    printf("possible options are:\n\tnewgraph i,j\n\tkosaraju\n\tnewedge i,j\n\tremoveedge i,j\n\texit\n");

    int listener; // Listening socket descriptor

    char remoteIP[INET6_ADDRSTRLEN];

    // Start off with room for 5 connections
    // (We'll realloc as necessary)
    int fd_size = MAX_NO_OF_THREADS;
    pfds = malloc(sizeof *pfds * fd_size);

    // Set up and get a listening socket
    listener = get_listener_socket();

    if (listener == -1)
    {
        fprintf(stderr, "error getting listening socket\n");
        exit(1);
    }

    // Add the listener to set
    pfds[0].fd = listener;
    pfds[0].events = POLLIN; // Report ready to read on incoming connection

    fd_count = 1; // For the listener

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init has failed\n");
        return 1;
    }

    // Main loop
    for (;;)
    {

        int poll_count = poll(pfds, fd_count, -1);

        if (poll_count == -1)
        {
            perror("poll");
            exit(1);
        }

        // Run through the existing connections looking for data to read
        for (int i = 0; i < fd_count; i++)
        {
            // Check if someone's ready to read
            if (pfds[i].revents & POLLIN)
            { // We got one!!

                if (pfds[i].fd == listener)
                {
                    int newfd;                          // Newly accepted socket descriptor
                    struct sockaddr_storage remoteaddr; // Client address
                    socklen_t addrlen;

                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);

                    if (newfd == -1)
                        perror("accept");
                    else
                    {
                        add_to_pfds(&pfds, newfd, &fd_count, &fd_size);
                        printf("pollserver: new connection from %s on socket %d\n", inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr *)&remoteaddr), remoteIP, INET6_ADDRSTRLEN), newfd);
                    }

                    pThread p;

                    p.sockfd = newfd;
                    p.tid = startProactor(&newfd, handle_client_thread_wrapper);

                    threads[i] = p;
                }

                // } // END handle data from client
            } // END got ready-to-read from poll()
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!

    pthread_mutex_destroy(&lock);

    return 0;
}