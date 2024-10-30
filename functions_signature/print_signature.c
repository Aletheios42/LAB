#include <dlfcn.h>
#include <stdio.h>

// Definimos dos funciones de ejemplo
int task_a(char **args, void *context, int flag) {
  printf("Executing task A\n");
  return 0;
}

int task_b(char **args, void *context, int flag) {
  printf("Executing task B\n");
  return 0;
}

// Función para imprimir el nombre de la función y ejecutarla
int print_signature(void *ft) {
  Dl_info info;

  // Usamos dladdr para obtener información sobre la función
  if (dladdr(ft, &info)) {
    printf("Function name: %s\n", info.dli_sname); // Nombre de la función
  } else {
    printf("Function name: unknown\n");
  }

  // Casteamos el puntero genérico a un puntero a función
  int (*action)(char **, void *, int) = ft;

  // Ejecutar la función si no es NULL
  if (action != NULL) {
    char *dummy_args[] = {NULL};        // Argumentos ficticios
    return action(dummy_args, NULL, 0); // Llamar a la función
  }

  return -1; // Si la función no es válida, devolvemos un código de error
}

int main() {
  // Punteros a las funciones
  void *ft_a = task_a;
  void *ft_b = task_b;

  // Imprimimos los nombres de las funciones y ejecutamos las acciones
  print_signature(ft_a);
  print_signature(ft_b);

  return 0;
}

