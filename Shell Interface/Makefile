CC = gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow -pedantic-errors \
	-fstack-protector-all -Wextra

PROGS = d8sh

.PHONY: all clean

# Call target d8sh
all: $(PROGS)

d8sh: lexer.o parser.tab.o executor.o d8sh.o
	$(CC) -lreadline lexer.o parser.tab.o executor.o d8sh.o -o d8sh

# Compile source files into object files
lexer.o: lexer.c
	$(CC) $(CFLAGS) -c lexer.c

parser.tab.o: parser.tab.c command.h
	$(CC) $(CFLAGS) -c parser.tab.c

executor.o: executor.c executor.h command.h
	$(CC) $(CFLAGS) -c executor.c

d8sh.o: d8sh.c executor.h lexer.h
	$(CC) $(CFLAGS) -c d8sh.c

# Clean
clean:
	rm -f *.o $(PROGS)
