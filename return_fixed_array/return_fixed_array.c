#include <stdio.h>

// Definimos un tamaño fijo para los colores
#define COLOR_COUNT 5

// Función que devuelve un arreglo de punteros a char (nombres de colores)
char **get_color_names() {
  // Arreglo estático de punteros a char (nombres de colores)
  static char *colors[COLOR_COUNT] = {"Rojo", "Verde", "Azul", "Amarillo",
                                      "Naranja"};

  return colors; // Retorna el puntero al arreglo de colores
}

int main() {
  // Obtener el arreglo de colores
  char **colors = get_color_names();

  // Imprimir todos los colores
  printf("Colores disponibles:\n");
  for (int i = 0; i < COLOR_COUNT; i++) {
    printf("%d: %s\n", i + 1, colors[i]);
  }

  // Si se desea, también se puede acceder a un color específico
  int index;
  printf("Ingresa el índice del color (1 a %d): ", COLOR_COUNT);
  scanf("%d", &index);

  if (index >= 1 && index <= COLOR_COUNT) {
    printf("El color seleccionado es: %s\n", colors[index - 1]);
  } else {
    printf("Índice fuera de rango.\n");
  }

  return 0;
}

