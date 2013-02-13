#include<stdlib.h>
#include<stdio.h>

int main(void){
	int nbytes= 20;
	char *test = (char*)malloc(nbytes+1);
	int bytes_read;


	bytes_read=getline(&test, &nbytes, stdin);

	while(*test != '\n'){
		printf("%c",*test);
		test++;
	} 
	
	return 0;
}
