CC = gcc
CFLAGS = -pg -g -Wall -Werror -Wextra -lpthread
OBJS = kosaraju_threaded.o adjmatrix.o deque.o pollserver.o

all: kosaraju_threaded

kosaraju_threaded: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

*.o: *.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f kosaraju_threaded *.o gmon.out

.PHONY: clean all
