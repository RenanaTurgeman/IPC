#include "server_b.h"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int ipv4_tcp(int port)
{
    int sockfd, connfd, filefd, nbytes;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len;
    char buffer[BUFFER_SIZE];

    // Create a socket for the server
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Accept a connection from a client
    cli_len = sizeof(cli_addr);
    connfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_len);
    if (connfd < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Open a file for writing
    filefd = open("received_file.txt", O_CREAT | O_WRONLY, 0644);
    if (filefd < 0)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Receive data from the client and write it to the file
    while ((nbytes = read(connfd, buffer, sizeof(buffer))) > 0)
    {
        if (write(filefd, buffer, nbytes) != nbytes)
        {
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
    if (fp == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        printf("%s", buffer);
    }
    fclose(fp);
    return 0;
}

int ipv4_udp(int port)
{
    int sockfd, nbytes;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len;
    char buffer[BUFFER_SIZE];

    // Create a socket for the server
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Set up the pollfd structure for the socket
    struct pollfd pollfds[1];
    pollfds[0].fd = sockfd;
    pollfds[0].events = POLLIN;

    // Receive data from a client and write it to a file
    int num_ready;
    while (1)
    {
        // Wait for data to become available on the socket
        num_ready = poll(pollfds, 1, -1);
        if (num_ready < 0)
        {
            perror("poll");
            exit(EXIT_FAILURE);
        }
        else if (num_ready == 0)
        {
            continue;
        }

        // If there is data available, receive it and write it to a file
        cli_len = sizeof(cli_addr);
        if ((nbytes = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&cli_addr, &cli_len)) < 0)
        {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }

        // Open a file for writing
        int filefd = open("received_file.txt", O_CREAT | O_WRONLY | O_APPEND, 0644);
        if (filefd < 0)
        {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // Write the received data to the file
        if (write(filefd, buffer, nbytes) != nbytes)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }

        // Close the file
        close(filefd);

        // Print the contents of the file to the screen
        FILE *fp = fopen("received_file.txt", "r");
        if (fp == NULL)
        {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
        while (fgets(buffer, sizeof(buffer), fp) != NULL)
        {
            printf("%s", buffer);
        }
        fclose(fp);
    }

    // Close the socket
    close(sockfd);

    return 0;
}

int ipv6_tcp(int port)
{
    int sockfd, connfd, filefd, nbytes;
    struct sockaddr_in6 serv_addr, cli_addr;
    socklen_t cli_len;
    char buffer[BUFFER_SIZE];

    // Create a socket for the server
    sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_addr = in6addr_any;
    serv_addr.sin6_port = htons(port);

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Accept a connection from a client
    cli_len = sizeof(cli_addr);
    connfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_len);
    if (connfd < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Open a file for writing
    filefd = open("received_file.txt", O_CREAT | O_WRONLY, 0644);
    if (filefd < 0)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Receive data from the client and write it to the file
    while ((nbytes = read(connfd, buffer, sizeof(buffer))) > 0)
    {
        if (write(filefd, buffer, nbytes) != nbytes)
        {
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
    if (fp == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        printf("%s", buffer);
    }
    fclose(fp);
    return 0;
}

int ipv6_udp(int port)
{
    int sockfd, filefd, nbytes;
    struct sockaddr_in6 serv_addr, cli_addr;
    socklen_t cli_len;
    char buffer[BUFFER_SIZE];

    // Create a socket for the server
    sockfd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_addr = in6addr_any;
    serv_addr.sin6_port = htons(port);

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Set up the poll structure
    struct pollfd fds[1];
    fds[0].fd = sockfd;
    fds[0].events = POLLIN;

    // Wait for data to arrive on the socket
    if (poll(fds, 1, -1) < 0)
    {
        perror("poll");
        exit(EXIT_FAILURE);
    }

    // Receive data from a client and write it to a file
    cli_len = sizeof(cli_addr);
    if ((nbytes = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&cli_addr, &cli_len)) < 0)
    {
        perror("recvfrom");
        exit(EXIT_FAILURE);
    }

    // Open a file for writing
    filefd = open("received_file.txt", O_CREAT | O_WRONLY, 0644);
    if (filefd < 0)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Write data to the file
    if (write(filefd, buffer, nbytes) != nbytes)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }

    // Close the file and socket
    close(filefd);
    close(sockfd);

    // Print the contents of the file to the screen
    FILE *fp = fopen("received_file.txt", "r");
    if (fp == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        printf("%s", buffer);
    }
    fclose(fp);
    return 0;
}

int uds_stream()
{
    int s, s2, len, fd;
    struct sockaddr_un remote, local = {
                                   .sun_family = AF_UNIX, // Set the socket family to UNIX
                               };
    char buf[BUFFER_SIZE]; // Create a buffer for reading/writing data

    // Create a UNIX socket for communication
    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    // Set up the local socket address
    strcpy(local.sun_path, SOCK_PATH);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(s, (struct sockaddr *)&local, len) == -1)
    {
        perror("bind");
        exit(1);
    }
    // Set the socket to listen for incoming connections
    if (listen(s, 5) == -1)
    {
        perror("listen");
        exit(1);
    }

    // Create an array of pollfd structures to hold the file descriptors being monitored by poll()
    struct pollfd fds[10];
    memset(fds, 0, sizeof(fds));
    fds[0].fd = s;
    fds[0].events = POLLIN;

    while (1)
    {
        printf("Waiting for a connection...\n");
        int rv = poll(fds, 10, -1); // Wait for incoming data or events on the file descriptors in fds
        if (rv == -1)
        {
            perror("poll");
            exit(1);
        }

        for (int i = 0; i < 10; i++)
        {
            if (fds[i].revents == 0)
            { // If there are no events waiting for this file descriptor, skip to the next one
                continue;
            }

            if (fds[i].revents & POLLIN)
            { // If there is incoming data waiting for this file descriptor...
                if (fds[i].fd == s)
                { // ...and it's the socket we're listening on...
                    socklen_t slen = sizeof(remote);
                    if ((s2 = accept(s, (struct sockaddr *)&remote, &slen)) == -1)
                    {
                        perror("accept");
                        exit(1);
                    }

                    printf("Connected.\n");

                    // Read file size from client
                    int file_size;
                    if (recv(s2, &file_size, sizeof(file_size), 0) < 0)
                    {
                        perror("recv");
                        exit(1);
                    }

                    // Read file data from client and save to file
                    if ((fd = open("received_file.txt", O_CREAT | O_WRONLY, 0644)) < 0)
                    {
                        perror("open");
                        exit(1);
                    }

                    int total = 0, n;
                    while (total < file_size)
                    {
                        n = recv(s2, buf, sizeof(buf), 0);
                        if (n <= 0)
                        {
                            if (n < 0)
                                perror("recv");
                            break;
                        }

                        if (write(fd, buf, n) < 0)
                        {
                            perror("write");
                            break;
                        }

                        total += n;
                    }

                    close(fd);
                    close(s2);

                    if (total == file_size)
                    {
                        printf("File transfer completed.\n");
                    }
                    else
                    {
                        printf("File transfer failed.\n");
                    }
                }
            }
        }
    }

    return 0;
}

int uds_dgram()
{
    int s, len, fd;
    struct sockaddr_un local = {
        .sun_family = AF_UNIX,
    };
    char buf[1024];

    if ((s = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    strcpy(local.sun_path, SOCK_PATH);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(s, (struct sockaddr *)&local, len) == -1)
    {
        perror("bind");
        exit(1);
    }

    struct pollfd ufds[1];
    ufds[0].fd = s;
    ufds[0].events = POLLIN;

    printf("Waiting for a datagram...\n");

    int file_size = 0;
    while (1)
    {
        int rv = poll(ufds, 1, -1);
        if (rv == -1)
        {
            perror("poll");
            exit(1);
        }
        if (rv == 0)
        {
            continue;
        }
        if (ufds[0].revents & POLLIN)
        {
            socklen_t slen = sizeof(local);
            if (recvfrom(s, &file_size, sizeof(file_size), 0, (struct sockaddr *)&local, &slen) == -1)
            {
                perror("recvfrom");
                exit(1);
            }
            break;
        }
    }

    // Read file data from client and save to file
    if ((fd = open("received_file.txt", O_CREAT | O_WRONLY, 0644)) < 0)
    {
        perror("open");
        exit(1);
    }

    int total = 0, n;
    while (total < file_size)
    {
        socklen_t slen = sizeof(local);
        n = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *)&local, &slen);
        if (n <= 0)
        {
            if (n < 0)
                perror("recvfrom");
            break;
        }

        if (write(fd, buf, n) < 0)
        {
            perror("write");
            break;
        }

        total += n;
    }

    close(fd);
    close(s);

    if (total == file_size)
    {
        printf("File transfer completed.\n");
    }
    else
    {
        printf("File transfer failed.\n");
    }

    return 0;
}

