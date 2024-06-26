CC = gcc
CFLAGS = -pg -g -Wall -Werror -Wextra
OBJS = kosaraju.o adjmatrix.o deque.o pollserver.o

all: kosaraju

kosaraju: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

*.o: *.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f kosaraju *.o gmon.out

.PHONY: clean all
