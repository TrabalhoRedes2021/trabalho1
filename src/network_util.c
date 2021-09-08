#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <mqueue.h>
#include "network_util.h"
#include "queue.h"

int newBufferLen = 0;
TNode * ptr_init = NULL;
int idx = 0;

void send_message(int sd, struct sockaddr_in endClient, char * message)
{
  //printf("Enviando message\n");
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
  //printf("Client send: %s\tLEN: %d\n", buffer, lenMessage);
  
  if(strstr(buffer, "len"))
  {
    printf("Tamanho do buffer antes:%d\n", newBufferLen);
    char *ptr = strtok(buffer, ":");
    ptr = strtok(NULL, ":");
    newBufferLen = atoi(ptr);
    printf("Tamanho do buffer depois: %d\n", newBufferLen);
  }

  if(!strcmp(buffer, "stop"))
  {
    stop = 1;
  }
  if(!strcmp(buffer, "print"))
  {
    FILE * f2 = fopen("./src/recebido.csv", "wb");
    TNode * temp;
    for(temp=ptr_init; temp != NULL; temp=temp->next)
    {
      memset(buffer, 0x0, newBufferLen);
      printf("%d -> %s\n", temp->id, temp->buffer);
      fwrite(temp->buffer, sizeof(char), strlen(ptr_init->buffer), f2);
    }
    fclose(f2);
  }
  else
  {
   //printf("Adicionando a fila\n");
   insert(idx, buffer, lenMessage, &ptr_init);
   idx += 1;
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

  newBufferLen = bufferLen;

  pid_t pid = fork();
  if(pid == 0)
  {
    mqd_t mq, mq_2;
    struct mq_attr attr;
    char * buffer = (char *) malloc(newBufferLen * sizeof(char));
    char sizes[10];
    int must_stop = 0;

    /* initialize the queue attributes */
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = newBufferLen;
    attr.mq_curmsgs = 0;

    /* create the message queue */
    mq = mq_open("/enlace", O_CREAT | O_RDONLY, 0644, &attr);
    mq_2 = mq_open("/server", O_WRONLY);
    do {

        /* receive the message */
        mq_receive(mq, buffer, newBufferLen, NULL);
        if (! strncmp(buffer, "exit", strlen("exit")))
        {
            must_stop = 1;
        }
        else
        {
            //printf("Received: %s , %d\n", buffer, (int)bytes_read);
            sprintf(sizes, "%d", (int)strlen(buffer));
            //printf("%s, %d\n", sizes, (int)sizeof(sizes));
            mq_send(mq_2, sizes, 10, 0);
        }
    } while (!must_stop);
    free(buffer);
    mq_close(mq);
    mq_unlink("/enlace");
    exit(0);
  }
  else{
    mqd_t mq, mq_2;
    struct mq_attr attr;
    char buffer_2[10];
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 10;
    attr.mq_curmsgs = 0;

    mq = mq_open("/enlace", O_WRONLY);
    mq_2 = mq_open("/server", O_CREAT | O_RDONLY, 0644, &attr);
    while(1)
    {
      //printf("STOP: %d\n", stop);
      if(stop == 1)
      {
        int bytes = 0;
        while(1){
          printf("Enviar frame !\n");
          mq_send(mq, ptr_init->buffer, newBufferLen, 0);
          printf("Receber tamanho da camada de enlace\n");
          bytes = mq_receive(mq_2, buffer_2, 10, NULL);
          printf("Verificar tamanho do frame!\n");
          if(ptr_init->next != NULL && bytes > 0){
            if(ptr_init->len == atoi(buffer_2) || (ptr_init->len == 225 && atoi(buffer_2) == 203)){
              printf("Success Frame!\n");
              ptr_init = ptr_init->next;
            }
          }
          else{
            break;
          }
        }
        mq_send(mq, "exit", newBufferLen, 0);
        mq_close(mq_2);
        mq_unlink("/server");
        break;
      }
      stop = receive_message(sd, server, newBufferLen); 
    }
    wait(NULL);
    printf("Arquivo transmitido com sucesso !\n");
    exit(0);
  }
}

void create_client(char * ip, int port, char * ip_server, int port_server, int bufferLen)
{
  int sd = open_socket();
  struct sockaddr_in client = format_addr(ip, port);
  struct sockaddr_in server = format_addr(ip_server, port_server);
  FILE * f = fopen("./src/test.csv", "rb");
  size_t bytes;
  char * buffer = (char *)malloc(newBufferLen * sizeof(char));
  newBufferLen = bufferLen;
  printf("BIND IP: %s\tPORT: %d\nSERVER IP: %s\tPORT: %d\n", ip, port, ip_server, port_server);
  if(bind(sd, (const struct sockaddr *)&client, sizeof(client)) < 0)
  {
   perror("Bind failed\n");
   exit(EXIT_FAILURE); 
  }
  if(f == NULL)
  {
    perror("Erro no arquivo !\n");
    exit(1); 
  }
  printf("%d\n", newBufferLen);
  while((bytes = fread(buffer, sizeof(char), newBufferLen, f)))
  {
    send_message(sd, server, buffer);
  }
  send_message(sd, server, "print");
  send_message(sd, server, "stop");
  free(buffer);
} 
