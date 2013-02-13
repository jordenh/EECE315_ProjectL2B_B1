#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>

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

    nextCommand.argc = 123;
    nextCommand.name = (char*)malloc(BUFFER*sizeof(char));
    for(i=0;i<100;i++){
        nextCommand.argv[i] = (char*)malloc(BUFFER*sizeof(char));
    }    


    printf("The value of argc is: %d\n" ,nextCommand.argc);
    printf("The value of name is: %s\n" ,nextCommand.name);


    while(!exitBool){

        printf("cmd%d:~myDirectoryValue> ", commandCount);
        scanf("%s", nextCommand.argv[0]); //initially scan whole in whole string

        printf("You typed: %s\n",nextCommand.name);
        
        nextCommand.argc = 0;
        charParserIndex = 0;
		individualCharIndex = 0;
		tempChar = nextCommand.argv[0][charParserIndex];
        while(tempChar != '$' ){
            if(nextCommand.argc = 0){
				if(tempChar != ' '){
					nextCommand.name[individualCharIndex++] = nextCommand.argv[0][charParserIndex];
				}
				else{
					nextCommand.argc++;  //move to next string
					individualCharIndex = 0;
				}
			}
            else{
                if(tempChar != ' '){
					nextCommand.argv[nextCommand.argc][individualCharIndex++] = nextCommand.argv[0][charParserIndex];
				}
				else{
					nextCommand.argc++;  //move to next string
					individualCharIndex = 0;
				}
            }
            charParserIndex++;
			tempChar = nextCommand.argv[0][charParserIndex];
        }
        
		nextCommand.argv[0] = nextCommand.name;

        if(1){
            commandCount++;
        }
    }
    

	printf(	"Hello, World! \n");

	return 0;


}
