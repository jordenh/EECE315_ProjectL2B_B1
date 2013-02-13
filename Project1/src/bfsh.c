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
	int i;
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


	while(!exitBool){

		printf("cmd%d:~myDirectoryValue> ", commandCount);
		bytes_read = getline(&nextCommand.argv[0], &nbytes, stdin);

	        
		nextCommand.argc = 0;
		charParserIndex = 0;
		individualCharIndex = 0;
		tempChar = nextCommand.argv[0][charParserIndex];
		while(tempChar != '\n' ){
			if(nextCommand.argc == 0){
				if(tempChar != 0x20){
					nextCommand.name[individualCharIndex++] = nextCommand.argv[0][charParserIndex];
				}
				else{
					nextCommand.argc++;  //move to next string
					individualCharIndex = 0;
					printf("ping1%s\n", nextCommand.name);
				}
			}
			else{
				if(tempChar != 0x20){
					nextCommand.argv[nextCommand.argc][individualCharIndex++] = nextCommand.argv[0][charParserIndex];
				}
				else{
					nextCommand.argc++;  //move to next string
					individualCharIndex = 0;

					printf("ping2%s\n", nextCommand.argv[nextCommand.argc]);
				}
			}
			charParserIndex++;
			tempChar = nextCommand.argv[0][charParserIndex];
		}

		nextCommand.argv[0] = nextCommand.name;
		printf("You typed: %s\n",nextCommand.name);
		
		for(i=0; i<=nextCommand.argc; i++){
			printf("-%s-\n",nextCommand.argv[i]);
		}

		if(1){
			commandCount++;
		}
	}


	return 0;
}
