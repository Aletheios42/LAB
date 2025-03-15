#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

// Variable global para la suma
int suma = 0;
sem_t *semaforo; // Semáforo con nombre

// Función que ejecutará cada hilo
void *sumar(void *arg) {
  int num = *((int *)arg);

  // Bloquear el semáforo antes de acceder a la variable compartida
  sem_wait(semaforo);
  suma += num;
  sem_post(semaforo); // Liberar el semáforo

  pthread_exit(NULL);
}

int main() {
  pthread_t hilos[10000];
  int numeros[10000];

  // Crear un semáforo con valor inicial 1 (equivalente a un mutex)
  semaforo = sem_open("/mi_semaforo", O_CREAT, 0644, 1);
  if (semaforo == SEM_FAILED) {
    perror("sem_open");
    return 1;
  }

  // Crear 10000 hilos
  for (int i = 0; i < 10000; i++) {
    numeros[i] = 1; // Cada hilo sumará 1
    pthread_create(&hilos[i], NULL, sumar, &numeros[i]);
  }

  // Esperar a que todos los hilos terminen
  for (int i = 0; i < 10000; i++) {
    pthread_join(hilos[i], NULL);
  }

  // Mostrar resultado
  printf("Suma total: %d\n", suma);

  // Cerrar y eliminar el semáforo
  sem_close(semaforo);
  sem_unlink("/mi_semaforo");

  return 0;
}
