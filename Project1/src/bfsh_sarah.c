#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER 300
#define TRUE 1
#define FALSE 0

typedef struct command_t {
    int argc;
    char* name;
    char* argv[99]; //name is repeated in argv[0]
} command;

void cd_Command(char* argument){
	char* homeDirectory;
	char cwd[BUFFER];
	char tmpstring[300];
	int i = 0;
	int count = 0;
	homeDirectory = getenv("HOME");
	getcwd(cwd, BUFFER);	

	if (strncmp(argument, "..", 2) == 0) {
		printf("../.. chosen\n");
		while (strncmp(argument+3*i, "..", 2) == 0 && strncmp(argument+3*i+2, "/", 1) == 0) {
			count++;
			i++;
		}
		if(strncmp(argument+3*i, "..", 2) == 0) {
			count++;		
		}
		i = strlen(cwd);
		do {
			if (cwd[i-1] == '/') {			
				count--;
			}
			cwd[i-1] = '\0';
			i--;
		} while(count > 0);
		if(cwd[0] == '\0') {
			chdir("/");
		} else {
			chdir(cwd);	
		}
			
	} else if (strncmp(argument, "~", 1) == 0) {
		while (strncmp(argument+count, "~", 1) == 0 || strncmp(argument+count, "/", 1) == 0) {
			count++;
		}
		strncpy (tmpstring, argument+count, strlen(argument)-count);
		if (chdir(homeDirectory) == 0) {
			if (chdir(tmpstring) == 0) {
			} else {
				printf("Failure\n");
			}
		} else {
			printf("Failure\n");
		}
	} else {	
		if (chdir(argument) == 1) {
			printf("Failure\n");
		}
	}
	homeDirectory = NULL;
	return;
}

void clear_buffers(command *nextCommand){
	for(int i=0;i<=nextCommand->argc;i++){
		for(int j=0;j<BUFFER;j++){
			nextCommand->argv[i][j] = '\0'; //replace all of allocated buffer size to null
		}
	}
	for(int i=0;i<BUFFER;i++){
		nextCommand->name[i] = '\0';
	}	
        	
	nextCommand->argc = 0;
}

void parse_arguments(command *nextCommand, char *tempStr){
	char* token = (char*)malloc(BUFFER+1);
	
	token = strtok(tempStr, " ");
	while(token){
		if(nextCommand->argc == 0){
			strcpy(nextCommand->name, token);
			strcpy(nextCommand->argv[nextCommand->argc], token);
		}
		else
			strcpy(nextCommand->argv[nextCommand->argc], token);
		
		nextCommand->argc++;
		token = strtok(NULL, " ");
	}
		
	for(int i=0; i<nextCommand->argc; i++)	
		printf("%i: -%s- \n", i, nextCommand->argv[i]);
	free(token);
}

void update_CWD(char* cwd) {
	char* homeDirectory;
	getcwd(cwd, BUFFER);
	homeDirectory = getenv("HOME");

	if(strncmp(cwd, homeDirectory, sizeof(homeDirectory)) == 0) {
		cwd[2*(sizeof(homeDirectory)-2)] = '~';
		for (int i=0;i<BUFFER - 2*(sizeof(homeDirectory)-2);i++) {
			cwd[i] = cwd[i + 2*(sizeof(homeDirectory)-2)];
		}
	}

	return;
}

int main(void){
	int exitBool = FALSE;
	int nbytes = BUFFER;
	char *tempStr = (char*)malloc(nbytes+1);
	char CWD[BUFFER];
	char *hostname = (char*)malloc(nbytes+1);
	char *user = (char*)malloc(nbytes+1);

	command nextCommand ;
	nextCommand.argc = 0;
	nextCommand.name = (char*)malloc(nbytes+1);
	
	for(int i=0;i<100;i++){
		nextCommand.argv[i] = (char*)malloc(nbytes+1);
	}    


	while(!exitBool){
		clear_buffers(&nextCommand);
		update_CWD(CWD);
		gethostname(hostname, BUFFER);
		user = getenv("USER");

       		printf("[%s@%s %s]$ ", user, hostname, CWD);
		fgets(tempStr, (nbytes+1), stdin);
		tempStr[strlen(tempStr)-1] = '\0'; // remove the carriage return from the input
		parse_arguments(&nextCommand, tempStr);

		if (strcasecmp(nextCommand.name, "cd") == 0) {
			printf("CD command selected\n");
			cd_Command(nextCommand.argv[1]);
		} else if (strcasecmp(nextCommand.name, "q") == 0 || strcasecmp(nextCommand.name, "quit") == 0) {
			exitBool = 1;
		} else {
			printf("No command selected\n");
		}
	}
	
	free(tempStr);
	free(nextCommand.name);
	for(int i=0;i<100;i++){
		free(nextCommand.argv[i]);
	}    

	return 0;
}