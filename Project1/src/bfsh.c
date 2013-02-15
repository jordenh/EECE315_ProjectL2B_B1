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

int runCommand(command * nextCommand);

void clear_buffers(command *nextCommand){
	for(int i=0;i<=nextCommand->argc;i++){
		nextCommand->argv[i][0] = '\0';
	}
	nextCommand->name[0] = '\0';	
        	
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

	for(int i=0; i<nextCommand->argc+1; i++)	
		printf("-%s- \n", nextCommand->argv[i]);
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

		runCommand(&nextCommand);	
	}
	
	free(tempStr);
	free(cwd);
	free(nextCommand.name);
	for(int i=0;i<100;i++){
		free(nextCommand.argv[i]);
	}    

	return 0;
}


int runCommand(command * nextCommand)
{
    pid_t pid;
    //testing execvp
    pid = fork();
    if(pid == 0){ //child Process - run program
	    printf("****I am in the child Process, with ID %d*****\n",pid);
        nextCommand->argv[nextCommand->argc] = '\0'; // force next argv to NULL char
        printf("name:--%s--\nargv[0]:--%s--\nargv[1]:--%s--\nargv[2]:--%s--\n",nextCommand->name,nextCommand->argv[0],nextCommand->argv[1],nextCommand->argv[2]);

        execvp(nextCommand->name, nextCommand->argv); 
        perror("execvp failed to run nextCommand with execvp");	 //program should never reach this line, as execvp should stray this thread   
    }
	else if(pid > 0){ //Parent Process 
        wait((int*)0);
		printf("I have forked, and am the parent with ID %d\n",pid);
	}
    else{
        printf("error, child was not created properly\n");
    }

    return 1;
}

