#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include "bfsh.h"
#include "env.h"

static int DEBUG = 0;

void setenv_Command(const char* input_name, const char* input_value){
  if(setenv(input_name, input_value, 1)==-1)
  {
    printf( "%s = %s cannot be set\n", input_name, input_value);
  }
  if (DEBUG==1){//if debug on, print out value in variable
  	printf( "%s = %s\n", input_name, getenv (input_name));
  }
}

void unsetenv_Command(const char* input_name){
  if(unsetenv(input_name)==-1){
  	printf( "%s cannot be unset\n", input_name);
  }
  if (DEBUG==1){//if debug on, confirm unset
  	printf( "%s has been unset\n", input_name);
  }
}

void getenv_Command(const char* input_name){
  char* value;
  value = getenv (input_name);
  if(!value){
  	printf( "%s is not set\n", input_name);
  } else {
  	printf ( "%s = %s\n", input_name, value);
  }
}
