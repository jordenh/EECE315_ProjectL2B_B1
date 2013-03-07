CC=gcc
CFLAGS=-g -Wall -std=gnu99

all: bfsh
	@rm -f *.o
bfsh: bfsh.o parser.o directory.o runCommand.o env.o
	$(CC) -o bfsh bfsh.o parser.o directory.o runCommand.o env.o
bfsh.o: bfsh.c bfsh.h parser.h directory.h
	$(CC) $(CFLAGS) -c bfsh.c
parser.o: parser.c parser.h bfsh.h
	$(CC) $(CFLAGS) -c parser.c
directory.o: directory.c directory.h bfsh.h
	$(CC) $(CFLAGS) -c directory.c
runCommand.o: runCommand.c runCommand.h bfsh.h
	$(CC) $(CFLAGS) -c runCommand.c
env.o: env.c env.h bfsh.h
	$(CC) $(CFLAGS) -c env.c
	
clean:
	rm -f *o bfsh

