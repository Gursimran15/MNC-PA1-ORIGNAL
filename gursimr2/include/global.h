#ifndef GLOBAL_H_
#define GLOBAL_H_

#define HOSTNAME_LEN 128
#define PATH_LEN 256

int connect_to_host(char *server_port);
int client(char *port);
int server(char *arg);
char* findip();
char* findipc();
#endif