int mmap_filename(int port)
{
    // Define the socket and client address structures
    int sockfd;
    struct sockaddr_in serv_addr, client_addr;

    // Define the length of the client address
    socklen_t clientlen = sizeof(client_addr);

    // Define the buffer to store the received data
    char buffer[BUFFER_SIZE];

    // Define the file to write the received data
    FILE *file;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("Error opening socket.");
        exit(EXIT_FAILURE);
    }

    // Initialize the server address structure
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    // Bind the socket to the server address and port
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error on binding.");
        exit(EXIT_FAILURE);
    }

    // Display the server listening message
    printf("Server listening on port %d...\n", port);

    // Open the file for writing the received data
    file = fopen("received_text.txt", "w");
    if (file == NULL)
    {
        perror("Error opening file.");
        exit(EXIT_FAILURE);
    }

    // Create a pollfd structure to monitor the socket for activity
    struct pollfd fds[1];
    fds[0].fd = sockfd;
    fds[0].events = POLLIN;

    // Wait for activity on the socket
    while (poll(fds, 1, -1) >= 0)
    {
        // Receive data from the client
        bzero(buffer, BUFFER_SIZE);
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&client_addr, &clientlen);
        if (n < 0)
        {
            perror("Error receiving data.");
            exit(EXIT_FAILURE);
        }

        // Print the received data and write it to the file
        printf("Received message: %s\n", buffer);
        fprintf(file, "%s", buffer);

        // Close the file and socket
        fclose(file);
        close(sockfd);

        // Exit the program
        exit(EXIT_SUCCESS);
    }

    // If the poll function fails, display an error message and exit
    perror("Error on poll.");
    exit(EXIT_FAILURE);

    return 0;
}

