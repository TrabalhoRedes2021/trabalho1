#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/wait.h>
#define MAX_BUFFER 200
#define CLIENT_PORT 8080
#define SERVER_PORT 8888

int main(void)
{
  char ip_server[12], ip_client[12];
  int port;
  int sockfd;
  char msg[MAX_BUFFER];
  int len, n;
  pid_t pid;
  struct sockaddr_in serv_addr, client_addr;

  // Abrindo conexão com o socket
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }
  // Zerando area de memorio dos endereços para evitar lixo
  memset(&serv_addr, 0, sizeof(serv_addr));
  memset(&client_addr, 0, sizeof(client_addr));
  // Setando informações do servidor
  printf("Digite o ip do servidor: \n");
  scanf("%s", ip_server);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(ip_server);
  serv_addr.sin_port = htons(SERVER_PORT);
 // Conectando com o endenreço do servidor 
  if(bind(sockfd, (const struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  pid = fork();
  if(pid == 0)
  {
    char * send;
    while(1){
      print("Send Message \n");
      scanf("%s", send);
      sendto(sockfd, (char *) send, strlen(send), MSG_CONFIRM, (const struct sockaddr *) &client_addr, len);
      printf("Message sent\n");
      if(!strcmp(send, "stop"))
      {
        break;
      }

    }

  }
  while(1){
    len = sizeof(client_addr); 
    n = recvfrom(sockfd, (char *)msg, MAX_BUFFER, MSG_WAITALL, (struct sockaddr *) &client_addr, &len);
    msg[n] = '\0';
    printf("Client: %s\n", msg);
    if(!strcmp(msg, "stop"))
    {
      break;
    }
  }

  return 0;
}
