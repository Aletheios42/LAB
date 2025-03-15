#include <pthread.h>
#include <stdio.h>

// Variable global para la suma
int suma = 0;
pthread_mutex_t mutex;

// Función que ejecutará cada hilo
void *sumar(void *arg) {
  int num = *((int *)arg);

  /*pthread_mutex_lock(&mutex);*/
  suma += num;
  /*pthread_mutex_unlock(&mutex);*/

  pthread_exit(NULL);
}

int main() {
  pthread_t hilos[10000];
  int numeros[10000];

  // Inicializar mutex
  pthread_mutex_init(&mutex, NULL);

  // Crear 10000 hilos
  for (int i = 0; i < 10000; i++) {
    numeros[i] = 1; // Cada hilo sumará 1
    pthread_create(&hilos[i], NULL, sumar, &numeros[i]);
  }

  // Esperar a que todos los hilos terminen
  for (int i = 0; i < 10000; i++) {
    pthread_join(hilos[i], NULL);
  }

  // Destruir mutex
  pthread_mutex_destroy(&mutex);

  // Mostrar resultado
  printf("Suma total: %d\n", suma);

  return 0;
}
