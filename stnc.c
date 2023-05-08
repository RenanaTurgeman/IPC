/* use:
compile the code with:
gcc stnc.c -o stnc
run as sever:
./stnc -s 12345  (start the server on port 12345)
run as client:
./sttnc -c 127.0.0.1 12345 (contect to the server running at localhost 0n port 12345)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>

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

    struct pollfd pfds[2]; //we want to monitor 2 socets

    pfds[0].fd = sockfd; //the input is the sock
    pfds[0].events = POLLIN; // Tell me when ready to read
    pfds[1].fd = STDIN_FILENO;
    pfds[1].events = POLLIN; // Tell me when ready to read

    char buffer[BUFFER_SIZE];
    while (1) {
        // printf("Enter message: ");

        int num_events = poll(pfds, 2, -1); 

        if (num_events == 0) {
            printf("Poll timed out!\n");
        } else {
            int pollin_happened_1 = pfds[1].revents & POLLIN; //there is input

            if (pollin_happened_1) {
                
                fgets(buffer, BUFFER_SIZE, stdin);
                buffer[strlen(buffer) - 1] = '\0';

                // Send message to server
                int bytes_sent = send(sockfd, buffer, strlen(buffer), 0);
                if (bytes_sent < 0)
                    error("ERROR in send");
            } 

            int pollin_happened_0 = pfds[0].revents & POLLIN; //from server

            if (pollin_happened_0)
            {
                int bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0);

                if (bytes_received < 0)
                    error("ERROR in recv");

                else if (bytes_received == 0)
                {
                    printf("Server disconnected.\n");
                    break;
                }

                printf("Server: %s\n", buffer);
            }
        }
        
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

    struct pollfd pfds[2];
    pfds[0].fd = newsockfd;
    pfds[0].events = POLLIN;
    pfds[1].fd = STDIN_FILENO;
    pfds[1].events = POLLIN;

    char buffer[BUFFER_SIZE];
    while (1) {
        if (poll(pfds, 2, -1) < 0)
            error("ERROR on poll");
        
         else
        {
            int pollin_happened_1 = pfds[1].revents & POLLIN; 

            if (pollin_happened_1)
            {
                fgets(buffer, BUFFER_SIZE, stdin);
                // buffer[strlen(buffer) - 1] = '\0';

                // Send message to server
                int bytes_sent = send(newsockfd, buffer, strlen(buffer), 0);
                if (bytes_sent < 0)
                    error("ERROR in send");

            }

            int pollin_happened_0 = pfds[0].revents & POLLIN; //from server

            if (pollin_happened_0)
            {
                // Receive message from server
                int bytes_received = recv(newsockfd, buffer, BUFFER_SIZE, 0);

                if (bytes_received < 0)
                    error("ERROR recv");

                else if (bytes_received == 0)
                {
                    printf("Server disconnected.\n");
                    break;
                }
                printf("Client: %s\n", buffer);
            }
        }

    }

    close(newsockfd);
    close(sockfd);
}



int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: stnc [options]\n");
        printf("For help, use stnc -h\n");
        exit(0);
    }

    int isServer = 0;
    int isClient = 0;
    int quietMode = 0;
    int performTest = 0;
    char *address = NULL;
    char *port = NULL;
    char *type = NULL;
    char *param = NULL;

    int opt;
    while ((opt = getopt(argc, argv, "sqhsc:p:")) != -1) {
        switch (opt) {
            case 's':
                isServer = 1;
                port = strtok(NULL, ":");
                break;
            case 'c':
                isClient = 1;
                address = strtok(optarg, ":");
                port = strtok(NULL, ":");
                break;
            case 'p':
                performTest = 1;
                type = strtok(optarg, " ");
                param = strtok(NULL, " ");
                break;
            case 'q':
                quietMode = 1;
                break;
            case 'h':
                printf("Usage:\n");
                printf("Server: stnc -s PORT -p -q\n");
                printf("Client: stnc -c IP:PORT -p <type> <param>\n");
                printf("Options:\n");
                printf("  -s              Start server\n");
                printf("  -c IP:PORT      Connect to server\n");
                printf("  -p TYPE PARAM   Perform performance test with TYPE and PARAM\n");
                printf("  -q              Quiet mode, only show test results\n");
                printf("  -h              Show this help message\n");
                exit(0);
            default:
                printf("Invalid argument. Use stnc -h for help.\n");
                exit(1);
        }
    }

if (!isServer && !isClient) {
    printf("Invalid argument. Use stnc -h for help.\n");
    exit(1);
}

if (isServer) {
    if (!port || !performTest) {
        printf("Invalid argument. Use stnc -h for help.\n");
        exit(1);
    }
    server(atoi(port), performTest, quietMode);
} else {
    if (!address || !port || !performTest) {
        printf("Invalid argument. Use stnc -h for help.\n");
        exit(1);
    }
    client(address, atoi(port), type, param, performTest);
}

return 0;
}
