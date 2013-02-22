#ifndef ENV_h
#define ENV_h

  void setenv_Command(const char* input_name, const char* input_val);
	void unsetenv_Command(const char* input_name);
	void getenv_Command(const char* input_name);

#endif
