int client_main_test(int argc, char *argv[]) {
    if (argc < 7)
        error_c("Usage: stnc -c IP PORT -p <type> <param>");
    
    // Set up socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }
    printf("open sock");

    char ip_address[1024];
    int port;
    strcpy(ip_address, argv[2]);

    printf("IP adress: %s\n", ip_address);
    port = atoi(argv[3]);
    printf("Port: %d\n", port);

    // Set up server address
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8081);
    serv_addr.sin_addr.s_addr = inet_addr(ip_address);  // Server IP address

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connecting to server");
        exit(1);
    }
    printf("conect");

    // Send data over socket
    char type[1024], param[1024];
    strcpy(type, argv[5]);
    strcpy(param, argv[6]);
    printf("type: %s\n", type);
    printf("param: %s\n", param);

    char data[2048];
    sprintf(data, "%s %s", type, param);
    if (send(sockfd, data, strlen(data), 0) < 0) {
        perror("Error sending data");
        exit(1);
    }

    // Close socket
    close(sockfd);
    sleep(1);

    send_file(type , param , ip_address , port);
    return 0;
}

int server_main_test(int argc, char *argv[])
{
    if (argc < 5)
        error("Usage: stnc -s port -p (p for performance test) -q (q for quiet)");
    int port = atoi(argv[2]);
    // printf("Port: %d\n", port);
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
    printf("create socket");

    // Attach socket to the port 8081
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    printf("after setsockopt");
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8081);

    // Bind the socket to the specified port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("after bind");

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
    // printf("%s\n", buffer); //print which kind

    // Parse data into parameters
     type = strtok(buffer, " ");
     param = strtok(NULL, " ");

    printf("Type: %s\n", type);
    printf("Param: %s\n", param);

    // received_file(type , param, port);
    }


    // Close the socket
    close(new_socket);
    close(server_fd);
    
    received_file(type , param, port);
    return 0;
}
