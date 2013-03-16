#Compiler
CC := gcc

#Executable to compile
EXECUTABLE := bfsh 

#Compiler Flags
CFLAGS := -std=gnu99 -Wall -g

#Linker Flags
LDFLAGS := -g

#Source files
SRC := $(wildcard src/*.c)

#Object Files 
OBJ := $(subst src/, obj/, $(SRC:.c=.o))

#Prevent echoing of commands
.SILENT :

#Rule to link executable
$(addprefix bin/, $(EXECUTABLE)) : $(OBJ)
	echo "	LD	$@"
	mkdir -p bin/
	$(CC) $(LDFLAGS) -o $@ $(OBJ)
	echo "Build Succeeded"

#Rule to generate .d and .o files from .cpp files
obj/%.o : src/%.c
	echo "	CC	$<"
	mkdir -p obj/
	$(CC) $(CFLAGS) -o $@ -c -MMD -MP $<

#include dependency files
-include $(patsubst src/%.c, obj/%.d, $(SRC))  

#Rule to clean generated files
.PHONY : clean
clean  :
	$(RM) -r bin/ obj/

