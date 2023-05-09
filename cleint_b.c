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
#define FIFO_NAME "myfifo"

#define SERVER_IP "127.0.0.1"
#define SERVER_IP_6 "::1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024
#define SOCK_PATH "echo_socket"

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int ipv4_tcp(){
    int sockfd, filefd, nbytes;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];

    // Open the file for reading
    filefd = open("file.txt", O_RDONLY);
    if (filefd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Create a socket for the client
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    // Read from the file and send to the server
    while ((nbytes = read(filefd, buffer, sizeof(buffer))) > 0) {
        if (write(sockfd, buffer, nbytes) != nbytes) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    // Close the file and socket
    close(filefd);
    close(sockfd);
    return 0;
}

int ipv4_udp(){
    int sockfd, filefd, nbytes, n_sent;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];

    // Open the file for reading
    filefd = open("file.txt", O_RDONLY);
    if (filefd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Create a socket for the client
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    // Read from the file and send to the server
    while ((nbytes = read(filefd, buffer, sizeof(buffer))) > 0) {
        n_sent = sendto(sockfd, buffer, nbytes, 0, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
        if (n_sent < 0) {
            perror("sendto");
            exit(EXIT_FAILURE);
        } else if (n_sent != nbytes) {
            fprintf(stderr, "sendto: Sent %d bytes instead of %d bytes\n", n_sent, nbytes);
        }
    }

    // Close the file and socket
    close(filefd);
    close(sockfd);
    return 0;
}

int ipv6_tcp(){
    int sockfd, filefd, nbytes;
    struct sockaddr_in6 serv_addr;
    char buffer[BUFFER_SIZE];

    // Open the file for reading
    filefd = open("file.txt", O_RDONLY);
    if (filefd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Create a socket for the client
    sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET6, SERVER_IP_6, &serv_addr.sin6_addr) <= 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    // Read from the file and send to the server
    while ((nbytes = read(filefd, buffer, sizeof(buffer))) > 0) {
        if (write(sockfd, buffer, nbytes) != nbytes) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    // Close the file and socket
    close(filefd);
    close(sockfd);
    return 0;
}

int ipv6_udp(){
    int sockfd, filefd, nbytes, n_sent;
    struct sockaddr_in6 serv_addr;
    char buffer[BUFFER_SIZE];

    // Open the file for reading
    filefd = open("file.txt", O_RDONLY);
    if (filefd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Create a socket for the client
    sockfd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET6, SERVER_IP_6, &serv_addr.sin6_addr) <= 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    // Read from the file and send to the server
    while ((nbytes = read(filefd, buffer, sizeof(buffer))) > 0) {
        n_sent = sendto(sockfd, buffer, nbytes, 0, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
        if (n_sent < 0) {
            perror("sendto");
            exit(EXIT_FAILURE);
        } else if (n_sent != nbytes) {
            fprintf(stderr, "sendto: Sent %d bytes instead of %d bytes\n", n_sent, nbytes);
        }
    }

    // Close the file and socket
    if (close(filefd) < 0) {
        perror("close");
        exit(EXIT_FAILURE);
    }
    if (close(sockfd) < 0) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int uds_stream() {
    int s, len;
    struct sockaddr_un remote = {
            .sun_family = AF_UNIX,
            .sun_path = SOCK_PATH,
    };
    char buf[1024];

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
            perror("socket");
            exit(1);
    }

    len = strlen(remote.sun_path) + sizeof(remote.sun_family);
    if (connect(s, (struct sockaddr *)&remote, len) == -1) {
            perror("connect");
            exit(1);
    }

    // Open file to be sent
    int fd;
    if ((fd = open("file.txt", O_RDONLY)) < 0) {
            perror("open");
            exit(1);
    }

    // Get file size
    struct stat st;
    if (fstat(fd, &st) < 0) {
            perror("fstat");
            exit(1);
    }
    int file_size = st.st_size;

    // Send file size to server
    if (send(s, &file_size, sizeof(file_size), 0) < 0) {
            perror("send");
            exit(1);
    }

    // Send file data to server
    int total = 0, n;
    while (total < file_size) {
            n = read(fd, buf, sizeof(buf));
            if (n < 0) {
                    perror("read");
                    exit(1);
            }

            if (send(s, buf, n, 0) < 0) {
                    perror("send");
                    exit(1);
            }

            total += n;
    }

    close(fd);
    close(s);
    return 0;
}

int uds_dgram() {
    int s, len;
    struct sockaddr_un remote = {
        .sun_family = AF_UNIX,
        .sun_path = SOCK_PATH
    };

    if ((s = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // Open the file to send
    int fd;
    if ((fd = open("file.txt", O_RDONLY)) < 0) {
        perror("open");
        exit(1);
    }

    // Get file size
    off_t file_size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    // Send file size to server
    if (sendto(s, &file_size, sizeof(file_size), 0, (struct sockaddr *)&remote, sizeof(remote)) < 0) {
        perror("sendto");
        exit(1);
    }

    // Send file data to server
    char buf[1024];
    int nread;
    while ((nread = read(fd, buf, sizeof(buf))) > 0) {
        if (sendto(s, buf, nread, 0, (struct sockaddr *)&remote, sizeof(remote)) < 0) {
            perror("sendto");
            exit(1);
        }
    }

    close(fd);
    close(s);

    printf("File sent.\n");

    return 0;
}

int mmap_filename(){
    int sockfd, n;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];
    const char *filename = "file.txt";
    int fd;
    struct stat filestat;
    char *filedata;

    // Open file
    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        error("Error opening file.");
    }

    // Get file size
    if (fstat(fd, &filestat) < 0) {
        error("Error getting file size.");
    }

    // Map the file into memory
    filedata = mmap(0, filestat.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (filedata == MAP_FAILED) {
        error("Error mapping file into memory.");
    }

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        error("Error opening socket.");
    }

    // Set server address
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Send file data to server
    n = sendto(sockfd, filedata, filestat.st_size, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (n < 0) {
        error("Error sending data.");
    }

    // Close file and socket
    close(fd);
    close(sockfd);

    // Unmap the file from memory
    if (munmap(filedata, filestat.st_size) < 0) {
        error("Error unmapping file.");
    }

    return 0;
}



int pipe_filename() {
    int fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    FILE* file;

    // Open the file to be sent
    file = fopen("file.txt", "r");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Open the named pipe for writing
    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("Failed to open named pipe");
        exit(EXIT_FAILURE);
    }

    // Read data from the file and write it to the named pipe
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        write(fd, buffer, bytes_read);
    }

    // Close the file and named pipe
    fclose(file);
    close(fd);

    printf("File sent successfully.\n");

    return 0;

}

void send_file(char* type, char* param){
    if(strcmp(type,"ipv4") == 0 && (strcmp(param, "tcp"))== 0){
        ipv4_tcp();
    }
    else if(strcmp(type ,"ipv4") == 0 && (strcmp(param , "udp")) == 0){
        ipv4_udp();
}
else if(strcmp(type ,"ipv6") == 0 && (strcmp(param , "tcp")) == 0){
   
    ipv6_tcp();
}
else if(strcmp(type, "ipv6") == 0 && (strcmp(param, "udp")) == 0){

   ipv6_udp();
}

else if(strcmp(type, "mmap") == 0 && (strcmp(param, "file")) == 0){
    
    mmap_filename();
}
else if(strcmp(type ,"pipe") == 0 && (strcmp(param , "file")) == 0){

    pipe_filename();
}
else if(strcmp(type ,"uds") == 0 && (strcmp(param ,"dgram")) == 0){
   
    uds_dgram();
}
else if(strcmp(type ,"uds") == 0 && (strcmp(param, "stream")) == 0){
    
    uds_stream();
}
}

int main(int argc, char *argv[]) {
    if (argc < 7)
        error("Usage: stnc -c IP PORT -p <type> <param>");
    
    // Set up socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Set up server address
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8081);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Server IP address

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connecting to server");
        exit(1);
    }

    // Send data over socket
    char type[1024], param[1024];
    strcpy(type, argv[5]);
    strcpy(param, argv[6]);
    printf("message1: %s\n", type);
    printf("message2: %s\n", param);

    char data[2048];
    sprintf(data, "%s %s", type, param);
    if (send(sockfd, data, strlen(data), 0) < 0) {
        perror("Error sending data");
        exit(1);
    }

    // Close socket
    close(sockfd);
    sleep(1);

    send_file(type , param);
    return 0;
}
