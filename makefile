CC = gcc
CFLAGS = -Wall -g -Werror -Wextra

all: kosaraju

kosaraju: kosaraju.o
	$(CC) $(CFLAGS) -o kosaraju kosaraju.o

kosaraju.o: kosaraju.c
	$(CC) $(CFLAGS) -c kosaraju.c

clean:
	rm -f kosaraju kosaraju.o

.PHONY: clean all
