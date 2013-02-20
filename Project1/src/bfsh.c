#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
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

int runCommand(command * nextCommand);

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
		nextCommand.name[0] = '\0'; // ensure name doesnt repeat, to stop incorrect cd calls
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


int runCommand(command * nextCommand)
{
    if(nextCommand->name[0] == '\0' || nextCommand->argv[0] == '\0'){
        //no proper command to execute
        return 0;
    }
    
    pid_t pid;
    
    char* pathStrings[25];
    for(int i=0;i<25;i++){
        pathStrings[i] = (char*)malloc(BUFFER+1); 
    }
    char* pathString = (char*)malloc(BUFFER+1);
    strcpy(pathString,getenv("PATH"));
    if(DEBUG==1){
        printf("You're pathString val is: %s\n",pathString);
    }
    char* token = (char*)malloc(BUFFER+1);
	
	int numPathVars = 0;
	token = strtok(pathString, ":");
	while(token){
	    strcpy(pathStrings[numPathVars], token);
	    numPathVars++;
		token = strtok(NULL, ":");
	}
	free(token);
	//NOTE: numPathVars is 1 too high, so on last pass of fork, rather than searching a Path location, search CWD.
    
    char* concatenatedAbsPath = (char*)malloc(BUFFER);
    int status;
	pid = fork();
	
	
	if(pid == 0) {
	    if(strncmp(nextCommand->argv[(nextCommand->argc)-1], "&", 1) == 0) {//background process
	        if(numProcessInBack==MAXNUMBACKGROUNDPROCS){
	            printf("Error: cannot create more than %d background processes\n", MAXNUMBACKGROUNDPROCS);
	            abort();
	        }
	        
	        free(nextCommand->argv[(nextCommand->argc)-1]); // Free child's memory at this location, before it gets changed.
            nextCommand->argv[(nextCommand->argc)-1] = '\0'; // force "&" argv to NULL char, on Child 
	    }
	
		unsigned int foundProgramBool = 0;
		for(int i=numPathVars;i>=0;i--){  //search through different directories to discover which one has the desired program
			strcpy(concatenatedAbsPath,pathStrings[i]);
			int startIndexPathString = strlen(pathStrings[i]);
			concatenatedAbsPath[startIndexPathString] = '/';
			strcpy(&(concatenatedAbsPath[startIndexPathString + 1]) ,nextCommand->name);
			
			if(i==numPathVars){
				strcpy(&concatenatedAbsPath[0],nextCommand->name); //search CWD, since this index holds no path information      
			} 
			if(DEBUG==1){
				printf("concatenatedAbsPath = --%s--\n",concatenatedAbsPath);
			}
			
			struct stat tempBuf; //temporary buffer, so that "stat" will have enough args
			if(stat(concatenatedAbsPath, &tempBuf) >= 0){ //returns non-negative if program is found
				if(DEBUG == 1){
					printf("the program was found in path = --%s--\n",concatenatedAbsPath);
				}
				foundProgramBool = 1;
				break;
			}
		}
		
		if(foundProgramBool == 0){
			printf("%s: Command not found.\n", nextCommand->name);
			abort();
		}
		
        if(DEBUG==1){
            printf("***Attempting to use execv, in %s***\n",concatenatedAbsPath);
        }
        free(nextCommand->argv[nextCommand->argc]); // Free child's memory at this location, before it gets changed.
        nextCommand->argv[nextCommand->argc] = '\0'; // force next argv to NULL char, on Child 
     
        if(execv(concatenatedAbsPath,nextCommand->argv) == -1){
            printf("ERROR, program did not execute properly: aborting.\n");                  
            abort();
        }
        
        return -1; //should never hit this line
    }
    else if(pid > 0) { //Parent Process     
        if(strncmp(nextCommand->argv[(nextCommand->argc)-1], "&", 1) == 0) {//background process
	        processIDsInBack[numProcessInBack] = pid; 
	        numProcessInBack++;
	        printf("[%d] %d\n",numProcessInBack, pid);
	    }
        else{//foreground process
            waitpid(pid,&status,0); // hold for child that was just spawned
            if(DEBUG==1){
                printf("status=%d , pid=%d\n",status,pid);
            }
        }  
        
        if (numProcessInBack > 0){
            for(int processCount =0; processCount<10; processCount++){
                if(processIDsInBack[processCount] != -2){ //it's a valid ID
                    if(waitpid(processIDsInBack[processCount], &status, WNOHANG) == 0){
                        //process still running
                    }
                    else{
                        //process finished
                        numProcessInBack--;
                        printf("[%d]\tDone\t\t\t%d\n",processCount,processIDsInBack[processCount]);
                        processIDsInBack[processCount] = -2; //set back to invalid process ID
                    }
                
                }
            }
        
        }
        
    }
    else{
        printf("error, foreground child was not created properly\n");
    } 
    
    if (DEBUG ==1 && numProcessInBack > 0){
        printf("numProcessInBack= %d\n",numProcessInBack);
        for(int processCount =0; processCount<10; processCount++){
           printf("processIDsInBack[%d]= %d\n",processCount, processIDsInBack[processCount]); 
        }
    }
    
    free(concatenatedAbsPath);
    for(int i=0;i<25;i++){
        free(pathStrings[i]);
    }
    free(pathString);

    return 0;
}
