#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "bfsh.h"
#include "directory.h"

void update_CWD(char* cwd) {
	
	char* homeDirectory;
	getcwd(cwd, BUFFER);
	homeDirectory = getenv("HOME");

	if (DEBUG==1){
		printf ("%s : %s \n", cwd, homeDirectory);
	}

	if (strncmp(cwd, homeDirectory, strlen(homeDirectory)) == 0) {
		if(strlen(cwd) == strlen(homeDirectory)) {
			cwd[0] = '~';
			cwd[1] = '\0';
		} else {		
			cwd[strlen(homeDirectory)-1] = '~';
			if (DEBUG==1){
				//printf ("%s\n", cwd);
			}
			for (int i=0;i<BUFFER - strlen(homeDirectory) -2;i++) {
				cwd[i] = cwd[i + strlen(homeDirectory)-1];
			}
			if (DEBUG==1){
				//printf ("%s\n", cwd);
			}
		}
	}

	return;
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
		    printf ("%i : %i :", (int)strlen(argument), count);
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
