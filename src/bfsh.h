#ifndef BFSH_H
#define BFSH_H

#define BUFFER 300
#define TRUE 1
#define FALSE 0

typedef struct command_t {
    int argc;
    char* name;
    char* argv[100]; //name is repeated in argv[0]
} command;

#endif
