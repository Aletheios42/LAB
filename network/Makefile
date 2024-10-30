# Variables
CC = gcc
CFLAGS = -Wall -g
TARGETS = server client

# Compilación de ambos objetivos
all: $(TARGETS)

# Reglas de compilación
server: server.c
	$(CC) $(CFLAGS) server.c -o server

client: client.c
	$(CC) $(CFLAGS) client.c -o client

# Limpieza de archivos binarios
clean:
	rm -f $(TARGETS)
	rm -f *.o

# Ejecución del servidor y cliente
run_server: server
	./server

run_client: client
	./client

.PHONY: all clean run_server run_client

