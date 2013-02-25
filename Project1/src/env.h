#ifndef ENV_H
#define ENV_H

  void setenv_Command(const char* input_name, const char* input_val, int argc);
  void unsetenv_Command(const char* input_name);
  void getenv_Command(const char* input_name);

#endif
