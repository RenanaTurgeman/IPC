#include "client_b.h"

void error_c(const char *msg)
{
    perror(msg);
    exit(1);
}

int ipv4_tcp_c(char *ip_address, int port)
{
    printf("ipv4_tcp,");
    struct timeval start, end;

    int sockfd, filefd, nbytes;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE_C];

    // Open the file for reading
    filefd = open("file.txt", O_RDONLY);
    if (filefd < 0)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Create a socket for the client
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip_address, &serv_addr.sin_addr) <= 0)
    {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    gettimeofday(&start, NULL); // get start time before start to send

    // Read from the file and send to the server
    while ((nbytes = read(filefd, buffer, sizeof(buffer))) > 0)
    {
        if (write(sockfd, buffer, nbytes) != nbytes)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }
    gettimeofday(&end, NULL); // get end time after finish to send

    // Close the file and socket
    close(filefd);
    close(sockfd);
    long seconds = end.tv_sec - start.tv_sec;
    long useconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds * 1000.0 + useconds / 1000.0;
    printf("%.2f\n", elapsed);
    return 0;
}


int ipv4_udp_c(char *ip_address, int port)
{
    printf("ipv4_udp,");
    struct timeval start, end;

    int sockfd, filefd, nbytes, n_sent;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE_C];

    // Open the file for reading
    filefd = open("file.txt", O_RDONLY);
    if (filefd < 0)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Create a socket for the client
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip_address, &serv_addr.sin_addr) <= 0)
    {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    gettimeofday(&start, NULL); // get start time before start to send

    // Read from the file and send to the server
    while ((nbytes = read(filefd, buffer, sizeof(buffer))) > 0)
    {
        n_sent = sendto(sockfd, buffer, nbytes, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        if (n_sent < 0)
        {
            perror("sendto");
            exit(EXIT_FAILURE);
        }
        else if (n_sent != nbytes)
        {
            fprintf(stderr, "sendto: Sent %d bytes instead of %d bytes\n", n_sent, nbytes);
        }
    }
    gettimeofday(&end, NULL); // get end time after finish to send

    // Close the file and socket
    close(filefd);
    close(sockfd);
    long seconds = end.tv_sec - start.tv_sec;
    long useconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds * 1000.0 + useconds / 1000.0;
    printf("%.2f\n", elapsed); // in ms

    return 0;
}


int ipv6_udp_c(char *ip_address, int port)
{
    printf("ipv6_udp,");
    struct timeval start, end;

    int sockfd, filefd, nbytes;
    struct sockaddr_in6 serv_addr;
    char buffer[BUFFER_SIZE_C];

    // Open the file for reading
    filefd = open("file.txt", O_RDONLY);
    if (filefd < 0)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Create a socket for the client
    sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_port = htons(port);
    if (inet_pton(AF_INET6, ip_address, &serv_addr.sin6_addr) <= 0) 
    // if (inet_pton(AF_INET6, SERVER_IP_6_C, &serv_addr.sin6_addr) <= 0)
    {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    // Start timer
    gettimeofday(&start, NULL);

    // Read from the file and send to the server
    while ((nbytes = read(filefd, buffer, sizeof(buffer))) > 0)
    {
        if (sendto(sockfd, buffer, nbytes, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != nbytes)
        {
            perror("sendto");
            exit(EXIT_FAILURE);
        }
    }

    // Stop timer
    gettimeofday(&end, NULL);

    // Close the file and socket
    close(filefd);
    close(sockfd);

    long seconds = end.tv_sec - start.tv_sec;
    long useconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds * 1000.0 + useconds / 1000.0;
    printf("%.2f\n", elapsed); // in ms

    return 0;
}


int ipv6_tcp_c(char *ip_address, int port)
{
    printf("ipv6_tcp,");
    struct timeval start, end;

    int sockfd, filefd, nbytes;
    struct sockaddr_in6 serv_addr;
    char buffer[BUFFER_SIZE_C];

    // Open the file for reading
    filefd = open("file.txt", O_RDONLY);
    if (filefd < 0)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Create a socket for the client
    sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_port = htons(port);
    // if (inet_pton(AF_INET6, ip_address, &serv_addr.sin6_addr) <= 0) 
    if (inet_pton(AF_INET6, SERVER_IP_6_C, &serv_addr.sin6_addr) <= 0)
    {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    gettimeofday(&start, NULL); // get start time before start to send

    // Read from the file and send to the server
    while ((nbytes = read(filefd, buffer, sizeof(buffer))) > 0)
    {
        if (write(sockfd, buffer, nbytes) != nbytes)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }
    gettimeofday(&end, NULL); // get end time after finish to send

    // Close the file and socket
    close(filefd);
    close(sockfd);
    long seconds = end.tv_sec - start.tv_sec;
    long useconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds * 1000.0 + useconds / 1000.0;
    printf("%.2f\n", elapsed); // in ms

    return 0;
}
/*
int ipv6_udp_c(int port)
{
    printf("ipv6_udp,");
    struct timeval start, end;

    int sockfd, filefd, nbytes, n_sent;
    struct sockaddr_in6 serv_addr;
    char buffer[BUFFER_SIZE_C];

    // Open the file for reading
    filefd = open("file.txt", O_RDONLY);
    if (filefd < 0)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Create a socket for the client
    sockfd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_port = htons(port);
    if (inet_pton(AF_INET6, ip_address, &serv_addr.sin6_addr) <= 0)
    // if (inet_pton(AF_INET6, SERVER_IP_6_C, &serv_addr.sin6_addr) <= 0)
    {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    gettimeofday(&start, NULL); // get start time before send

    // Read from the file and send to the server
    while ((nbytes = read(filefd, buffer, sizeof(buffer))) > 0)
    {
        n_sent = sendto(sockfd, buffer, nbytes, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        if (n_sent < 0)
        {
            perror("sendto");
            exit(EXIT_FAILURE);
        }
        else if (n_sent != nbytes)
        {
            fprintf(stderr, "sendto: Sent %d bytes instead of %d bytes\n", n_sent, nbytes);
        }
    }

    gettimeofday(&end, NULL); // get end time after finish to send

    // Close the file and socket
    if (close(filefd) < 0)
    {
        perror("close");
        exit(EXIT_FAILURE);
    }
    if (close(sockfd) < 0)
    {
        perror("close");
        exit(EXIT_FAILURE);
    }
    long seconds = end.tv_sec - start.tv_sec;
    long useconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds * 1000.0 + useconds / 1000.0;
    printf("%.2f\n", elapsed); // ms
    return 0;
}
*/
int uds_stream_c()
{
    printf("uds_stream,");
    struct timeval start, end;

    int s, len;
    struct sockaddr_un remote = {
        .sun_family = AF_UNIX,
        .sun_path = SOCK_PATH_C,
    };
    char buf[1024];

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    len = strlen(remote.sun_path) + sizeof(remote.sun_family);
    if (connect(s, (struct sockaddr *)&remote, len) == -1)
    {
        perror("connect");
        exit(1);
    }

    // Open file to be sent
    int fd;
    if ((fd = open("file.txt", O_RDONLY)) < 0)
    {
        perror("open");
        exit(1);
    }

    // Get file size
    struct stat st;
    if (fstat(fd, &st) < 0)
    {
        perror("fstat");
        exit(1);
    }
    int file_size = st.st_size;

    gettimeofday(&start, NULL); // get start time before start to send

    // Send file size to server
    if (send(s, &file_size, sizeof(file_size), 0) < 0)
    {
        perror("send");
        exit(1);
    }

    // Send file data to server
    int total = 0, n;
    while (total < file_size)
    {
        n = read(fd, buf, sizeof(buf));
        if (n < 0)
        {
            perror("read");
            exit(1);
        }

        if (send(s, buf, n, 0) < 0)
        {
            perror("send");
            exit(1);
        }

        total += n;
    }
    gettimeofday(&end, NULL); // get end time after finish to send

    close(fd);
    close(s);

    long seconds = end.tv_sec - start.tv_sec;
    long useconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds * 1000.0 + useconds / 1000.0;
    printf("%.2f\n", elapsed);

    return 0;
}

int uds_dgram_c()
{
    printf("uds_dgram,");
    struct timeval start, end;

    int s;
    struct sockaddr_un remote = {
        .sun_family = AF_UNIX,
        .sun_path = SOCK_PATH_C};

    if ((s = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    // Open the file to send
    int fd;
    if ((fd = open("file.txt", O_RDONLY)) < 0)
    {
        perror("open");
        exit(1);
    }

    // Get file size
    off_t file_size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    gettimeofday(&start, NULL); // get start time before start to send

    // Send file size to server
    if (sendto(s, &file_size, sizeof(file_size), 0, (struct sockaddr *)&remote, sizeof(remote)) < 0)
    {
        perror("sendto");
        exit(1);
    }

    // Send file data to server
    char buf[1024];
    int nread;
    while ((nread = read(fd, buf, sizeof(buf))) > 0)
    {
        if (sendto(s, buf, nread, 0, (struct sockaddr *)&remote, sizeof(remote)) < 0)
        {
            perror("sendto");
            exit(1);
        }
    }
    gettimeofday(&end, NULL); // get end time after finish to send

    close(fd);
    close(s);
    long seconds = end.tv_sec - start.tv_sec;
    long useconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds * 1000.0 + useconds / 1000.0;
    printf("%.2f\n", elapsed);

    return 0;
}
/*
int mmap_filename_c(int port){
    printf("mmap_filename,");
    struct timeval start, end;

    int sockfd, n;
    struct sockaddr_in serv_addr;
    const char *filename = "file.txt";
    int fd;
    struct stat filestat;
    char *filedata;

    // Open file
    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        error_c("Error opening file.");
    }

    // Get file size
    if (fstat(fd, &filestat) < 0) {
        error_c("Error getting file size.");
    }

    // Map the file into memory
    // filedata = mmap(0, filestat.st_size, PROT_READ, MAP_SHARED, fd, 0);
    filedata = mmap(0, 100 * 1024 * 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (filedata == MAP_FAILED) {
        error_c("Error mapping file into memory.");
    }

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        error_c("Error opening socket.");
    }

    // Set server address
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    gettimeofday(&start, NULL); // get start time before start to send

    // Send file data to server
    n = sendto(sockfd, filedata, filestat.st_size, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (n < 0) {
        error_c("Error sending data.");
    }

    gettimeofday(&end, NULL); // get end time after finish to send

    // Close file and socket
    close(fd);
    close(sockfd);

    // Unmap the file from memory
    if (munmap(filedata, filestat.st_size) < 0) {
        error_c("Error unmapping file.");
    }
    long seconds = end.tv_sec - start.tv_sec;
    long useconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds * 1000.0 + useconds / 1000.0;
    printf("%.2f\n", elapsed);

    return 0;
}


int mmap_filename_c(int port)
{
    // Open the file
    int fd = open("file.txt", O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file");
        exit(1);
    }

    // Get the file size
    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == (off_t)-1) {
        perror("Error getting file size");
        exit(1);
    }

    // Map the file into memory
    void *data = mmap(NULL, file_size, PROT_READ, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED)
    {
        perror("Error mapping file to memory");
        exit(1);
    }

    // Create a socket and connect to the server
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Error creating socket");
        exit(1);
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8082);
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        perror("Invalid address/ Address not supported");
        exit(1);
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        perror("Error connecting to server");
        exit(1);
    }

    // Send the mapped memory region to the server
    if (send(sockfd, &file_size, sizeof(file_size), 0) == -1) {
        perror("Error sending file size to server");
        exit(1);
    }

    if (send(sockfd, data, file_size, 0) == -1) {
        perror("Error sending data to server");
        exit(1);
    }

    // Clean up
    close(sockfd);
    if (munmap(data, file_size) == -1) {
        perror("Error unmapping memory");
        exit(1);
    }
    close(fd);

    return 0;
}
*/

int mmap_filename_c(char *ip_address, int port)
{
    struct sockaddr_in serv_addr;
    struct stat filestat;
    int fd = open("file.txt", O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        return -1;
    }
    if (fstat(fd, &filestat) < 0)
    {
        perror("fstat");
        close(fd);
        return -1;
    }

    off_t file_size = filestat.st_size;
    char *filedata = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (filedata == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        return -1;
    }

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        munmap(filedata, file_size);
        close(fd);
        return -1;
    }
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr(ip_address);

    off_t offset = 0;
    ssize_t bytes_left = file_size;
    ssize_t bytes_sent;
    while (bytes_left > 0)
    {
        size_t chunk_size = (bytes_left > 1024) ? 1024 : bytes_left;
        bytes_sent = sendto(sockfd, filedata + offset, chunk_size, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        if (bytes_sent < 0)
        {
            perror("sendto");
            munmap(filedata, file_size);
            close(fd);
            close(sockfd);
            return -1;
        }
        offset += bytes_sent;
        bytes_left -= bytes_sent;
    }

    munmap(filedata, file_size);
    close(fd);
    close(sockfd);
    return 0;
}

int pipe_filename_c()
{
    printf("pipe_filename,");
    struct timeval start, end;

    int fd;
    char buffer[BUFFER_SIZE_C];
    ssize_t bytes_read;
    FILE *file;

    // Open the file to be sent
    file = fopen("file.txt", "r");
    if (file == NULL)
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Open the named pipe for writing
    fd = open(FIFO_NAME_C, O_WRONLY);
    if (fd == -1)
    {
        perror("Failed to open named pipe");
        exit(EXIT_FAILURE);
    }

    gettimeofday(&start, NULL); // get start time before start to send

    // Read data from the file and write it to the named pipe
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE_C, file)) > 0)
    {
        write(fd, buffer, bytes_read);
    }
    gettimeofday(&end, NULL); // get end time after finish to send

    // Close the file and named pipe
    fclose(file);
    close(fd);

    long seconds = end.tv_sec - start.tv_sec;
    long useconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds * 1000.0 + useconds / 1000.0;
    printf("%.2f\n", elapsed);

    return 0;
}

void send_file(char *type, char *param, char *ip_address, int port)
{
    if (strcmp(type, "ipv4") == 0 && (strcmp(param, "tcp")) == 0)
    {
        ipv4_tcp_c(ip_address, port);
    }
    else if (strcmp(type, "ipv4") == 0 && (strcmp(param, "udp")) == 0)
    {
        ipv4_udp_c(ip_address, port);
    }
    else if (strcmp(type, "ipv6") == 0 && (strcmp(param, "tcp")) == 0)
    {

        ipv6_tcp_c(ip_address, port);
    }
    else if (strcmp(type, "ipv6") == 0 && (strcmp(param, "udp")) == 0)
    {

        ipv6_udp_c(ip_address, port);
    }

    else if (strcmp(type, "mmap") == 0 && (strcmp(param, "filename")) == 0)
    {

        mmap_filename_c(ip_address, port);
    }
    else if (strcmp(type, "pipe") == 0 && (strcmp(param, "filename")) == 0)
    {

        pipe_filename_c();
    }
    else if (strcmp(type, "uds") == 0 && (strcmp(param, "dgram")) == 0)
    {

        uds_dgram_c();
    }
    else if (strcmp(type, "uds") == 0 && (strcmp(param, "stream")) == 0)
    {

        uds_stream_c();
    }
}

int client_main_test(int argc, char *argv[])
{
    if (argc < 7)
        error_c("Usage: stnc -c IP PORT -p <type> <param>");

    // Set up socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error opening socket");
        exit(1);
    }

    char ip_address[1024];
    int port;
    strcpy(ip_address, argv[2]);
    // printf("IP adress: %s\n", ip_address);
    port = atoi(argv[3]);
    // printf("Port: %d\n", port);

    // Set up server address
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8081);
    // serv_addr.sin_addr.s_addr = inet_addr(ip_address); // Server IP address
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error connecting to server");
        exit(1);
    }

    // Send data over socket
    char type[1024], param[1024];
    strcpy(type, argv[5]);
    strcpy(param, argv[6]);
    // printf("type: %s\n", type);
    // printf("param: %s\n", param);

    char data[2048];
    sprintf(data, "%s %s", type, param);
    if (send(sockfd, data, strlen(data), 0) < 0)
    {
        perror("Error sending data");
        exit(1);
    }

    // Close socket
    close(sockfd);
    sleep(1);

    send_file(type, param, ip_address, port);
    return 0;
}
