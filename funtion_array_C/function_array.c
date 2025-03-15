#include <stdio.h>
#include <stdlib.h>

void opcion1() { printf("Ejecutando la función 1...\n"); }
void opcion2() { printf("Ejecutando la función 2...\n"); }
void opcion3() { printf("Ejecutando la función 3...\n"); }

int main(void) {
  char entrada[10];
  int num;

  // Array de punteros a funciones
  void (*funciones[])() = {opcion1, opcion2, opcion3};

  printf("Ingresa un número del 1 al 3: ");
  if (fgets(entrada, sizeof(entrada), stdin) != NULL) {
    printf("%s\n", entrada);
    num = atoi(entrada);

    if (num >= 1 && num <= 3) {
      // Se llama a la función correspondiente (se usa num - 1 para el índice)
      funciones[num - 1]();
    } else {
      printf("Opción no válida.\n");
    }
  } else {
    printf("Error al leer la entrada.\n");
  }
  return 0;
}
