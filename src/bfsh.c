#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include "bfsh.h"
#include "parser.h"
#include "directory.h"
#include "runCommand.h"
#include "env.h"

static int DEBUG = 0;

int main(void){
	int exitBool = FALSE;
	int nbytes = BUFFER;
	int command_count = 0;
	char CWD[BUFFER];
	char *user = NULL;
	char *hostname = malloc(nbytes+1);
	char *tempStr = malloc(nbytes+1);
	command nextCommand ;
	nextCommand.argc = 0;
	nextCommand.name = malloc(nbytes+1);
	
	for(int i = 0; i < 100; i++){
		nextCommand.argv[i] = malloc(nbytes+1);
	}    


	while(!exitBool){
		//Update current working directory	
		update_CWD(CWD);
       
		//Get hostname and username for prompt
		gethostname(hostname, BUFFER);
		user = getenv("USER");
       	
		//User prompt	
		printf("[%d][%s@%s %s]$ ", command_count, user, hostname, CWD);
	
		//Read user input	
		fgets(tempStr, (nbytes+1), stdin);
        	tempStr[strlen(tempStr)-1] = '\0'; // remove the carriage return from the input
	
		//Parse user input	
		parse_arguments(&nextCommand, tempStr);
		
		if (strcasecmp(nextCommand.name, "cd") == 0) {
			if(DEBUG == 1){
			    printf("CD command selected\n");
			}
			cd_Command(nextCommand.argv[1]);
		} else if (strcasecmp(nextCommand.name, "set") == 0) {
  			if(DEBUG == 1){
  				printf("set command selected\n");
			}
			setenv_Command(nextCommand.argv[1], nextCommand.argv[2], nextCommand.argc);
  		} else if (strcasecmp(nextCommand.name, "unset") == 0) {			
  			if(DEBUG == 1){
  				printf("unset command selected\n");
  			}
			unsetenv_Command(nextCommand.argv[1]);
  		} else if (strcasecmp(nextCommand.name, "get") == 0) {
  			if(DEBUG == 1){
  				printf("get command selected\n");
  			}
			getenv_Command(nextCommand.argv[1]);
		} else if (strcasecmp(nextCommand.name, "q") == 0 || strcasecmp(nextCommand.name, "quit") == 0 || strcasecmp(nextCommand.name, "exit") == 0){ 
			exitBool = 1;
		} else if (nextCommand.name[0] != '\0') {  // execute program if there is a command to run
			runCommand(&nextCommand);
		}
				command_count++;
	}

	//Free dynamically allocated memory	
	free(hostname);
	free(tempStr);
	free(nextCommand.name);
	for(int i = 0; i < 100; i++){
		free(nextCommand.argv[i]);
	}    
	

	return 0;
}
