#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

// Estructura que se compartirá entre procesos
typedef struct {
  char message[100];
  int sender_id;
} shared_data_t;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Uso: %s [ID del proceso 1-3]\n", argv[0]);
    return 1;
  }

  int process_id = atoi(argv[1]);
  if (process_id < 1 || process_id > 3) {
    printf("El ID debe estar entre 1 y 3\n");
    return 1;
  }

  // Crear/abrir la memoria compartida
  int shm_fd = shm_open("/mi_memoria_compartida", O_CREAT | O_RDWR, 0666);
  if (shm_fd == -1) {
    perror("shm_open");
    return 1;
  }

  // Establecer el tamaño de la memoria compartida
  if (ftruncate(shm_fd, sizeof(shared_data_t)) == -1) {
    perror("ftruncate");
    return 1;
  }

  // Mapear la memoria compartida
  shared_data_t *shared_data =
      mmap(NULL, sizeof(shared_data_t), PROT_READ | PROT_WRITE, MAP_SHARED,
           shm_fd, 0);
  if (shared_data == MAP_FAILED) {
    perror("mmap");
    return 1;
  }

  // Crear/abrir semáforos para sincronización
  sem_t *write_sem = sem_open("/mi_semaforo_escritura", O_CREAT, 0666, 1);
  sem_t *read_sem1 = sem_open("/mi_semaforo_lectura1", O_CREAT, 0666, 0);
  sem_t *read_sem2 = sem_open("/mi_semaforo_lectura2", O_CREAT, 0666, 0);
  sem_t *read_sem3 = sem_open("/mi_semaforo_lectura3", O_CREAT, 0666, 0);

  if (write_sem == SEM_FAILED || read_sem1 == SEM_FAILED ||
      read_sem2 == SEM_FAILED || read_sem3 == SEM_FAILED) {
    perror("sem_open");
    return 1;
  }

  char input[100];
  printf("Terminal %d iniciada. Escribe mensajes o 'salir' para terminar.\n",
         process_id);

  while (1) {
    // Esperar por mensajes de otros procesos
    sem_t *my_sem;
    if (process_id == 1)
      my_sem = read_sem1;
    else if (process_id == 2)
      my_sem = read_sem2;
    else
      my_sem = read_sem3;

    // Verificar si hay un mensaje para este proceso
    int sem_value;
    sem_getvalue(my_sem, &sem_value);

    if (sem_value > 0) {
      sem_wait(my_sem);
      printf("Mensaje recibido de Terminal %d: %s\n", shared_data->sender_id,
             shared_data->message);
    }

    // Verificar si hay entrada del usuario
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100000; // 100ms

    if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0) {
      if (fgets(input, sizeof(input), stdin) != NULL) {
        // Eliminar el salto de línea
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "salir") == 0) {
          break;
        }

        // Adquirir semáforo de escritura
        sem_wait(write_sem);

        // Escribir mensaje en memoria compartida
        shared_data->sender_id = process_id;
        strncpy(shared_data->message, input, sizeof(shared_data->message) - 1);
        shared_data->message[sizeof(shared_data->message) - 1] = '\0';

        // Señalar a otros procesos que hay un mensaje
        for (int i = 1; i <= 3; i++) {
          if (i != process_id) {
            sem_t *target_sem;
            if (i == 1)
              target_sem = read_sem1;
            else if (i == 2)
              target_sem = read_sem2;
            else
              target_sem = read_sem3;

            sem_post(target_sem);
          }
        }

        // Liberar semáforo de escritura
        sem_post(write_sem);
      }
    }
  }

  // Limpiar recursos
  sem_close(write_sem);
  sem_close(read_sem1);
  sem_close(read_sem2);
  sem_close(read_sem3);

  // Solo el último proceso que sale elimina los recursos
  if (process_id == 1) {
    sem_unlink("/mi_semaforo_escritura");
    sem_unlink("/mi_semaforo_lectura1");
    sem_unlink("/mi_semaforo_lectura2");
    sem_unlink("/mi_semaforo_lectura3");
    shm_unlink("/mi_memoria_compartida");
  }

  return 0;
}
