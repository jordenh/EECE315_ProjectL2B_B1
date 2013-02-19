#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER 300
#define TRUE 1
#define FALSE 0
static int DEBUG = 0; //1=on,0=off

typedef struct command_t {
    int argc;
    char* name;
    char* argv[99]; //name is repeated in argv[0]
} command;

int runCommand(command * nextCommand);
void cd_Command(char* argument);

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
    if(DEBUG==1){
    	for(int i=0; i<nextCommand->argc+1; i++)	
	    	printf("-%s- \n", nextCommand->argv[i]);
	}
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
		} else {  // execute program
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
    
    pid_t pid;
    
    
    //testing execvp
/*    pid = fork();
    if(pid == 0){ //child Process - run program
	    printf("****I am in the child Process, with ID %d*****\n",pid);
	    free(nextCommand->argv[nextCommand->argc]); // Free child's memory at this location, before it gets changed.
        nextCommand->argv[nextCommand->argc] = '\0'; // force next argv to NULL char, on Child
        printf("name:--%s--\nargv[0]:--%s--\nargv[1]:--%s--\nargv[2]:--%s--\n",nextCommand->name,nextCommand->argv[0],nextCommand->argv[1],nextCommand->argv[2]);

        execvp(nextCommand->name, nextCommand->argv); 
        perror("execvp failed to run nextCommand with execvp");	 //program should never reach this line, as execvp should stray this thread  
        return -1; 
    }
	else if(pid > 0){ //Parent Process 
        wait((int*)0);
		printf("I have forked, and am the parent with ID %d\n",pid);
	}
    else{
        printf("error, child was not created properly\n");
    } */


    char* tempCWD = (char*)malloc(BUFFER+1);
    getcwd(tempCWD,BUFFER+1);
    
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
	//NOTE: numPathVars is 1 too high, so on last pass of fork, rather than searching a Path location, search CWD.

    char* concatenatedAbsPath = (char*)malloc(BUFFER);
    unsigned int boolExecutedProg = 0;
    for(int i=numPathVars;i>=0;i--){
        strcpy(concatenatedAbsPath,pathStrings[i]);
        int startIndexPathString = strlen(pathStrings[i]);
        concatenatedAbsPath[startIndexPathString] = '/';
        strcpy(&(concatenatedAbsPath[startIndexPathString + 1]) ,nextCommand->name);
        if(DEBUG==1){
            //printf("concatenatedAbsPath = --%s--\n",concatenatedAbsPath);
        }
        
        int status;
        pid = fork();
        if(pid == 0) {
            if(i==numPathVars){  //search CWD, since this index holds no path information
                strcpy(&concatenatedAbsPath[0],nextCommand->name);                
            }  
            if(DEBUG==1){
                printf("***Attempting to use execv, in %s***\n",concatenatedAbsPath);
            }
            free(nextCommand->argv[nextCommand->argc]); // Free child's memory at this location, before it gets changed.
            nextCommand->argv[nextCommand->argc] = '\0'; // force next argv to NULL char, on Child 
         
            execv(concatenatedAbsPath,nextCommand->argv);
        
            perror("execv failed to run\n");
            //return -1;  //end child process, return with -1.  
            abort();
              
        }
        else if(pid > 0) { //Parent Process
            wait(&status);
            if(DEBUG==1){
                printf("status=%d , pid=%d\n",status,pid);
            }
            if (status == pid){//child was successful
                boolExecutedProg = 1;
                if(DEBUG==1){
                    printf("I am in the status==pid branch\n");
                }
            }
            else if (status == -1){//child Unsuccessful
                boolExecutedProg = 0;
                if(DEBUG==1){
                    printf("I am in the status==-1 branch\n");
                }
            }
            else if (status==0){
                break; //this seemed to be what was returned when ls was found. . exploring further
            }
            else{
               if(DEBUG==1){
                //printf("I dont think it can get to this line\n");
                }
            }
        }
        else{
            printf("error, child was not created properly\n");
        }    
    }
    
    
    
    
/*    //testing execv
    pid = fork();
    if(pid == 0) {
        printf("\n***Attempting to use execv***\n");
        free(nextCommand->argv[nextCommand->argc]); // Free child's memory at this location, before it gets changed.
        nextCommand->argv[nextCommand->argc] = '\0'; // force next argv to NULL char, on Child
  
//       char* av1[] = {"/bin/ls", tempCWD, "\0"};
//       execv("/bin/ls", av1);   
         
        execv(nextCommand->name,nextCommand->argv);
        
        perror("execv failed to run nextCommand with execv");
        return -1;        
    }
    else if(pid > 0) { //Parent Process
        wait((int*)0);
        printf("I am the parent for my execv test\n");

    }
    else{
        printf("error, child was not created properly\n");
    }
*/


    free(tempCWD);
    free(concatenatedAbsPath);
    printf("\n\n"); 

    return 1;
}


void cd_Command(char* argument){
	char* homeDirectory;
	char cwd[BUFFER];
	char tmpstring[300];
	int i = 0;
	int count = 0;
	homeDirectory = getenv("HOME");
	getcwd(cwd, BUFFER);	

	if (strncmp(argument, "..", 2) == 0) {
		if(DEBUG==1){
	    		printf("../.. chosen\n");
		}
		while (strncmp(argument+3*i, "../", 3) == 0 || strncmp(argument+3*i, "..", 3) == 0) {
			count++;
			i++;
		}
		if(DEBUG==1){
		    printf ("%i : %i :", strlen(argument), count);
		}
		if (strlen(argument) > 3*i) {
			count = 0;
		}
		if(DEBUG==1){
		    printf ("%i\n", count);
		}
		i = strlen(cwd);
		while (count > 0) {
			if (cwd[i-1] == '/') {			
				count--;
			}
			cwd[i-1] = '\0';
			i--;
			if(DEBUG==1){
	    			printf("%i:%i:%s\n", count, i, cwd);
			}
		}
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
			if (strlen(tmpstring) > 0 && chdir(tmpstring) == 0) {
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

