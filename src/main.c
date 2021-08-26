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
#define MAX_BUFFER 200



int main(int argc, char *argv[])
{
  char * ip_server, * ip_client, * message;
  int port_server, port_client, port_server2;
  ip_server = argv[1];
  port_server = atoi(argv[2]);
  port_server2 = atoi(argv[3]);
  ip_client = argv[4];
  port_client = atoi(argv[5]);
  message = argv[6]; 
  
  pid_t pid = fork();
  if(pid == 0)
  {
    create_client(ip_server, port_server2, ip_client, port_client, message);
  }
  create_server(ip_server, port_server, MAX_BUFFER);
  return 0; 
}
