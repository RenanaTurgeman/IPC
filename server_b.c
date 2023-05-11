#include "server_b.h"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void checksum_s(const char *filename)
{
    FILE *file;
    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    unsigned long long sum = 0;

    file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return;
    }

    while ((bytes_read = fread(buffer, sizeof(char), BUFFER_SIZE, file)) != 0)
    {
        for (size_t i = 0; i < bytes_read; i++)
        {
            sum += buffer[i];
        }
    }

    printf("checksum = %llu\n", sum);

    fclose(file);
}

int ipv4_tcp(int port, int isQuiet)
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

    if(isQuiet)
    {
        checksum_s("received_file.txt");
    }

    // Close the file and socket
    close(filefd);
    close(connfd);
    close(sockfd);

    return 0;
}

int ipv4_udp(int port, int isQuiet)
{
    int sockfd, filefd, nbytes;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len;
    char buffer[BUFFER_SIZE * 1024];

    // Create a socket for the server
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
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

    // Set up poll for waiting on the socket
    struct pollfd pollfds[1];
    pollfds[0].fd = sockfd;
    pollfds[0].events = POLLIN;

    // Open a file for writing
    filefd = open("received_file.txt", O_CREAT | O_WRONLY, 0644);
    if (filefd < 0)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Receive data from the client and write it to the file
    int total_bytes_received = 0;
    int timeout_ms = 2000;                           // Wait up to 1 second for data
    while (total_bytes_received < 100 * 1024 * 1024) // Receive 100MB file
    {
        int pollret = poll(pollfds, 1, timeout_ms);
        if (pollret < 0)
        {
            perror("poll");
            exit(EXIT_FAILURE);
        }
        else if (pollret == 0) // Timeout
        {
            // printf("Timeout waiting for data.\n");
            break;
        }
        else // Data available
        {
            cli_len = sizeof(cli_addr);
            nbytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&cli_addr, &cli_len);
            if (nbytes < 0)
            {
                perror("recvfrom");
                exit(EXIT_FAILURE);
            }
            total_bytes_received += nbytes;
            if (write(filefd, buffer, nbytes) != nbytes)
            {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
    }

    // Close the file and socket
    close(filefd);
    close(sockfd);

    return 0;
}

int ipv6_tcp(int port, int isQuiet)
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

    return 0;
}

int ipv6_udp(int port, int isQuiet)
{
    int sockfd, filefd, nbytes;
    struct sockaddr_in6 serv_addr, cli_addr;
    socklen_t cli_len;
    char buffer[BUFFER_SIZE * 1024];

    // Create a socket for the server
    sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
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

    // Set up poll for waiting on the socket
    struct pollfd pollfds[1];
    pollfds[0].fd = sockfd;
    pollfds[0].events = POLLIN;

    // Open a file for writing
    filefd = open("received_file.txt", O_CREAT | O_WRONLY, 0644);
    if (filefd < 0)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Receive data from the client and write it to the file
    int total_bytes_received = 0;
    int timeout_ms = 2000;                           // Wait up to 2 seconds for data
    while (total_bytes_received < 100 * 1024 * 1024) // Receive 100MB file
    {
        int pollret = poll(pollfds, 1, timeout_ms);
        if (pollret < 0)
        {
            perror("poll");
            exit(EXIT_FAILURE);
        }
        else if (pollret == 0) // Timeout
        {
            // printf("Timeout waiting for data.\n");
            break;
        }
        else // Data available
        {
            cli_len = sizeof(cli_addr);
            nbytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&cli_addr, &cli_len);
            if (nbytes < 0)
            {
                perror("recvfrom");
                exit(EXIT_FAILURE);
            }
            total_bytes_received += nbytes;
            if (write(filefd, buffer, nbytes) != nbytes)
            {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
    }

    // Close the file and socket
    close(filefd);
    close(sockfd);

    return 0;
}

int uds_stream(int isQuiet)
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

                if (total != file_size)
                { // if total == file_size so File transfer completed
                    error("File transfer failed.\n");
                }
            }
        }
    }

    return 0;
}

int uds_dgram(int isQuiet)
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

    if (total != file_size)
    { // if total == file_size so File transfer completed
        error("File transfer failed.\n");
    }

    return 0;
}

