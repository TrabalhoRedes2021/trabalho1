#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include "network_util.h"

void send_message(int sd, struct sockaddr_in endClient, char * message)
{
  printf("Enviando message\n");
  sendto(sd, message, strlen(message), 0, (const struct sockaddr *)&endClient, sizeof(endClient));
}

int receive_message(int sd, struct sockaddr_in endClient, int bufferLen)
{
  char * buffer;
  int lenMessage;
  int len = sizeof(endClient);
  int stop = 0;
  buffer = (char *)malloc(bufferLen * sizeof(char));
  printf("Aguardando messagem do servidor\n");
  memset(buffer, 0x0, bufferLen);
  lenMessage = recvfrom(sd, buffer, bufferLen, 0, (struct sockaddr *)&endClient, (unsigned int *)&len);
  printf("Client send: %s\tLEN: %d\n", buffer, lenMessage);
  if(!strcmp(buffer, "stop"))
  {
    stop = 1;
  }
  free(buffer);
  return stop;
}

int open_socket(){
  int openSocket;
  openSocket = socket(AF_INET, SOCK_DGRAM, 0);
  if(openSocket < 0)
  {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }
  return openSocket;
}

struct sockaddr_in format_addr(char * ip, int port)
{
  struct sockaddr_in server_addr;
  memset(&server_addr, 0x0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(ip);
  server_addr.sin_port = htons(port);
  return server_addr;
}

void create_server(char * ip, int port, int bufferLen)
{
  int sd = open_socket();
  int stop = 0;
  struct sockaddr_in server = format_addr(ip, port);
  if(bind(sd, (const struct sockaddr *)&server, sizeof(server)) < 0)
  {
   perror("Bind failed\n");
   exit(EXIT_FAILURE); 
  }
  printf("Client server ready to receive!\nIP: %s\tPORT: %d\n", ip, port);
  while(1)
  {
    stop = receive_message(sd, server, bufferLen);
    if(stop == 1)
    {
      break;
    }
  }
}

void create_client(char * ip, int port, char * ip_server, int port_server, char * message)
{
  int sd = open_socket();
  struct sockaddr_in client = format_addr(ip, port);
  struct sockaddr_in server = format_addr(ip_server, port_server);
  if(bind(sd, (const struct sockaddr *)&client, sizeof(client)) < 0)
  {
   perror("Bind failed\n");
   exit(EXIT_FAILURE); 
  }
  while(1)
  {
    send_message(sd, server, message);
    if(!strcmp(message, "stop"))
    {
      break;
    }
    sleep(3);
  }
} 
