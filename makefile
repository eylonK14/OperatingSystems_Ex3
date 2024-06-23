CC = gcc
CFLAGS = -g -Wall -Werror -Wextra
OBJS = kosaraju.o adjmatrix.o

all: kosaraju

kosaraju: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

*.o: *.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f kosaraju *.o

.PHONY: clean all
