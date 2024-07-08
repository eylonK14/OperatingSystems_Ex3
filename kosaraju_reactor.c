#include "prof.h"

#ifdef TWO_DIM_ARR
#include "twoDimArr.h"
#endif // TWO_DIM_ARR

#ifdef DEQUE
#include "deque.h"
#endif // DEQUE

#include "reactor.h"
#include "pollserver.h"

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
    char *result = (char *)malloc(sizeof(char) * 256);
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
        result = kosarajuSCC(*graph);
    return result;
}

void send_to_everyone(int fd_count, struct pollfd *pfds, int listener, int sender_fd, char *result, int nbytes)
{
    for (int j = 0; j < fd_count; j++)
    {
        // Send to everyone!
        int dest_fd = pfds[j].fd;

        // Except the listener and ourselves
        if (dest_fd != listener && dest_fd != sender_fd && send(dest_fd, result, nbytes, 0) == -1)
            perror("send");
    }
}

int main()
{
    char *result = NULL;
    int edge_counter = 0, n = 0, m = 0, newfd = 0;
    Graph *myGraph = NULL;

    struct sockaddr_storage remoteaddr; // Client address
    socklen_t addrlen;

    char buf[256] = {0}; // Buffer for client data
    char remoteIP[INET6_ADDRSTRLEN];

    reactor *myReactor = startReactor();

    printf("Welcome to our graph factory! What would you like to do?\n");
    printf("Options are:\n\tnewgraph i,j\n\tkosaraju\n\tnewedge i,j\n\tremoveedge i,j\n\texit\n");

    // Main loop
    for (;;)
    {
        struct pollfd *pfds = malloc(sizeof *pfds * myReactor->capacity);
        const size_t noOfHandles = buildPollArray(pfds);
        int poll_count = poll(pfds, noOfHandles, -1);

        if (poll_count == -1)
        {
            perror("poll");
            exit(1);
        }

        // Run through the existing connections looking for data to read
        for (size_t i = 0; i < noOfHandles; i++)
        {
            // Check if someone's ready to read
            if (pfds[i].revents & POLLIN)
            { // We got one!!
                if (pfds[i].fd == listener)
                {
                    // If listener is ready to read, handle new connection

                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);

                    printf("got connection\n");

                    if (newfd == -1)
                        perror("accept");
                    else
                    {
                        add_to_pfds(&pfds, newfd, &fd_count, &fd_size);

                        printf("pollserver: new connection from %s on socket %d\n",
                               inet_ntop(remoteaddr.ss_family,
                                         get_in_addr((struct sockaddr *)&remoteaddr),
                                         remoteIP, INET6_ADDRSTRLEN),
                               newfd);
                    }
                }
                else
                {
                    for (int i = 0; i < 256; i++)
                        buf[i] = 0;
                    // If not the listener, we're just a regular client
                    int nbytes = recv(pfds[i].fd, buf, sizeof buf, 0);

                    printf("got message\n");

                    int sender_fd = pfds[i].fd;

                    if (nbytes <= 0)
                    {
                        // Got error or connection closed by client
                        if (nbytes == 0) // Connection closed
                            printf("pollserver: socket %d hung up\n", sender_fd);
                        else
                            perror("recv");

                        close(pfds[i].fd); // Bye!

                        del_from_pfds(pfds, i, &fd_count);
                    }
                    else
                    {
                        // We got some good data from a client
                        result = parse(buf, &myGraph, &edge_counter, &n, &m);

                        send_to_everyone(fd_count, pfds, listener, sender_fd, result, strlen(result));
                    }
                } // END handle data from client
            } // END got ready-to-read from poll()
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!

    return 0;
}