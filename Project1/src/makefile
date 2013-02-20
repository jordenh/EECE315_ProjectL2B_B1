CC=gcc
CFLAGS=-g -Wall -std=gnu99

all: bfsh

bfsh: bfsh.o parser.o directory.o
	$(CC) -o bfsh bfsh.o parser.o directory.o
bfsh.o: bfsh.c bfsh.h parser.h directory.h
	$(CC) $(CFLAGS) -c bfsh.c
parser.o: parser.c parser.h bfsh.h
	$(CC) $(CFLAGS) -c parser.c
directory.o: directory.c directory.h bfsh.h
	$(CC) $(CFLAGS) -c directory.c

clean:
	rm -rf *o bfsh

