#ifndef BFSH_h
#define BFSH_h

typedef struct command_t {
    int argc;
    char* name;
    char* argv[99]; //name is repeated in argv[0]
} command;

#endif
