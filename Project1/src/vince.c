#include<stdlib.h>
#include<stdio.h>
#include<string.h>

getenv_func (const char* input_name)
{
  char * value;
	value - getenv (input_name);
	if (!value)
	{
		printf( "'%s' is not set\n", input_name);
	}
	else
	{
		printf( "%s = %s\n", var_name, val);
	}
}

setenv_func （const char* input_name, const char* input_val)
{
	if( setenv(input_name, input_val, 1)== -1) //returns -1 on errors, 0 otherwise
	{
		printf( "%s = %s cannot be set\n", input_name, input_val);//error
	}
}

unsetenv_func (const char * input_name)
{
	if( setenv(name, input_val, 1)== -1) //returns -1 on errors, 0 otherwise
	{
		printf( "%s = %s cannot be unset\n"", input_name, input_val);//error
	}
}

exportenv_func (const char* input_name, const char* export_val)
{
	char * value;
	value - getenv (input_name);
	if (!value)
	{
		getenv(input_name) = input_name;
	}
	else
	{
		//append location to env variable
		printf( "%s = %s\n", var_name, val);
	}

int main (void)
{
	if (nextCommand.name == "set")
  {
		setenv_func ();
  }
	if (nextCommand.name == "unset")
  {
		unsetenv_func ();
  }
	if (nextCommand.name == "echo")
  {
		getenv_func ();
  }
	if (nextCommand.name == "export")
  {
		exportenv_func ();
  }
	return 0;
}
