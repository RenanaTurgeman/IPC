#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/un.h>
#include <errno.h>
#include <sys/un.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <netdb.h>
#include <poll.h>

#define MAX_FILENAME_LEN 256
#define MAX_FILE_SIZE 1024 * 1024

#define BUFFER_SIZE 1024
#define SOCK_PATH "echo_socket"
#define FIFO_NAME "myfifo"



void error(const char *msg);
int ipv4_tcp(int port);
int ipv4_udp(int port);
int ipv6_tcp(int port);
int ipv6_udp(int port);
int uds_stream();
int uds_dgram();
int mmap_filename(int port);
int pipe_filename();
void received_file(char* type , char* param, int port);
int server_main_test(int argc, char *argv[]);