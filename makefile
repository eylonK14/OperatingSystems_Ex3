CC = gcc
CFLAGS = -pg -Wall -Werror -Wextra
OBJS = kosaraju.o adjmatrix.o deque.o

all: kosaraju

kosaraju: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

*.o: *.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f kosaraju *.o gmon.out

.PHONY: clean all
