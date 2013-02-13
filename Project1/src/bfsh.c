#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define BUFFER 300
#define TRUE 1
#define FALSE 0

struct command_t {
    int argc;
    char* name;
    char* argv[99]; //name is repeated in argv[0]
};

typedef struct command_t command;

int main(void){
	int exitBool = FALSE;
	//char* pwd;
	command nextCommand ;	
	int commandCount = 0;
	int individualCharIndex = 0;
	int nbytes= BUFFER;
	char* tempChar=malloc(nbytes+1);
	nextCommand.argc = 0;


	nextCommand.name = (char*)malloc(nbytes+1);
	for(int i=0;i<100;i++){
		nextCommand.argv[i] = (char*)malloc(nbytes+1);
	}    


	while(!exitBool){
		//clear argv/argc/name for the next input.			
        	for(int i=0;i<=nextCommand.argc;i++){
	            for(int j=0;j<BUFFER;j++){
        	        nextCommand.argv[i][j] = '\0'; //replace all of allocated buffer size to null
        	    }
        	}
		for(int i=0;i<BUFFER;i++){
			nextCommand.name[i] = '\0';
		}	
        	 
        	nextCommand.argc = 0;
		individualCharIndex = 0;		

		printf("cmd%d:~myDirectoryValue> ", commandCount);
		fgets(tempChar, (nbytes+1), stdin);
		
		while(*tempChar != '\n' ){
			if(nextCommand.argc == 0){
				if(*tempChar != 0x20){
					nextCommand.name[individualCharIndex++] = *tempChar;
				}
				else{
					nextCommand.argc++;  //move to next string
					individualCharIndex = 0;
					//printf("ping1%s,%d,%d\n", nextCommand.name, nextCommand.argc,charParserIndex);
				}
			}
			else{
				if(*tempChar != 0x20){
					nextCommand.argv[nextCommand.argc][individualCharIndex++] = *tempChar;
				}
				else{
					nextCommand.argc++;  //move to next string
					individualCharIndex = 0;

					//printf("ping2%s,%d\n", nextCommand.argv[nextCommand.argc], nextCommand.argc);
				}
			}
			tempChar++;
		}

		strcpy(nextCommand.argv[0],nextCommand.name);
		
		for(int i=0; i<=nextCommand.argc; i++){
			printf("-%s-\n",nextCommand.argv[i]);
		}

		if(1){
			commandCount++;
		}

        
	}


	return 0;
}
