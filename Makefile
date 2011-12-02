CC=gcc -g -Wall
CFLAGS=-c
all: program
program: main.o filereader.o
	$(CC) main.o filereader.o -o mysh

main.o: main.c
	$(CC) $(CFLAGS) main.c

filereader.o: filereader.c
	$(CC) $(CFLAGS) filereader.c
clean:
	rm -rf *.o *.out mysh
