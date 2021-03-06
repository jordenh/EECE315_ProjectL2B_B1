#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "bfsh.h"
#include "runCommand.h"

static int MAXNUMBACKGROUNDPROCS = 10;
static int numProcessInBack = 0;
static int processIDsInBack[10] = {-2,-2,-2,-2,-2,-2,-2,-2,-2,-2}; //initialize all with -2, as that will be the "not active process ID" slot indicator
static char* processNamesInBack[10];

static int DEBUG = 0;

int runCommand(command * nextCommand)
{
    pid_t pid;
    int numPathVars = 0;
    int status;
    int startIndexPathString; 
    unsigned int foundProgramBool = 0;
    char* pathStrings[25];
    char* pathString = malloc(BUFFER + 1);
    char* token = NULL;
    char* concatenatedAbsPath = malloc(BUFFER + 1);
    
    for(int i = 0; i < 25; i++){
        pathStrings[i] = malloc(BUFFER + 1); 
    }

    strcpy(pathString,getenv("PATH"));

    if(DEBUG==1){
        printf("You're pathString val is: %s\n",pathString);
    }

    token = strtok(pathString, ":");
    while(token){
	    strcpy(pathStrings[numPathVars], token);
	    numPathVars++;
	    token = strtok(NULL, ":");
    }
    
    //NOTE: numPathVars is 1 too high, so on last pass of the search loop within the child, rather than searching a Path location, search CWD.
    
    pid = fork();
	
    if(pid == 0) { // child process
	    if(strncmp(nextCommand->argv[(nextCommand->argc)], "&", 1) == 0) {//background process
	        if(numProcessInBack == MAXNUMBACKGROUNDPROCS){
	            printf("Error: cannot create more than %d background processes\n", MAXNUMBACKGROUNDPROCS);
	            abort();
	        }
	        
	        free(nextCommand->argv[(nextCommand->argc)]); // Free child's memory at this location, before it gets changed.
		nextCommand->argv[(nextCommand->argc)] = '\0'; // force "&" argv to NULL char, on Child 
	    }
	
		for(int i = numPathVars; i >= 0; i--){  //search through different directories to discover which one has the desired program
			if(i == numPathVars){
				strcpy(&concatenatedAbsPath[0],nextCommand->name); //search CWD, since this index holds no path information      
			}
			else{ 
				strcpy(concatenatedAbsPath,pathStrings[i]);
				startIndexPathString = strlen(pathStrings[i]);
				concatenatedAbsPath[startIndexPathString] = '/';
				strcpy(&(concatenatedAbsPath[startIndexPathString + 1]) ,nextCommand->name);
			}
		
			if(DEBUG == 1){
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
		
        if(DEBUG == 1){
            printf("***Attempting to use execv, in %s***\n",concatenatedAbsPath);
        }
        free(nextCommand->argv[nextCommand->argc + 1]); // Free child's memory at this location, before it gets changed.
        nextCommand->argv[nextCommand->argc + 1] = '\0'; // force next argv to NULL char, on Child 
     
        if(execv(concatenatedAbsPath, nextCommand->argv) == -1){
            printf("ERROR, program did not execute properly: aborting.\n");                  
            abort();
        }
        
        return -1; //should never hit this line
    }
    else if(pid > 0) { //Parent Process     
    
    	//Index Background Processes in arrays
        if(strncmp(nextCommand->argv[(nextCommand->argc)], "&", 1) == 0) {//background process
            if(numProcessInBack!= MAXNUMBACKGROUNDPROCS){// dont index process ID if there are already MANUMBACKGROUNDPROCS created
	            for(int processCount = 0; processCount < 10; processCount++){//find first free slot (-2) in processIDsInBack
	                if(processIDsInBack[processCount] == -2){
	                    processIDsInBack[processCount] = pid; 
	                    processNamesInBack[processCount] = malloc(strlen(nextCommand->name) + 2);
	                    strncpy(processNamesInBack[processCount], nextCommand->name, (strlen(nextCommand->name) + 1));
	                    numProcessInBack++;
	                    printf("[%d] %d\n",processCount+1, pid);   
	                    break;
	                }
	            }   
	        }
	        
	    }
        else{//foreground process - wait until finished to return
            waitpid(pid, &status,0); // hold for child that was just spawned
            if(DEBUG == 1){
                printf("status=%d , pid=%d\n",status,pid);
            }
        }  
        
        //Determine if currently indexed background processes have finished and remove from index if they have
        if(numProcessInBack > 0){
            for(int processCount = 0; processCount < MAXNUMBACKGROUNDPROCS; processCount++){
                if(processIDsInBack[processCount] != -2){ //it's a valid ID
                    if(waitpid(processIDsInBack[processCount], &status, WNOHANG) == processIDsInBack[processCount]){
                        //background process finished
                        printf("[%d]\tDone\t\t%d - %s\n", processCount+1, processIDsInBack[processCount], processNamesInBack[processCount]);
                        free(processNamesInBack[processCount]);
                        numProcessInBack--;
                        processIDsInBack[processCount] = -2; //set back to invalid process ID
                    }
                    else if(waitpid(processIDsInBack[processCount], &status, WNOHANG) == -1){
                        printf("Error has occured in waitPID - research further\n");
                    }
                    else if(waitpid(processIDsInBack[processCount], &status, WNOHANG) == 0){
                        //process still running
                    }
                    else{
                        //process changed state in some other way
                        printf("[%d]\tChangedState\t%d\n", processCount+1, processIDsInBack[processCount]);
                    }
                
                }
            }
        
        }
        
    }
    else{
        printf("error, child was not created properly\n");
    } 
    
    if(DEBUG == 1 && numProcessInBack > 0){
        printf("numProcessInBack= %d\n", numProcessInBack);
        for(int processCount = 0; processCount < 10; processCount++){
           printf("processIDsInBack[%d]= %d\n", processCount, processIDsInBack[processCount]); 
        }
    }
    
    free(concatenatedAbsPath);
    for(int i = 0; i < 25; i++){
        free(pathStrings[i]);
    }
    free(pathString);

    return 0;
}
