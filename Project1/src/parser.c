#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "bfsh.h"
#include "parser.h"

void parse_arguments(command *nextCommand, char *tempString){
	int i = 0, j = 0;
	int previous_char_is_alphanum = 0;
	int found_dollar_char = 0;
	int found_quotes = 0;
	char buffer[80];		
	char *shifted_buffer;
	
	while(1){
		if(*tempString == ' ' && previous_char_is_alphanum){
			if(!found_quotes){
				buffer[i] = '\0';
				if(found_dollar_char){
					found_dollar_char = 0;
					shifted_buffer = malloc(strlen(buffer));

					for(int k = 0; k<strlen(buffer); k++)
						*(shifted_buffer + k) = buffer[k+1];
				
					shifted_buffer = getenv(shifted_buffer);
					strcpy(nextCommand->argv[j++], shifted_buffer);
					free(shifted_buffer);
					previous_char_is_alphanum = 0;
					nextCommand->argc++;
					i = 0;
				}
				else{
					strcpy(nextCommand->argv[j++], buffer);
					previous_char_is_alphanum = 0;
					nextCommand->argc++;
					i = 0;
				}
			}
			else
				buffer[i++] = *tempString;	
		}
		else if(*tempString == '\"' || *tempString == '\''){
			found_quotes = ~(found_quotes);
		}
		else if(*tempString == '\0'){
			if(found_quotes){
				printf("unmatched \"\n");
				strcpy(nextCommand->argv[0], "\0");
			}
			else if(previous_char_is_alphanum){
				buffer[i] = '\0';
				if(found_dollar_char){
					found_dollar_char = 0;
					shifted_buffer = malloc(strlen(buffer));

					for(int k = 0; k<strlen(buffer); k++)
						*(shifted_buffer + k) = buffer[k+1];
				
					shifted_buffer = getenv(shifted_buffer);
					strcpy(nextCommand->argv[j], shifted_buffer);
				}
				else
					strcpy(nextCommand->argv[j], buffer);
			}
			else if(j == 0){
				strcpy(nextCommand->argv[0],"\0");
			}

			strcpy(nextCommand->name, nextCommand->argv[0]);
			break;
		}

		else if(*tempString != ' '){
			if(*tempString == '$')
				found_dollar_char = 1;
					
			buffer[i++] = *tempString;
			previous_char_is_alphanum = 1;
		}	

		tempString++;
	}
}
