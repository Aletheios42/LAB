#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define NUM_THREADS 4
#define MAX_EVENTS 100
#define MAX_MESSAGE_LEN 100

// Códigos de color ANSI
#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_RESET "\x1b[0m"

// Nombres de las personas
const char *nombres[] = {"Ana", "Carlos", "Elena", "Pablo"};

// Actividades cotidianas que realizan las personas
const char *actividades[][5] = {
    {"desayunando ☕", "leyendo 📚", "sacando al perro 🐕",
     "jugando al fútbol ⚽", "cocinando 🍳"},
    {"duchándose 🚿", "trabajando 💻", "viendo Netflix 📺", "comprando 🛒",
     "haciendo ejercicio 🏋️"},
    {"pintando 🎨", "bailando 💃", "escribiendo ✍️", "hablando por teléfono 📱",
     "durmiendo 😴"},
    {"tocando guitarra 🎸", "corriendo 🏃", "estudiando 📝", "paseando 🚶",
     "escuchando música 🎧"}};

// Estructura para almacenar un evento
typedef struct {
  int person_id;
  char message[MAX_MESSAGE_LEN];
  struct timeval timestamp;
} event_t;

// Buffer de eventos compartido
event_t event_log[MAX_EVENTS];
int event_count = 0;
sem_t *log_semaphore; // Semáforo para proteger el log

// Función para registrar un evento en el log
void log_event(int person_id, const char *message) {
  // Proteger el acceso al log compartido con semáforo
  sem_wait(log_semaphore);

  if (event_count < MAX_EVENTS) {
    // Guardar ID de la persona
    event_log[event_count].person_id = person_id;

    // Copiar el mensaje
    strncpy(event_log[event_count].message, message, MAX_MESSAGE_LEN - 1);
    event_log[event_count].message[MAX_MESSAGE_LEN - 1] = '\0';

    // Obtener y guardar el timestamp actual
    gettimeofday(&event_log[event_count].timestamp, NULL);

    // Incrementar el contador de eventos
    event_count++;
  }

  sem_post(log_semaphore);
}

// Obtener el color correspondiente a una persona
const char *get_person_color(int person_id) {
  switch (person_id) {
  case 0:
    return COLOR_RED;
  case 1:
    return COLOR_GREEN;
  case 2:
    return COLOR_YELLOW;
  case 3:
    return COLOR_BLUE;
  default:
    return COLOR_MAGENTA;
  }
}

// Función que ejecutará cada hilo
void *person_activities(void *arg) {
  int id = *((int *)arg);
  char msg[MAX_MESSAGE_LEN];

  // Cada persona realiza varias actividades
  for (int i = 0; i < 5; i++) {
    // Simular el tiempo que toma cada actividad
    usleep((rand() % 1000 + 500) * 1000); // Entre 0.5 y 1.5 segundos

    // Crear mensaje y registrar evento
    sprintf(msg, "%s está %s", nombres[id], actividades[id][i]);
    log_event(id, msg);
  }

  return NULL;
}

// Función para imprimir todos los eventos registrados
void print_event_log() {
  printf("\n🕒 === DIARIO DE ACTIVIDADES === 🕒\n\n");

  for (int i = 0; i < event_count; i++) {
    int person_id = event_log[i].person_id;

    // Formatear timestamp en horas:minutos:segundos.milisegundos
    struct tm *tm_info;
    tm_info = localtime(&event_log[i].timestamp.tv_sec);

    printf("[%02d:%02d:%02d.%03ld] %s%s%s\n", tm_info->tm_hour, tm_info->tm_min,
           tm_info->tm_sec, event_log[i].timestamp.tv_usec / 1000,
           get_person_color(person_id), event_log[i].message, COLOR_RESET);
  }

  printf("\n✨ Todas las actividades han sido registradas ✨\n");
}

int main() {
  pthread_t threads[NUM_THREADS];
  int person_ids[NUM_THREADS];

  // Crear un semáforo con valor inicial 1 (equivalente a un mutex)
  log_semaphore = sem_open("/event_log_sem", O_CREAT, 0644, 1);
  if (log_semaphore == SEM_FAILED) {
    perror("sem_open");
    return 1;
  }

  // Inicializar generador de números aleatorios
  srand(time(NULL));

  printf("🚀 Comenzando simulación de actividades diarias 🚀\n");
  printf("Cada persona realiza sus actividades en paralelo\n");

  // Crear hilos (cada uno representa una persona)
  for (int i = 0; i < NUM_THREADS; i++) {
    person_ids[i] = i;
    pthread_create(&threads[i], NULL, person_activities, &person_ids[i]);
    printf("💫 %s%s%s ha comenzado su día\n", get_person_color(i), nombres[i],
           COLOR_RESET);
  }

  // Esperar a que todos los hilos terminen
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
    printf("✅ %s%s%s ha terminado todas sus actividades\n",
           get_person_color(i), nombres[i], COLOR_RESET);
  }

  // Mostrar todos los eventos registrados
  print_event_log();

  // Cerrar y eliminar el semáforo
  sem_close(log_semaphore);
  sem_unlink("/event_log_sem");

  return 0;
}
