if (argc < 7)
{
//fprintf("error");
exit(1);
}

char message[2][1024];
strcpy(message[0], argv[5]);  // Replace "IPv4 Address" with the actual IPv4 address
strcpy(message[1], argv[6]);


printf("%s,%s\n" ,argv[5], argv[6] );
printf("%s,%s\n" ,message[0], message[1] );


//--------------sending choise to server-------------

int client_fd;
struct sockaddr_in server_addr;


// Create a socket
if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
}

// Prepare the server address structure
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Server IP address
server_addr.sin_port = htons(8000);

// Connect to the server
if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
    perror("connect");
    exit(EXIT_FAILURE);
}


printf("message1: %s\n", message[0]);
printf("message2: %s\n", message[1]);

// Send data to the server
if (send(client_fd, message[0], strlen(message[0]), 0) == -1) {
    perror("send");
    exit(EXIT_FAILURE);
}
sleep(1);
if (send(client_fd, message[1], strlen(message[1]), 0) == -1) {
    perror("send");
    exit(EXIT_FAILURE);
}


// Close the connection
close(client_fd);



sleep(1);
//------------------------------------------------


if(strcmp(argv[5],"ipv4") == 0 && (strcmp(argv[6], "tcp"))== 0){
    printf("client open\n");
    c_ipv4_tcp();
}
else if(strcmp(argv[5] ,"ipv4") == 0 && (strcmp(argv[6] , "udp")) == 0){
    
    c_ipv4_udp();
}
else if(strcmp(argv[5] ,"ipv6") == 0 && (strcmp(argv[6] , "tcp")) == 0){
   
    c_ipv6_tcp();
}
else if(strcmp(argv[5], "ipv6") == 0 && (strcmp(argv[6], "udp")) == 0){

    c_ipv6_udp();
}

else if(strcmp(argv[5], "mmap") == 0 && (strcmp(argv[6], "file")) == 0){
    
    c_mmap();
}
else if(strcmp(argv[5] ,"pipe") == 0 && (strcmp(argv[6] , "file")) == 0){

    c_pipe();
}
else if(strcmp(argv[5] ,"uds") == 0 && (strcmp(argv[6] ,"dgram")) == 0){
   
    c_uds_dgram();
}
else if(strcmp(argv[5] ,"uds") == 0 && (strcmp(argv[6], "stream")) == 0){
    
    c_uds_stream();
}








    return 0;
}