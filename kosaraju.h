#pragma once

void send_to_everyone(int fd_count, struct pollfd *pfds, int listener, int sender_fd, char *result, int nbytes);

void parseCommand(char *input, char **command, int word_count);
