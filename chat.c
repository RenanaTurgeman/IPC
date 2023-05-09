/* use:
compile the code with:
gcc stnc.c -o stnc
run as sever:
./stnc -s 12345  (start the server on port 12345)
run as client:
./sttnc -c 127.0.0.1 12345 (contect to the server running at localhost 0n port 12345)
*/

#include "chat.h"

void error_ch(char *msg) {
    perror(msg);
    exit(1);
}

void run_client(char *ip, int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error_ch("ERROR_ch opening socket");

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0)
        error_ch("ERROR_ch invalid address");

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error_ch("ERROR_ch connecting");

    struct pollfd pfds[2]; //we want to monitor 2 socets

    pfds[0].fd = sockfd; //the input is the sock
    pfds[0].events = POLLIN; // Tell me when ready to read
    pfds[1].fd = STDIN_FILENO;
    pfds[1].events = POLLIN; // Tell me when ready to read

    char buffer[BUFFER_SIZE];
    while (1) {
        // Initialize buffer to zero
        memset(buffer, 0, BUFFER_SIZE);

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
                    error_ch("ERROR_ch in send");
            } 

            int pollin_happened_0 = pfds[0].revents & POLLIN; //from server

            if (pollin_happened_0)
            {
                int bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0);

                if (bytes_received < 0)
                    error_ch("ERROR_ch in recv");

                else if (bytes_received == 0)
                {
                    printf("Server disconnected.\n");
                    break;
                }
                buffer[bytes_received] = '\0';/////////////////
                printf("Server: %s\n", buffer);
            }
        }
        
    }

    close(sockfd);
}

void run_server(int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error_ch("ERROR_ch opening socket");

    struct sockaddr_in serv_addr, cli_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error_ch("ERROR_ch on binding");

    listen(sockfd, 5);

    socklen_t clilen = sizeof(cli_addr);
    int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
        error_ch("ERROR_ch on accept");

    struct pollfd pfds[2];
    pfds[0].fd = newsockfd;
    pfds[0].events = POLLIN;
    pfds[1].fd = STDIN_FILENO;
    pfds[1].events = POLLIN;

    char buffer[BUFFER_SIZE];
    while (1) {
        // Initialize buffer to zero
        memset(buffer, 0, BUFFER_SIZE);
        if (poll(pfds, 2, -1) < 0)
            error_ch("ERROR_ch on poll");
        
         else
        {
            int pollin_happened_1 = pfds[1].revents & POLLIN; 

            if (pollin_happened_1)
            {
                fgets(buffer, BUFFER_SIZE, stdin);

                // Send message to server
                int bytes_sent = send(newsockfd, buffer, strlen(buffer), 0);
                if (bytes_sent < 0)
                    error_ch("ERROR_ch in send");

            }

            int pollin_happened_0 = pfds[0].revents & POLLIN; //from server

            if (pollin_happened_0)
            {
                // Initialize buffer to zero
                memset(buffer, 0, BUFFER_SIZE);

                // Receive message from server
                int bytes_received = recv(newsockfd, buffer, BUFFER_SIZE, 0);

                if (bytes_received < 0)
                    error_ch("ERROR_ch recv");

                else if (bytes_received == 0)
                {
                    printf("Server disconnected.\n");
                    break;
                }
                buffer[bytes_received] = '\0';/////////////////

                printf("Client: %s\n", buffer);
            }
        }

    }

    close(newsockfd);
    close(sockfd);
}

int main_chat(int argc, char *argv[]) {
    if (argc < 2)
        error_ch("Usage: stnc -s PORT (server) or stnc -c IP PORT (client)");

    if (strcmp(argv[1], "-s") == 0) {
        if (argc < 3)
            error_ch("ERROR_ch no port provided for server");
        int port = atoi(argv[2]);
        run_server(port); 
    } else if (strcmp(argv[1], "-c") == 0) {
        if (argc < 4)
            error_ch("ERROR_ch no IP address or port provided for client");
        char *ip = argv[2];
        int port = atoi(argv[3]);
        run_client(ip, port);
    } else {
        error_ch("ERROR_ch invalid arguments");
    }
    return 0;
}
