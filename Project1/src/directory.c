#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include "bfsh.h"
#include "directory.h"

static int DEBUG = 0;

void update_CWD(char* cwd) {
	
	char* homeDirectory;
	getcwd(cwd, BUFFER);
	homeDirectory = getenv("HOME");

	if(DEBUG == 1){
		printf ("%s : %s \n", cwd, homeDirectory);
	}

	if(strncmp(cwd, homeDirectory, strlen(homeDirectory)) == 0) {
		if(strlen(cwd) == strlen(homeDirectory)) {
			cwd[0] = '~';
			cwd[1] = '\0';
		} else {		
			cwd[strlen(homeDirectory) - 1] = '~';
			if(DEBUG == 1){
				printf ("%s\n", cwd);
			}
			for(int i = 0; i < BUFFER - strlen(homeDirectory) - 2; i++) {
				cwd[i] = cwd[i + strlen(homeDirectory) - 1];
			}
			if(DEBUG == 1){
				printf ("%s\n", cwd);
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
	int count1 = 0;
	int count2 = 0;
	homeDirectory = getenv("HOME");
	getcwd(cwd, BUFFER);	

	if (strncmp(argument, "..", 2) == 0) {
		if(DEBUG == 1){
	    		printf("../.. chosen\n");
		}
		while (strncmp(argument+3*i, "../", 3) == 0) {
			count1++;
			i++;
		}
		if(DEBUG == 1){
		    printf ("%i ../ found ", count1);
		}
		if (strncmp(argument+3*i, "..", 3) == 0) {
			count1++;
			if(DEBUG == 1){
			    printf (": %i .. found ", count1);
			}
			tmpstring[0] = '\0';
		} else {
			while (strncmp(argument+3*i+count2, "/", 1) == 0) {
				count2++;
			}
			if(DEBUG == 1){
			    printf (": %i char until file ", count2);
			}
			if(strncmp (argument+3*i+count2-1, "/", 1) == 0) {
				strcpy (tmpstring, argument+3*i+count2);
			} else {
				count1 = 0;
				printf("Invalid Format\n");
			}
			if(DEBUG == 1){
			    printf (": [%s] found ", tmpstring);
			}
		}

		if(DEBUG == 1){
		    printf (": %i\n", count1);
		}
		i = strlen(cwd);
		while (count1 > 0 && i > 0) {
			if(cwd[i-1] == '/') {			
				count1--;
			}
			cwd[i-1] = '\0';
			i--;
			if(DEBUG == 1){
	    			printf("%i:%i:%s\n", count1, i, cwd);
			}
		}
		
		if(cwd[0] == '\0') {
			if(chdir("/") == 0) {
				if(strlen(tmpstring) > 0 && chdir(tmpstring) == -1) {
					printf("Failure: unable to find %s\n", tmpstring);
				}
			} else {
				printf("Failure\n");
			}
		} else {
			if(strlen(cwd) > 0 && chdir(cwd) == -1) {
				printf("Failure: unable to find %s\n", cwd);
			} else {
				if(strlen(tmpstring) > 0 && chdir(tmpstring) == -1) {
					printf("Failure: unable to find %s\n", tmpstring);
				}	
			}
		}
			
	} else if (strncmp(argument, "~", 1) == 0) {
		while (strncmp(argument+count1, "~", 1) == 0 || strncmp(argument+count1, "/", 1) == 0) {
			count1++;
		}
		strncpy (tmpstring, argument+count1, strlen(argument)-count1);
		if(chdir(homeDirectory) == 0) {
			if(strlen(tmpstring) > 0 && chdir(tmpstring) == -1) {
				printf("Failure: unable to find %s\n", tmpstring);
			}
		} else {
			printf("Failure: unable to find %s\n", homeDirectory);
		}
	} else {	
		if(strlen(argument) > 0 && chdir(argument) == -1) {
			printf("Failure: unable to find %s\n", argument);
		}
	}
	homeDirectory = NULL;
	return;
}
