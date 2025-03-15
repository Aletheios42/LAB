#include <stdio.h>
#include <string.h>

int main(void) {
  const char *str = "Hola, mundo!";
  // Se ejecuta el printf, y luego se evalúa 'str' para pasarla a strlen.
  size_t len = strlen((printf("soy el rey del mundo\n"), str));
  printf("La longitud de la cadena es: %zu\n", len);
  return 0;
}
