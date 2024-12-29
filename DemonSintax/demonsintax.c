#include <stdio.h>

// Simple functions that return void
void sayHello() { printf("Hello!\n"); }

void sayBye() { printf("Bye!\n"); }

void sayWhat() { printf("What?\n"); }

// Functions that return pointers to functions
void (*getHello())() { return sayHello; }

void (*getBye())() { return sayBye; }

void (*getWhat())() { return sayWhat; }

// Our array of pointers to functions that return pointers to functions
void (*(*f[3])())() = {getHello, getBye, getWhat};

int main() {
  for (int i = 0; i < 3; i++) {
    void (*func)() = f[i](); // Get the function from our array
    func();
  }

  // We can also access them directly
  printf("\nNow directly:\n");
  (f[0]())(); // Will print "Hello!"
  (f[1]())(); // Will print "Bye!"
  (f[2]())(); // Will print "What?"

  return 0;
}
