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
	
	// is the current working directory a sub folder of the home directory
	if(strncmp(cwd, homeDirectory, strlen(homeDirectory)) == 0) {
		// if the cwd is the home directory then print out ~/
		if(strlen(cwd) == strlen(homeDirectory)) {
			cwd[0] = '~';
			cwd[1] = '\0';
		// else print out something along the lines of ~/[directory path]
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
	// if the cwd is not a sub folder of the home directory then nothing needs to be done

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

	// does the argument fall along the lines of "../../"
	if (strncmp(argument, "..", 2) == 0) {
		if(DEBUG == 1){
	    		printf("../.. chosen\n");
		}
		// count the number of consecutive "../" show up
		while (strncmp(argument+3*i, "../", 3) == 0) {
			count1++;
			i++;
		}
		if(DEBUG == 1){
		    printf ("%i ../ found ", count1);
		}
		// does the argument get terminated with ..?
		if (strncmp(argument+3*i, "..", 3) == 0) {
			count1++;
			if(DEBUG == 1){
			    printf (": %i .. found ", count1);
			}
			tmpstring[0] = '\0';
		} else { // if it doesn't then the argument is along the lines of ../../[directory path]
			// user is allowed to place extra /// before [directory path]. count how many they placed
			while (strncmp(argument+3*i+count2, "/", 1) == 0) {
				count2++;
			}
			if(DEBUG == 1){
			    printf (": %i char until file ", count2);
			}
			// make sure the user is following a valid format and hasn't put ... or ../...
			if(strncmp (argument+3*i+count2-1, "/", 1) == 0) {
				// seperate and store the [direcotry path] portion of the argument
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
		// starting at the back replace charatcters with nulls untill the user has been navigated up
		// the desired number of directories
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
		
		// change to the desired directory
		// two step process: first the desired number of directories up then to the [directory path] location
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
		
	} else if (strncmp(argument, "~", 1) == 0) { // ~/[directory path] type argument
		// user is allowed to have as many /// as they want between ~ and [directory path]
		count1 = 1;
		while (strncmp(argument+count1, "/", 1) == 0) {
			count1++;
		}
		// seperate and store [directory path]
		strncpy (tmpstring, argument+count1, strlen(argument)-count1);
		// change to desired directory
		// two steps: first to home directory then to [directory path] location
		if(chdir(homeDirectory) == 0) {
			if(strlen(tmpstring) > 0 && chdir(tmpstring) == -1) {
				printf("Failure: unable to find %s\n", tmpstring);
			}
		} else {
			printf("Failure: unable to find %s\n", homeDirectory);
		}
	} else {	
		// relative and absolute paths can be sent directly to chdir
		if(strlen(argument) > 0 && chdir(argument) == -1) {
			printf("Failure: unable to find %s\n", argument);
		}
	}
	homeDirectory = NULL;
	return;
}
