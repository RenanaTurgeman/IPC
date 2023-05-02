#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 1234
#define FILENAME "file.txt"

int ipv4_tcp() {
    int sockfd, filefd, len;
    struct sockaddr_in servaddr;
    struct timeval start, end;
    char buffer[1024];
    struct stat st;

    // Get file size
    stat(FILENAME, &st);
    int file_size = st.st_size;

    // Open file
    filefd = open(FILENAME, O_RDONLY);
    if (filefd < 0) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set server address
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    servaddr.sin_port = htons(SERVER_PORT);

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    // Send file
    gettimeofday(&start, NULL);
    while ((len = read(filefd, buffer, sizeof(buffer))) > 0) {
        write(sockfd, buffer, len);
    }
    gettimeofday(&end, NULL);

    // Close file and socket
    close(filefd);
    close(sockfd);

    // Calculate time taken to send file
    double time_taken = ((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec)) / 1000.0;

    // Print time taken
    printf("File sent in %f milliseconds\n", time_taken);

    return 0;
}

int main(){
    ipv4_tcp();
}