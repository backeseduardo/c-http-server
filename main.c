#include "client_handler.c"
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8080

int main() {
  int server_fd;
  struct sockaddr_in address;
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 10) < 0) {
    perror("listen failed");
    exit(EXIT_FAILURE);
  }

  printf("Server listen on port %d\n", PORT);

  while (1) {
    int *client_fd = malloc(sizeof(int));
    socklen_t client_add_len = sizeof(address);
    if ((*client_fd = accept(server_fd, (struct sockaddr *)&address,
                             &client_add_len)) < 0) {
      perror("accept failed");
      exit(EXIT_FAILURE);
    }

    pthread_t thread;
    pthread_create(&thread, NULL, handle_client, (void *)client_fd);
    pthread_detach(thread);
  }

  close(server_fd);
  return 0;
}
