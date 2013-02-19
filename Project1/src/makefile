CC=gcc
CFLAGS=-g -Wall -std=gnu99

all: bfsh

bfsh: bfsh.o parser.o
	$(CC) -o bfsh bfsh.o parser.o
bfsh.o: bfsh.c bfsh.h parser.h
	$(CC) $(CFLAGS) -c bfsh.c
parser.o: parser.c parser.h bfsh.h
	$(CC) $(CFLAGS) -c parser.c

clean:
	rm -rf *o bfsh

