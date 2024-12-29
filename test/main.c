#include "function.h"
#include <stdio.h>

int global_var = 42;

int main() {
  int local_var = 10;
  print_message(global_var, local_var);
  return 0;
}
