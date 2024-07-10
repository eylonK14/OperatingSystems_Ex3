#include "prof.h"

#ifdef TWO_DIM_ARR
#include "twoDimArr.h"
#endif // TWO_DIM_ARR

#ifdef DEQUE
#include "deque.h"
#endif // DEQUE

#include "pollserver.h"

#include <pthread.h>

#define MAX_NO_OF_THREADS 32

pthread_mutex_t lock;

struct arg_struct
{
    char *buffer;
    struct pollfd *pfds;
    int i;
    int *fd_count;
    char *result;
    Graph **myGraph;
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

char *parse(char *input, Graph **graph, int *edge_counter, int *n, int *m)
{
    pthread_mutex_lock(&lock);
    printf("the graph is %p\n", graph);
    char *result = (char *)malloc(sizeof(char) * 256);
    if (input[strlen(input) - 1] == '\n')
        input[strlen(input) - 1] = '\0';
    int input_length = strlen(input);
    char *input_copy = (char *)malloc(sizeof(char) * input_length);
    strcpy(input_copy, input);
    int word_count = words(input);
    printf("the input is %s\n", input);
    printf("the input copy is %s\n", input_copy);
    if (word_count > 1)
    {
        char **command = (char **)malloc(sizeof(char *) * (word_count + 1));
        parseCommand(input, command, word_count);
        if (input_length == 3 || !strcmp(command[0], "newgraph"))
        {
            if (input_length != 3)
            {
                *n = command[1][0] - '0', *m = command[1][2] - '0';
                *graph = createGraph((int)*n);
            }
            else if (*edge_counter < *m)
            {
                addEdge(*graph, input_copy[0] - '1', input_copy[2] - '1');
                (*edge_counter)++;
            }

            result = "newgraph\n";
        }
        else if (!strcmp(command[0], "newedge"))
            addEdge(*graph, command[1][0] - '1', command[1][2] - '1');
        else if (!strcmp(command[0], "removeedge"))
            (*graph)->G[command[1][0] - '1'][command[1][2] - '1'] = 0;
    }
    else if (!strcmp(input, "exit"))
        exit(EXIT_SUCCESS);
    else if (!strcmp(input, "kosaraju"))
    {
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                printf("%d\n", (*graph)->G[i][j]);
        result = kosarajuSCC(*graph);
    }

    pthread_mutex_unlock(&lock);
    return result;
}

void send_to_everyone(int fd_count, struct pollfd *pfds, char *result, int nbytes)
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
    printf("in handle_client_thread\n");
    struct arg_struct *args = (struct arg_struct *)arguments;
    char *buf = args->buffer;
    struct pollfd *pfds = args->pfds;
    int i = args->i;
    int *fd_count = args->fd_count;
    char *result = args->result;
    Graph **myGraph = args->myGraph;
    int *edge_counter = args->edge_counter;
    int *n = args->n;
    int *m = args->m;

    for (int i = 0; i < 256; i++)
        buf[i] = 0;
    // If not the listener, we're just a regular client
    int nbytes = recv(pfds[i].fd, buf, sizeof buf, 0);

    printf("got message\n");

    int sender_fd = pfds[i].fd;

    printf("the buffer is %s\n", buf);

    if (nbytes <= 0)
    {
        // Got error or connection closed by client
        if (nbytes == 0) // Connection closed
            printf("pollserver: socket %d hung up\n", sender_fd);
        else
            perror("recv");

        close(pfds[i].fd); // Bye!

        del_from_pfds(pfds, i, fd_count);
    }
    else
    {
        // We got some good data from a client
        result = parse(buf, myGraph, edge_counter, n, m);
        printf("result is %s\n", result);
        send_to_everyone(*fd_count, pfds, result, strlen(result));
    }
    return NULL;
}

void *accept_client_thread(void *arguments)
{
    // If listener is ready to read, handle new connection

    struct accept_args *args = (struct accept_args *)arguments;

    char *remoteIP = args->remoteIP;
    struct pollfd *pfds = *args->pfds;
    int listener = args->listener;
    int *fd_count = args->fd_count;
    int *fd_size = args->fd_size;

    int newfd;                          // Newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // Client address
    socklen_t addrlen;

    addrlen = sizeof remoteaddr;
    newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);

    printf("got connection\n");
    // for (int i = 0; i < fd_size; i++)
    // {
    //     printf("fd_no_%d is: %d\n", i, pfds[i].fd);
    // }

    if (newfd == -1)
        perror("accept");
    else
    {
        printf("newfd is %d\n", newfd);
        printf("fd_count is %d\n", *fd_count);
        printf("fd_size is %d\n", *fd_size);

        add_to_pfds(&pfds, newfd, fd_count, fd_size);

        printf("pollserver: new connection from %s on socket %d\n", inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr *)&remoteaddr), remoteIP, INET6_ADDRSTRLEN), newfd);
    }
    return NULL;
}

int main()
{
    char *result = NULL;
    pthread_t threads[MAX_NO_OF_THREADS];

    printf("welcome to our graph factory what would you like to do?\n");
    printf("possible options are:\n\tnewgraph i,j\n\tkosaraju\n\tnewedge i,j\n\tremoveedge i,j\n\texit\n");
    Graph *myGraph = NULL;
    int edge_counter = 0, n = 0, m = 0;

    int listener; // Listening socket descriptor

    char buf[256] = {0}; // Buffer for client data

    char remoteIP[INET6_ADDRSTRLEN];

    // Start off with room for 5 connections
    // (We'll realloc as necessary)
    int fd_count = 0;
    int fd_size = MAX_NO_OF_THREADS;
    struct pollfd *pfds = malloc(sizeof *pfds * fd_size);

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
        printf("fd_count is %d\n", fd_count);
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
                printf("got data from %d\n", pfds[i].fd);
                printf("listener is %d\n", listener);
                if (pfds[i].fd == listener)
                {
                    struct accept_args args;
                    args.listener = listener;
                    args.pfds = &pfds;
                    args.remoteIP = remoteIP;
                    args.fd_count = &fd_count;
                    args.fd_size = &fd_size;

                    if (pthread_create(&threads[i], NULL, &accept_client_thread, (void *)&args) != 0)
                    {
                        perror("pthread_create");
                        exit(1);
                    }

                    pthread_join(threads[i], NULL);
                }
                else
                {
                    struct arg_struct args;
                    args.buffer = buf;
                    args.pfds = pfds;
                    args.i = i;
                    args.fd_count = &fd_count;
                    args.result = result;
                    args.myGraph = &myGraph;
                    args.edge_counter = &edge_counter;
                    args.n = &n;
                    args.m = &m;

                    if (pthread_create(&threads[i], NULL, &handle_client_thread, (void *)&args) != 0)
                    {
                        perror("pthread_create");
                        exit(1);
                    }

                    pthread_join(threads[i], NULL);

                } // END handle data from client
            } // END got ready-to-read from poll()
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!

    pthread_mutex_destroy(&lock);

    return 0;
}