# Variables
CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -lc -fprofile-arcs -ftest-coverage
LDFLAGS = -static
LIBNAME = libmylib.a

# Object files for the library
OBJ_LIB_FILES = parser.o tcpHandler.o udpHandler.o udsHandler.o

# Object files for executables
OBJ_EXEC_FILES = ttt.o mync.o

# Default target
all: lib executables

# Static library
$(LIBNAME): $(OBJ_LIB_FILES)
	ar rcs $(LIBNAME) $(OBJ_LIB_FILES) -lc
	ranlib $(LIBNAME)

# Compile each.c file into an object file (.o)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for creating the static library
lib: $(LIBNAME)

# Rule for creating executables
executables: $(OBJ_EXEC_FILES)
	$(CC) $(CFLAGS) -o ttt ttt.o
	$(CC) $(CFLAGS) -o mync mync.o $(LIBNAME)

# Clean up
clean:
	rm -f $(OBJ_LIB_FILES) $(OBJ_EXEC_FILES) $(LIBNAME) ttt mync *.gcda *.gcno *.gcov

.PHONY: clean all