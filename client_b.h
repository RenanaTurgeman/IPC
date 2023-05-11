#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/un.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/time.h> //for measure times

#define FIFO_NAME_C "myfifo"
#define SERVER_IP_6_C "::1"
// #define SERVER_IP_6_C "2001:db8::1"
#define BUFFER_SIZE_C 1024
#define SOCK_PATH_C "echo_socket"


void error(const char *msg);
int ipv4_tcp_c(char* ip_address , int port);
int ipv4_udp_c(char* ip_address, int port);
int ipv6_tcp_c(char* ip_address, int port);
// int ipv6_udp_c(int port);
int ipv6_udp_c(char *ip_address, int port);
int uds_stream_c();
int uds_dgram_c();
int mmap_filename_c(char *ip_address, int port);
int pipe_filename_c();
void send_file_c(char* type, char* param , char* ip_address, int port);
int client_main_test(int argc, char *argv[]);