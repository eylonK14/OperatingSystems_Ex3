CC = gcc
CFLAGS = -pg -g -Wall -Werror -Wextra -lpthread
OBJS = kosaraju_producer_consumer.o adjmatrix.o deque.o pollserver.o proactor.o

all: kosaraju_producer_consumer

kosaraju_producer_consumer: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

*.o: *.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f kosaraju_producer_consumer *.o gmon.out *.gcov *.gcda *.gcno
	
