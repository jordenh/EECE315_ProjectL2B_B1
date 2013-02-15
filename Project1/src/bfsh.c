#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>

#define BUFFER 300
#define TRUE 1
#define FALSE 0

typedef struct command_t {
    int argc;
    char* name;
    char* argv[99]; //name is repeated in argv[0]
} command;

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
		printf("%s \n", nextCommand->argv[i]);
	free(token);
}

void update_CWD(char* cwd) {
	
	char* homeDirectory;
	getcwd(cwd, BUFFER);
	homeDirectory = getenv("HOME");

	if(memcmp(cwd, homeDirectory, sizeof(homeDirectory)) == 0) {
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
	char *cwd = (char*)malloc(nbytes+1);
	char *tempStr = (char*)malloc(nbytes+1);
	char CWD[BUFFER];
	command nextCommand ;
	nextCommand.argc = 0;
	nextCommand.name = (char*)malloc(nbytes+1);
	
	for(int i=0;i<100;i++){
		nextCommand.argv[i] = (char*)malloc(nbytes+1);
	}    


	while(!exitBool){
		clear_buffers(&nextCommand);
		update_CWD(CWD);
       		printf("cmd:%s ", CWD);
		fgets(tempStr, (nbytes+1), stdin);
		parse_arguments(&nextCommand, tempStr);	
	}
	
	free(tempStr);
	free(cwd);
	free(nextCommand.name);
	for(int i=0;i<100;i++){
		free(nextCommand.argv[i]);
	}    

	return 0;
}
