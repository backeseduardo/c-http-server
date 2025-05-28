#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

const int VERB_GET = 1;
const int VERB_POST = 2;
const int VERB_ERROR_UNKNOWN = -1;
const int VERB_ERROR_INVALID_REQUEST = -2;
const int VERB_ERROR_TOO_LONG = -3;

int extract_verb(const char *request) {
  const char *end = strchr(request, ' ');
  if (!end) {
    perror("invalid request");
    return VERB_ERROR_INVALID_REQUEST;
  }

  int len = end - request;
  char verb[10];
  if (len > sizeof(verb)) {
    return VERB_ERROR_TOO_LONG;
  }

  strncpy(verb, request, len);
  verb[len] = '\0';

  if (strcmp(verb, "GET") == 0) {
    return VERB_GET;
  }
  if (strcmp(verb, "POST") == 0) {
    return VERB_POST;
  }

  return VERB_ERROR_UNKNOWN;
}

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

  int verb;
  if ((verb = extract_verb(buffer)) < 0) {
    perror("extract verb failed");
    exit(EXIT_FAILURE);
  }
  free(buffer);

  char response[BUFFER_SIZE];
  if (verb == VERB_GET) {
    printf("It is a GET\n");
    sprintf(response, "HTTP/1.1 404 Not Found\r\n"
                      "Content-Type: text/plain\r\n"
                      "\r\n"
                      "404 Not Found");
    send(client_fd, response, strlen(response), 0);
    close(client_fd);
    return NULL;
  } else if (verb == VERB_POST) {
    printf("It is a POST\n");
  }

  time_t seconds = time(NULL);
  sprintf(response,
          "HTTP/1.1 200 OK\r\n"
          "Content-Type: text/html\r\n"
          "\r\n"
          "<!DOCTYPE html>"
          "<html lang=\"en\">"
          "<head><title>test</title></head>"
          "<body><h1>%s</h1><h2>%ld</h2></body>"
          "</html>",
          "Hello", seconds);
  send(client_fd, response, strlen(response), 0);
  close(client_fd);
  return NULL;
}
