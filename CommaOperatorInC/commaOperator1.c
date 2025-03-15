#include <stdio.h>

int main(void) {
  int x = 1;
  // La expresión entre paréntesis evalúa primero (x += 2) y luego (x * 3).
  // El valor global es el resultado de (x * 3).
  int y = (x += 2, x * 3);
  printf("x = %d, y = %d\n", x, y);
  return 0;
}
