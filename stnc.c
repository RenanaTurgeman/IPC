#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void error(char *msg) {
    perror(msg);
    exit(1);
}

void run_client(char *ip, int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0)
        error("ERROR invalid address");

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    char buffer[BUFFER_SIZE];
    while (1) {
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        if (strcmp(buffer, "exit\n") == 0)
            break;
        if (write(sockfd, buffer, strlen(buffer)) < 0)
            error("ERROR writing to socket");
        memset(buffer, 0, BUFFER_SIZE);
        if (read(sockfd, buffer, BUFFER_SIZE - 1) < 0)
            error("ERROR reading from socket");
        printf("Received message: %s", buffer);
    }

    close(sockfd);
}

void run_server(int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    struct sockaddr_in serv_addr, cli_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd, 5);

    socklen_t clilen = sizeof(cli_addr);
    int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");

    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        if (read(newsockfd, buffer, BUFFER_SIZE - 1) < 0)
            error("ERROR reading from socket");
        printf("Received message: %s", buffer);
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        if (strcmp(buffer, "exit\n") == 0)
            break;
        if (write(newsockfd, buffer, strlen(buffer)) < 0)
            error("ERROR writing to socket");
    }

    close(newsockfd);
    close(sockfd);
}

int main(int argc, char *argv[]) {
    if (argc < 2)
        error("Usage: stnc -s PORT (server) or stnc -c IP PORT (client)");

    if (strcmp(argv[1], "-s") == 0) {
        if (argc < 3)
            error("ERROR no port provided for server");
        int port = atoi(argv[2]);
        run_server(port);
    } else if (strcmp(argv[1], "-c") == 0) {
        if (argc < 4)
            error("ERROR no IP address or port provided for client");
char *ip = argv[2];
int port = atoi(argv[3]);
run_client(ip, port);
} else {
error("ERROR invalid arguments");
}
return 0;
}