int pipe_filename()
{
    int fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    // Create the named pipe
    mkfifo(FIFO_NAME, 0666);

    // Open the named pipe for reading
    fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1)
    {
        perror("Failed to open named pipe");
        exit(EXIT_FAILURE);
    }

    // Open the file to write the received data
    FILE *file = fopen("received_file.txt", "w");
    if (file == NULL)
    {
        perror("Failed to open file for writing");
        exit(EXIT_FAILURE);
    }

    // Read data from the named pipe and write it to the file
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        fwrite(buffer, 1, bytes_read, file);
    }

    // Close the file and named pipe
    fclose(file);
    close(fd);

    // Remove the named pipe file
    unlink(FIFO_NAME);

    printf("File received successfully.\n");

    return 0;
}

void received_file(char* type , char* param, int port){
      if(strcmp(type,"ipv4") == 0 && (strcmp(param, "tcp"))== 0){
        ipv4_tcp(port);
    }
    else if(strcmp(type ,"ipv4") == 0 && (strcmp(param , "udp")) == 0){
        ipv4_udp(port);
    }
    else if(strcmp(type ,"ipv6") == 0 && (strcmp(param , "tcp")) == 0){
   
    ipv6_tcp(port);
    }
    else if(strcmp(type, "ipv6") == 0 && (strcmp(param, "udp")) == 0){

   ipv6_udp(port);
    }

    else if(strcmp(type, "mmap") == 0 && (strcmp(param, "filename")) == 0){
    
    mmap_filename(port);
    }
    else if(strcmp(type ,"pipe") == 0 && (strcmp(param , "filename")) == 0){

    pipe_filename();
    }
        else if(strcmp(type ,"uds") == 0 && (strcmp(param ,"dgram")) == 0){
   
    uds_dgram();
    }
    else if(strcmp(type ,"uds") == 0 && (strcmp(param, "stream")) == 0){
    
    uds_stream();
    }
}

int server_main_test(int argc, char *argv[])
{
     if (argc < 5)
        error("Usage: stnc -s port -p (p for performance test) -q (q for quiet)");
    int port = atoi(argv[2]);
    printf("Port: %d\n", port);
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Attach socket to the port 8081
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8081);

    // Bind the socket to the specified port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Wait for incoming connection and accept it
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    char *type ,*param;
    // Receive messages from the client and print them to the console
    while (1) {
    valread = read(new_socket, buffer, BUFFER_SIZE);
    if (valread <= 0) {
        break;
    }
    printf("%s\n", buffer);

    // Parse data into parameters
     type = strtok(buffer, " ");
     param = strtok(NULL, " ");

    printf("Type: %s\n", type);
    printf("Param: %s\n", param);

    received_file(type , param, port);
    // break;
}


    // Close the socket
    close(new_socket);

    // Parse data into parameters
        // char *type = strtok(buffer, " ");
        // char *param = strtok(NULL, " ");

        // printf("Type1: %s\n", type);
        // printf("Param1: %s\n", param);

    received_file(type , param, port);
    return 0;
}