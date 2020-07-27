#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void on_accept(int socket) {
  char* buff = (char*)malloc(sizeof(char) * 4096);
  while (1) {
    size_t bytes_read = read(socket, buff, 4096);
    if (bytes_read <= 0) {
      break;
    }
    buff[bytes_read / sizeof(char)] = '\0';
    printf("Received: %s", buff);
  }

  printf("Disconnected: %d\n", socket);
  free(buff);
}

int main() {
  struct sockaddr_in servaddr;

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    printf("socket creation failed...\n");
    exit(0);
  }
  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
  servaddr.sin_port = htons(3333);

  if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
    printf("socket bind failed...\n"); 
    return -1;
  }

  if ((listen(sockfd, 5)) != 0) { 
    printf("Listen failed...\n"); 
    return -1;
  }
  printf("Server listening..\n");

  while(1) {
    struct sockaddr_in client;
    int client_size = sizeof(client);
    int client_sock = accept(sockfd, (struct sockaddr*)&client, &client_size);
    if (client_sock < 0) {
      printf("Server acccept failed...\n");
      continue;
    }
    printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    if (!fork()) {
      on_accept(client_sock);
    }
  }

  close(sockfd);
}

