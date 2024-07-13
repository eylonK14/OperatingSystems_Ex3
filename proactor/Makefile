CC = gcc
CFLAGS = -pg -g -Wall -Werror -Wextra -lpthread
OBJS = kosaraju_proactor.o adjmatrix.o deque.o pollserver.o proactor.o

all: kosaraju_proactor

kosaraju_proactor: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

*.o: *.c
	$(CC) $(CFLAGS) -c $<


.PHONY: clean all

clean:
	rm -f kosaraju_proactor *.o gmon.out
