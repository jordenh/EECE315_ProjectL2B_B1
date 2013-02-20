#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include "bfsh.h"
#include "parser.h"

void parse_arguments(command *nextCommand, char *tempString){
	int i = 0, j = 0;
	int previous_char_is_alphanum = 0;
	int found_quotes = 0;
	char buffer[80];		
	
	while(1){
		if(*tempString == ' ' && previous_char_is_alphanum){
			if(!found_quotes){
				buffer[i] = '\0';
				strcpy(nextCommand->argv[j++], buffer);
				previous_char_is_alphanum = 0;
				nextCommand->argc++;
				i = 0;
			}
			else
				buffer[i++] = *tempString;	
		}
		else if(*tempString == '\"' || *tempString == '\''){
			found_quotes = ~(found_quotes);
		}
		else if(*tempString == '\0'){
			if(previous_char_is_alphanum){
				buffer[i] = '\0';
				strcpy(nextCommand->argv[j], buffer);
				nextCommand->argc++;
			}
			else if(j == 0){
				strcpy(nextCommand->argv[0],"\0");
			}

			strcpy(nextCommand->name, nextCommand->argv[0]);
			break;
		}
		else if(*tempString != ' '){
			buffer[i++] = *tempString;
			previous_char_is_alphanum = 1;
		}	

		tempString++;
	}
}
