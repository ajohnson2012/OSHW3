CC=gcc
CFLAGS=-c
all: program
program: main.o
	$(CC) main.o -o shell

main.o: main.c
	$(CC) $(CFLAGS) main.c

clean:
	rm -rf *.o *.out shell
