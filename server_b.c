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

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024
#define SOCK_PATH "echo_socket"


int ipv4_tcp(){
     int sockfd, connfd, filefd, nbytes;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len;
    char buffer[BUFFER_SIZE];

    // Create a socket for the server
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERVER_PORT);

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Accept a connection from a client
    cli_len = sizeof(cli_addr);
    connfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len);
    if (connfd < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Open a file for writing
    filefd = open("received_file.txt", O_CREAT | O_WRONLY, 0644);
    if (filefd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Receive data from the client and write it to the file
    while ((nbytes = read(connfd, buffer, sizeof(buffer))) > 0) {
        if (write(filefd, buffer, nbytes) != nbytes) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    // Close the file and socket
    close(filefd);
    close(connfd);
    close(sockfd);

    // Print the contents of the file to the screen
    FILE *fp = fopen("received_file.txt", "r");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }
    fclose(fp);
    return 0;
}

int ipv4_udp(){
    int sockfd, nbytes;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len;
    char buffer[BUFFER_SIZE];

    // Create a socket for the server
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERVER_PORT);

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Receive data from a client and write it to a file
    cli_len = sizeof(cli_addr);
    if ((nbytes = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &cli_addr, &cli_len)) < 0) {
        perror("recvfrom");
        exit(EXIT_FAILURE);
    }

    // Open a file for writing
    int filefd = open("received_file.txt", O_CREAT | O_WRONLY, 0644);
    if (filefd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Write the received data to the file
    if (write(filefd, buffer, nbytes) != nbytes) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    // Close the file and socket
    close(filefd);
    close(sockfd);

    // Print the contents of the file to the screen
    FILE *fp = fopen("received_file.txt", "r");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }
    fclose(fp);
    return 0;
}

int ipv6_tcp() {
    int sockfd, connfd, filefd, nbytes;
    struct sockaddr_in6 serv_addr, cli_addr;
    socklen_t cli_len;
    char buffer[BUFFER_SIZE];

    // Create a socket for the server
    sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_addr = in6addr_any;
    serv_addr.sin6_port = htons(SERVER_PORT);

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Accept a connection from a client
    cli_len = sizeof(cli_addr);
    connfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len);
    if (connfd < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Open a file for writing
    filefd = open("received_file.txt", O_CREAT | O_WRONLY, 0644);
    if (filefd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Receive data from the client and write it to the file
    while ((nbytes = read(connfd, buffer, sizeof(buffer))) > 0) {
        if (write(filefd, buffer, nbytes) != nbytes) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    // Close the file and socket
    close(filefd);
    close(connfd);
    close(sockfd);

    // Print the contents of the file to the screen
    FILE *fp = fopen("received_file.txt", "r");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }
    fclose(fp);
    return 0;
}

int ipv6_udp() {
    int sockfd, connfd, filefd, nbytes;
    struct sockaddr_in6 serv_addr, cli_addr;
    socklen_t cli_len;
    char buffer[BUFFER_SIZE];

    // Create a socket for the server
    sockfd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_addr = in6addr_any;
    serv_addr.sin6_port = htons(SERVER_PORT);

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

   // Receive data from a client and write it to a file
    cli_len = sizeof(cli_addr);
    if ((nbytes = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &cli_addr, &cli_len)) < 0) {
        perror("recvfrom");
        exit(EXIT_FAILURE);
    }

    // Open a file for writing
    filefd = open("received_file.txt", O_CREAT | O_WRONLY, 0644);
    if (filefd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Write data to the file
    if (write(filefd, buffer, nbytes) != nbytes) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    // Close the file and socket
    close(filefd);
    close(connfd);
    close(sockfd);

    // Print the contents of the file to the screen
    FILE *fp = fopen("received_file.txt", "r");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }
    fclose(fp);
    return 0;
}

int uds_dgram() {
    // int sockfd, filefd, nbytes;
    // struct sockaddr_un serv_addr, cli_addr;
    // socklen_t cli_len;
    // char buffer[BUFSIZ];

    // // Create a socket for the server
    // sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    // if (sockfd < 0) {
    //     perror("socket");
    //     exit(EXIT_FAILURE);
    // }

    // // Set up the server address
    // memset(&serv_addr, 0, sizeof(serv_addr));
    // serv_addr.sun_family = AF_UNIX;
    // strncpy(serv_addr.sun_path, SERVER_SOCKET_PATH, sizeof(serv_addr.sun_path) - 1);

    // // unlink(local.sun_path);
    // // Bind the socket to the server address
    // if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    //     perror("bind");
    //     exit(EXIT_FAILURE);
    // }

    // // Open a file for writing
    // filefd = open("received_file.txt", O_CREAT | O_WRONLY, 0644);
    // if (filefd < 0) {
    //     perror("open");
    //     exit(EXIT_FAILURE);
    // }

    // // Receive data from the client and write it to the file
    // cli_len = sizeof(cli_addr);
    // while ((nbytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &cli_addr, &cli_len)) > 0) {
    //     if (write(filefd, buffer, nbytes) != nbytes) {
    //         perror("write");
    //         exit(EXIT_FAILURE);
    //     }
    // }

    // // Close the file and socket
    // close(filefd);
    // close(sockfd);

    // // Print the contents of the file to the screen
    // FILE *fp = fopen("received_file.txt", "r");
    // if (fp == NULL) {
    //     perror("fopen");
    //     exit(EXIT_FAILURE);
    // }
    // while (fgets(buffer, sizeof(buffer), fp) != NULL) {
    //     printf("%s", buffer);
    // }
    // fclose(fp);

    int s, s2, len, fd;
    struct sockaddr_un remote, local = {
            .sun_family = AF_UNIX,
    };
    char buf[1024];

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
            perror("socket");
            exit(1);
    }

    strcpy(local.sun_path, SOCK_PATH);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(s, (struct sockaddr *)&local, len) == -1) {
            perror("bind");
            exit(1);
    }

    if (listen(s, 5) == -1) {
            perror("listen");
            exit(1);
    }

    for (;;) {
            printf("Waiting for a connection...\n");
            socklen_t slen = sizeof(remote);
            if ((s2 = accept(s, (struct sockaddr *)&remote, &slen)) == -1) {
                    perror("accept");
                    exit(1);
            }

            printf("Connected.\n");

            // Read file size from client
            int file_size;
            if (recv(s2, &file_size, sizeof(file_size), 0) < 0) {
                    perror("recv");
                    exit(1);
            }

            // Read file data from client and save to file
            if ((fd = open("received_file.txt", O_CREAT|O_WRONLY, 0644)) < 0) {
                    perror("open");
                    exit(1);
            }

            int total = 0, n;
            while (total < file_size) {
                    n = recv(s2, buf, sizeof(buf), 0);
                    if (n <= 0) {
                            if (n < 0) perror("recv");
                            break;
                    }

                    if (write(fd, buf, n) < 0) {
                            perror("write");
                            break;
                    }

                    total += n;
            }

            close(fd);
            close(s2);

            if (total == file_size) {
                    printf("File transfer completed.\n");
            } else {
                    printf("File transfer failed.\n");
            }
    }

    return 0;
}


int main(int argc, char *argv[]) {
    // ipv4_tcp();
    // ipv4_udp();
    // ipv6_tcp();
    // ipv6_udp();
    uds_dgram();

    return 0;
}
