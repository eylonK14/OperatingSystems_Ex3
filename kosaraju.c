#include "prof.h"

#ifdef TWO_DIM_ARR
#include "twoDimArr.h"
#endif // TWO_DIM_ARR

#ifdef DEQUE
#include "deque.h"
#endif // DEQUE

#include "pollserver.h"
#include "kosaraju.h"

int **newGraph(int edges, int fd_count, struct pollfd *pfds, int listener, int sender_fd)
{
    int i;
    int **graph = malloc(edges * sizeof(int *));
    for (i = 0; i < edges; i++)
    {
        graph[i] = (int *)calloc(2, sizeof(int));
    }
    char buf[256] = {0}; // Buffer for client data
    for (i = 0; i < edges; i++)
    {
        int a, b;
        // printf("Enter the edge #%d: ", i + 1);
        send_to_everyone(fd_count, pfds, listener, sender_fd, "Enter the edge #", 15);
        // scanf("%d %d", &a, &b);
        int nbytes = recv(pfds[i].fd, buf, sizeof buf, 0);

        int sender_fd = pfds[i].fd;

        if (nbytes <= 0)
        {
            // Got error or connection closed by client
            if (nbytes == 0)
            {
                // Connection closed
                printf("pollserver: socket %d hung up\n", sender_fd);
            }
            else
            {
                perror("recv");
            }
        }
        a = buf[0] - '0';
        b = buf[2] - '0';
        graph[i][0] = a - 1;
        graph[i][1] = b - 1;
    }
    return graph;
}

int words(char sentence[])
{
    int count = 1;
    for (int i = 0; sentence[i] != '\0'; ++i)
        if (sentence[i] == ' ')
            count++;
    return count;
}

char *parse(char *input, Graph **graph, int fd_count, struct pollfd *pfds, int listener, int sender_fd)
{
    char *result = NULL;
    // strcpy(result, "");
    int word_count = words(input);
    if (word_count > 1)
    {
        char **command = (char **)malloc(sizeof(char *) * (word_count + 1));
        char *token = strtok(input, " ");
        int i = 0;
        while (token != NULL)
        {
            command[i] = (char *)malloc(sizeof(char) * strlen(token));
            command[i++] = token;
            token = strtok(NULL, " ");
        }
        command[word_count] = NULL;
        if (!strcmp(command[0], "newgraph"))
        {
            int n = command[1][0] - '0', m = command[1][2] - '0';
            int **myGraph = newGraph(m, fd_count, pfds, listener, sender_fd);

            *graph = createGraph(n);
            for (int i = 0; i < m; i++)
            {
                addEdge(*graph, myGraph[i][0], myGraph[i][1]);
            }
        }
        else if (!strcmp(command[0], "newedge"))
        {
            addEdge(*graph, command[1][0] - '1', command[1][2] - '1');
        }
        else if (!strcmp(command[0], "removeedge"))
        {
            (*graph)->G[command[1][0] - '1'][command[1][2] - '1'] = 0;
        }
    }
    else if (!strcmp(input, "exit"))
    {
        exit(EXIT_SUCCESS);
    }
    else if (!strcmp(input, "kosaraju"))
    {
        result = kosarajuSCC(*graph);
    }
    return result;
}

int main()
{
    char *result = NULL;
    printf("welcome to our graph factory what would you like to do?\n");
    printf("possible options are:\n\tnewgraph i,j\n\tkosaraju\n\tnewedge i,j\n\tremoveedge i,j\n\texit\n");
    Graph *myGraph = NULL;

    int listener; // Listening socket descriptor

    int newfd;                          // Newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // Client address
    socklen_t addrlen;

    char buf[256] = {0}; // Buffer for client data

    char remoteIP[INET6_ADDRSTRLEN];

    // Start off with room for 5 connections
    // (We'll realloc as necessary)
    int fd_count = 0;
    int fd_size = 5;
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
                    // If listener is ready to read, handle new connection

                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener,
                                   (struct sockaddr *)&remoteaddr,
                                   &addrlen);

                    if (newfd == -1)
                    {
                        perror("accept");
                    }
                    else
                    {
                        add_to_pfds(&pfds, newfd, &fd_count, &fd_size);

                        printf("pollserver: new connection from %s on "
                               "socket %d\n",
                               inet_ntop(remoteaddr.ss_family,
                                         get_in_addr((struct sockaddr *)&remoteaddr),
                                         remoteIP, INET6_ADDRSTRLEN),
                               newfd);
                    }
                }
                else
                {
                    // If not the listener, we're just a regular client
                    int nbytes = recv(pfds[i].fd, buf, sizeof buf, 0);

                    int sender_fd = pfds[i].fd;

                    if (nbytes <= 0)
                    {
                        // Got error or connection closed by client
                        if (nbytes == 0)
                        {
                            // Connection closed
                            printf("pollserver: socket %d hung up\n", sender_fd);
                        }
                        else
                        {
                            perror("recv");
                        }

                        close(pfds[i].fd); // Bye!

                        del_from_pfds(pfds, i, &fd_count);
                    }
                    else
                    {
                        // We got some good data from a client
                        result = parse(buf, &myGraph, fd_count, pfds, listener, sender_fd);

                        send_to_everyone(fd_count, pfds, listener, sender_fd, result, nbytes);
                    }
                } // END handle data from client
            } // END got ready-to-read from poll()
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!

    return 0;
}

void send_to_everyone(int fd_count, struct pollfd *pfds, int listener, int sender_fd, char *result, int nbytes)
{
    for (int j = 0; j < fd_count; j++)
    {
        // Send to everyone!
        int dest_fd = pfds[j].fd;

        // Except the listener and ourselves
        if (dest_fd != listener && dest_fd != sender_fd)
        {
            if (send(dest_fd, result, nbytes, 0) == -1)
            {
                perror("send");
            }
        }
    }
}
