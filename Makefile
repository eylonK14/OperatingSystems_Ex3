CC = gcc
CFLAGS = -pg -g -Wall -Wextra -Werror -lc -fprofile-arcs -ftest-coverage -lpthread

all:
	$(MAKE) -C standard all
	$(MAKE) -C threaded all
	$(MAKE) -C reactor all
	$(MAKE) -C proactor all
	$(MAKE) -C producer_consumer all

clean:
	$(MAKE) -C standard clean
	$(MAKE) -C threaded clean
	$(MAKE) -C reactor clean
	$(MAKE) -C proactor clean
	$(MAKE) -C producer_consumer clean