#include <stdio.h>
#include <string.h>

// Definimos el número de colores y subtonos
#define COLOR_COUNT 5
#define TONE_COUNT 3

// Prototipo de la función que devuelve un arreglo de punteros a char
char **get_subtones(const char *color) {
  // Arreglos estáticos de punteros a char (nombres de subtonos)
  static char *violet_tones[TONE_COUNT] = {"Violeta", "Lila", "Magenta"};
  static char *blue_tones[TONE_COUNT] = {"Cyan", "Aguamarina", "Turquesa"};
  static char *red_tones[TONE_COUNT] = {"Carmesí", "Rosa", "Borgoña"};
  static char *green_tones[TONE_COUNT] = {"Verde Lima", "Verde Oliva",
                                          "Verde Bosque"};
  static char *yellow_tones[TONE_COUNT] = {"Amarillo Claro", "Mostaza",
                                           "Dorado"};

  // Retorna un puntero a los subtonos según el color
  if (color == NULL)
    return NULL;

  if (strcmp(color, "Rojo") == 0)
    return red_tones; // Retorna puntero al arreglo de subtonos
  if (strcmp(color, "Verde") == 0)
    return green_tones;
  if (strcmp(color, "Azul") == 0)
    return blue_tones;
  if (strcmp(color, "Amarillo") == 0)
    return yellow_tones;
  if (strcmp(color, "Violeta") == 0)
    return violet_tones;

  return NULL; // Si no se encuentra el color
}

// Función que devuelve un arreglo de punteros a char (nombres de colores)
char **get_color_names() {
  // Arreglo estático de punteros a char (nombres de colores)
  static char *colors[COLOR_COUNT] = {"Rojo", "Verde", "Azul", "Amarillo",
                                      "Violeta"};
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

  // Elegir un color
  int index;
  printf("Ingresa el índice del color (1 a %d): ", COLOR_COUNT);
  scanf("%d", &index);

  if (index >= 1 && index <= COLOR_COUNT) {
    char *selected_color = colors[index - 1];
    printf("Has seleccionado: %s\n", selected_color);

    // Obtener los subtonos del color seleccionado
    char **subtones = get_subtones(selected_color);

    // Imprimir los subtonos
    if (subtones != NULL) {
      printf("Subtonos de %s:\n", selected_color);
      for (int i = 0; i < TONE_COUNT; i++) {
        printf("- %s\n",
               subtones[i]); // Acceso a los subtonos a través del puntero
      }
    } else {
      printf("No hay subtonos disponibles para el color seleccionado.\n");
    }
  } else {
    printf("Índice fuera de rango.\n");
  }

  return 0;
}
