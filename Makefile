CC=gcc -g -Wall
CFLAGS=-c
all: program
program: main.o
	$(CC) main.o -o mysh

main.o: main.c
	$(CC) $(CFLAGS) main.c

clean:
	rm -rf *.o *.out mysh
