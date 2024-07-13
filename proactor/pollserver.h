#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

#define PORT "9034" // Port we're listening on

void *get_in_addr(struct sockaddr *);

int get_listener_socket();

void add_to_pfds(struct pollfd *[], int, int *, int *);

void del_from_pfds(struct pollfd[], int, int *);
