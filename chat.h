#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>

#define BUFFER_SIZE 1024

void error_ch(char *msg);
void run_client(char *ip, int port) ;
void run_server(int port) ;
int main_chat(int argc, char *argv[]) ;