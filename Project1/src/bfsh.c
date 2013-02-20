#include "bfsh.h"
#include "parser.h"
#include "directory.h"

#define BUFFER 300
#define TRUE 1
#define FALSE 0
static int MAXNUMBACKGROUNDPROCS = 10;
static int DEBUG = 0; //1=on,0=off
static int numProcessInBack = 0;
static int processIDsInBack[10] = {-2,-2,-2,-2,-2,-2,-2,-2,-2,-2}; //initialize all with -2, as that will be the "not active process ID" slot indicator
static char* processNamesInBack[10];

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
		nextCommand.argc = 0;
		update_CWD(CWD);
       	
		gethostname(hostname, BUFFER);
		user = getenv("USER");
       		printf("[%s@%s %s]$ ", user, hostname, CWD);
		
		fgets(tempStr, (nbytes+1), stdin);
        	tempStr[strlen(tempStr)-1] = '\0'; // remove the carriage return from the input
		parse_arguments(&nextCommand, tempStr);
		
		if (strcasecmp(nextCommand.name, "cd") == 0) {
			if(DEBUG==1){
			    printf("CD command selected\n");
			}
			cd_Command(nextCommand.argv[1]);
		} else if (strcasecmp(nextCommand.name, "q") == 0 || strcasecmp(nextCommand.name, "quit") == 0) {
			exitBool = 1;
		} else if (nextCommand.name[0] != '\0') {  // execute program if there is a command to run
			runCommand(&nextCommand);
		}
	}
	
	free(tempStr);
	free(cwd);
	free(nextCommand.name);
	for(int i=0;i<100;i++){
		free(nextCommand.argv[i]);
	}    
	

	return 0;
}
