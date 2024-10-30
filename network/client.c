#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
  int sock = 0;
  struct sockaddr_in serv_addr;
  char *message = "¡Hola, servidor!";
  char buffer[BUFFER_SIZE] = {0};

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Error al crear el socket\n");
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // Usar la dirección IP del servidor. En Docker, usamos el nombre del
  // servicio.
  if (inet_pton(AF_INET, "server", &serv_addr.sin_addr) <= 0) {
    printf("Dirección no válida\n");
    return -1;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("Error al conectar con el servidor\n");
    return -1;
  }

  send(sock, message, strlen(message), 0);
  printf("Mensaje enviado: %s\n", message);

  close(sock);
  return 0;
}

