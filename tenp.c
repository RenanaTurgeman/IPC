int ipv6_udp() {
    int sockfd, filefd, nbytes;
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

    while (1) {
        // Receive data from a client and write it to a file
        cli_len = sizeof(cli_addr);
        if ((nbytes = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &cli_addr, &cli_len)) < 0) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }

        // Open a file for writing
        filefd = open("received_file.txt", O_CREAT | O_WRONLY | O_APPEND, 0644);
        if (filefd < 0) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // Write data to the file
        if (write(filefd, buffer, nbytes) != nbytes) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        // Close the file
        close(filefd);

        // Send a response back to the client
        if (sendto(sockfd, "OK", 2, 0, (struct sockaddr *) &cli_addr, sizeof(cli_addr)) < 0) {
            perror("sendto");
            exit(EXIT_FAILURE);
        }
    }

    // Close the socket (this line will never be executed)
    close(sockfd);

    return 0;
}
