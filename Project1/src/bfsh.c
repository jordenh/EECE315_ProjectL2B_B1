#include<stdlib.h>
#include<stdio.h>

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
	command nextCommand ;	
	int commandCount = 0;
	int exitBool = FALSE;
	int i,j;
	int charParserIndex = 0;
	int individualCharIndex = 0;
	char tempChar;
	int nbytes= BUFFER*sizeof(char);
	int bytes_read;

	nextCommand.name = (char*)malloc(nbytes+1);
	for(i=0;i<100;i++){
		nextCommand.argv[i] = (char*)malloc(nbytes+1);
	}    


	printf("The value of argc is: %d\n" ,nextCommand.argc);
	printf("The value of name is: %s\n" ,nextCommand.name);

	nextCommand.argc = 0;
	while(!exitBool){
		//clear argv/argc/name for the next input.			
        	for(i=0;i<=nextCommand.argc;i++){
	            for(j=0;j<BUFFER;j++){
        	        nextCommand.argv[i][j] = '\0'; //replace all of allocated buffer size to null
        	    }
        	}
		for(i=0;i<BUFFER;i++){
			nextCommand.name[i] = '\0';
		}	
        	 
        	nextCommand.argc = 0;
		charParserIndex = 0;
		individualCharIndex = 0;		

		printf("cmd%d:~myDirectoryValue> ", commandCount);
		bytes_read = getline(&nextCommand.argv[0], &nbytes, stdin);
		
		tempChar = nextCommand.argv[0][charParserIndex];
		while(tempChar != '\n' ){
			if(nextCommand.argc == 0){
				if(tempChar != 0x20){
					nextCommand.name[individualCharIndex++] = nextCommand.argv[0][charParserIndex];
				}
				else{
					nextCommand.argc++;  //move to next string
					individualCharIndex = 0;
					printf("ping1%s,%d,%d\n", nextCommand.name, nextCommand.argc,charParserIndex);
				}
			}
			else{
				if(tempChar != 0x20){
					nextCommand.argv[nextCommand.argc][individualCharIndex++] = nextCommand.argv[0][charParserIndex];
				}
				else{
					nextCommand.argc++;  //move to next string
					individualCharIndex = 0;

					printf("ping2%s,%d\n", nextCommand.argv[nextCommand.argc], nextCommand.argc);
				}
			}
			charParserIndex++;
			tempChar = nextCommand.argv[0][charParserIndex];
		}

		strcpy(nextCommand.argv[0],nextCommand.name);
		
		for(i=0; i<=nextCommand.argc; i++){
			printf("-%s-\n",nextCommand.argv[i]);
		}

		if(1){
			commandCount++;
		}

        //PERFORM ACTION HERE

        
	}


	return 0;
}
