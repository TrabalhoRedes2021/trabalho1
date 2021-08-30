#ifndef NETWORK_UTIL
#define NETWORK_UTIL

void send_message(int sd, struct sockaddr_in endClient, char * message);
int receive_message(int sd, struct sockaddr_in endClient, int bufferLen);
int open_socket();
struct sockaddr_in format_addr(char * ip, int port);
void create_server(char * ip, int port, int bufferLen);
void create_client(char * ip, int port, char * ip_server, int port_server);

#endif
