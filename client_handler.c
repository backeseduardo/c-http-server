#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void *handle_client(void *arg) {
  int client_fd = *((int *)arg);
  char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
  ssize_t bytes_received;
  if ((bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0)) < 0) {
    perror("recv failed");
    exit(EXIT_FAILURE);
  }

  printf("Bytes received: %zd\n", bytes_received);
  printf("Message received\n");
  printf("%s\n", buffer);

  char *response = "HTTP/1.1 200 OK\r\n"
                   "Content-Type: text/html\r\n"
                   "\r\n"
                   "Hello from server";
  send(client_fd, response, strlen(response), 0);

  free(buffer);
  close(client_fd);

  return NULL;
}
