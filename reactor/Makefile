CC = gcc
CFLAGS = -pg -g -Wall -Werror -Wextra -fprofile-arcs -ftest-coverage
OBJS = kosaraju_reactor.o adjmatrix.o deque.o pollserver.o reactor.o

all: kosaraju_reactor

kosaraju_reactor: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

*.o: *.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f kosaraju_reactor *.o gmon.out *.gcov *.gcda *.gcno