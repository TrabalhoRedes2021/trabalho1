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
  char * ip_server, * ip_client;
  int client_opt;
  int sockfd;
  char msg[MAX_BUFFER];
  int len, n, status;
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
  client_opt = atoi(getenv("CLIENT_OPT"));
  ip_server = getenv("IP_SERVER");
  ip_client = getenv("IP_CLIENT");
  // Setando informações do servidor
  // printf("Digite o ip do client: \n");
  // scanf("%s", ip_client);

  // Servidor
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(ip_server);
  serv_addr.sin_port = htons(SERVER_PORT);
  //Client
  if(client_opt==2)
  {
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr(ip_client);
    client_addr.sin_port = htons(CLIENT_PORT);
  }
 // Conectando com o endenreço do servidor 
 if(client_opt == 1)
 {
    if(bind(sockfd, (const struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
      perror("bind failed");
      exit(EXIT_FAILURE);
    }
 }
 else{
  if(bind(sockfd, (const struct sockaddr *)&client_addr, sizeof(client_addr)) < 0)
    {
      perror("bind failed");
      exit(EXIT_FAILURE);
    }
 }
 if(client_opt == 2)
 {
  pid = fork();
  if(pid == 0)
  {
    char * send;
    while(1){
      printf("Send Message \n");
      send = getenv("MESSAGE");
      sendto(sockfd, send, strlen(send), 0, (const struct sockaddr *) &serv_addr, sizeof(serv_addr));
      printf("Message sent\n");
      sleep(3);
      if(!strcmp(send, "stop"))
      {
        break;
      }

    }
    exit(0);

  }
 }
  if(client_opt == 1){
    printf("Client server ready to receive!\nIP: %s\tPORT: %d", ip_server, SERVER_PORT);
    while(1){
      memset(msg, 0x0, MAX_BUFFER);
      len = sizeof(client_addr); 
      n = recvfrom(sockfd, msg, MAX_BUFFER, 0, (struct sockaddr *) &client_addr, (unsigned int *)&len);
      printf("Client SENT: %s\n", msg);
      if(!strcmp(msg, "stop"))
      {
        break;
      }
    }
  }
  else
  {
    waitpid(pid, &status, 0);
  }

  return 0;
}