int mmap_filename(int port, int isQuiet)
{
    struct sockaddr_in serv_addr, client_addr;
    char buffer[4906];
    struct pollfd fds[1];
    int timeout = 5000;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        return -1;
    }
    int optval = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        perror("setsockopt");
        close(sockfd);
        return -1;
    }
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("bind");
        close(sockfd);
        return -1;
    }
    fds[0].fd = sockfd;
    fds[0].events = POLLIN;

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);
    int bite = 0;

    while (1)
    {
        int ret = poll(fds, 1, timeout);
        if (ret == -1)
        {
            perror("poll");
            close(sockfd);
            return -1;
        }
        else if (ret == 0)
        {
            break;
        }
        else
        {
            if (fds[0].revents & POLLIN)
            {
                bzero(buffer, 4096);
                socklen_t clientlen = sizeof(client_addr);

                int n = recvfrom(sockfd, buffer, 4096 - 1, 0, (struct sockaddr *)&client_addr, &clientlen);
                bite += n;
                if (n < 0)
                {
                    perror("recvfrom");
                    close(sockfd);
                    return -1;
                }
            }
        }
    }
    printf("%d\n", bite);
    gettimeofday(&end_time, NULL);

    long start_sec = start_time.tv_sec;
    long start_usec = start_time.tv_usec;
    long end_sec = end_time.tv_sec;
    long end_usec = end_time.tv_usec;
    long start_time_micros = start_sec * 1000000 + start_usec;
    long end_time_micros = end_sec * 1000000 + end_usec;
    long time = end_time_micros - start_time_micros;

    close(sockfd);
    printf("time = %ld", time);
    return time;
}
/*
int mmap_filename(int port)
{
    const int flags = O_WRONLY | O_CREAT | O_TRUNC;
    const int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    const size_t offset = 0;

    int file_desc = open("file.txt", flags, mode);
    if (file_desc == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    int shared_mem_desc = shm_open(SHM_NAME, O_RDONLY, mode);
    if (shared_mem_desc == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    struct stat shared_mem_stats;
    if (fstat(shared_mem_desc, &shared_mem_stats) == -1) {
        perror("fstat");
        exit(EXIT_FAILURE);
    }

    void *addr = mmap(NULL, shared_mem_stats.st_size, PROT_READ, MAP_SHARED, shared_mem_desc, offset);
    if (addr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_written = write(file_desc, addr, shared_mem_stats.st_size);
    if (bytes_written != shared_mem_stats.st_size) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    // Open a new file for writing
    int new_file_desc = open("recieve_file.txt", O_WRONLY | O_CREAT | O_TRUNC, mode);
    if (new_file_desc == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Write the contents of the shared memory object to the new file
    if (write(new_file_desc, addr, shared_mem_stats.st_size) != shared_mem_stats.st_size) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    // Close the new file
    if (close(new_file_desc) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    if (munmap(addr, shared_mem_stats.st_size) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    if (close(file_desc) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    if (close(shared_mem_desc) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink");
        exit(EXIT_FAILURE);
    }
    return 0;
}
*/

int pipe_filename(int isQuiet)
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

    return 0;
}

void received_file(char *type, char *param, int port, int isQuiet)
{
    if (strcmp(type, "ipv4") == 0 && (strcmp(param, "tcp")) == 0)
    {
        ipv4_tcp(port, isQuiet);
    }
    else if (strcmp(type, "ipv4") == 0 && (strcmp(param, "udp")) == 0)
    {
        ipv4_udp(port, isQuiet);
    }
    else if (strcmp(type, "ipv6") == 0 && (strcmp(param, "tcp")) == 0)
    {

        ipv6_tcp(port, isQuiet);
    }
    else if (strcmp(type, "ipv6") == 0 && (strcmp(param, "udp")) == 0)
    {

        ipv6_udp(port, isQuiet);
    }

    else if (strcmp(type, "mmap") == 0 && (strcmp(param, "filename")) == 0)
    {

        mmap_filename(port, isQuiet);
    }
    else if (strcmp(type, "pipe") == 0 && (strcmp(param, "filename")) == 0)
    {

        pipe_filename(isQuiet);
    }
    else if (strcmp(type, "uds") == 0 && (strcmp(param, "dgram")) == 0)
    {

        uds_dgram( isQuiet);
    }
    else if (strcmp(type, "uds") == 0 && (strcmp(param, "stream")) == 0)
    {

        uds_stream(isQuiet);
    }
}

int server_main_test(int argc, char *argv[])
{
    if (argc < 4)
        error("Usage: stnc -s port -p (p for performance test) -q (q for quiet)");
    int port = atoi(argv[2]);
    // printf("Port: %d\n", port);
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Attach socket to the port 8081
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8081);

    // Bind the socket to the specified port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Wait for incoming connection and accept it
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    char *type, *param;
    int isQuiet = 0;
    // Receive messages from the client and print them to the console
    while (1)
    {
        valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread <= 0)
        {
            break;
        }
        // printf("%s\n", buffer); //print which kind

        // Parse data into parameters
        type = strtok(buffer, " ");
        param = strtok(NULL, " ");

        
        char q[10];
        if (argc > 4 && strcmp(argv[3], "-q") == 0)
        {
            isQuiet = 1;
        }

        // printf("Type: %s\n", type);
        // printf("Param: %s\n", param);

        // received_file(type , param, port);
    }

    // Close the socket
    close(new_socket);
    close(server_fd);

    received_file(type, param, port, isQuiet);
    return 0;
}