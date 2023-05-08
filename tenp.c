
int main(int argc, char *argv[]) {
    if (argc < 2)
        error("Usage: stnc -s PORT (server) or stnc -c IP PORT (client)");

    if (strcmp(argv[1], "-s") == 0) {
        if (argc < 3)
            error("ERROR no port provided for server");
        int port = atoi(argv[2]);
        run_server(port); 
    } else if (strcmp(argv[1], "-c") == 0) {
        if (argc < 4)
            error("ERROR no IP address or port provided for client");
        char *ip = argv[2];
        int port = atoi(argv[3]);
        run_client(ip, port);
    } else {
        error("ERROR invalid arguments");
    }
    return 0;
}







int main(int argc, char *argv[]) {
if (argc < 2) {
printf("Usage: stnc [options]\n");
printf("For help, use stnc -h\n");
exit(0);
}
int isServer = 0;
int isClient = 0;
int quietMode = 0;
int performTest = 0;
char *address = NULL;
char *port = NULL;
char *type = NULL;
char *param = NULL;

int opt;
while ((opt = getopt(argc, argv, "sqhsc:p:")) != -1) {
    switch (opt) {
        case 's':
            isServer = 1;
            break;
        case 'c':
            isClient = 1;
            address = strtok(optarg, ":");
            port = strtok(NULL, ":");
            break;
        case 'p':
            performTest = 1;
            type = strtok(optarg, " ");
            param = strtok(NULL, " ");
            break;
        case 'q':
            quietMode = 1;
            break;
        case 'h':
            printf("Usage:\n");
            printf("Server: stnc -s PORT -p -q\n");
            printf("Client: stnc -c IP:PORT -p <type> <param>\n");
            printf("Options:\n");
            printf("  -s              Start server\n");
            printf("  -c IP:PORT      Connect to server\n");
            printf("  -p TYPE PARAM   Perform performance test with TYPE and PARAM\n");
            printf("  -q              Quiet mode, only show test results\n");
            printf("  -h              Show this help message\n");
            exit(0);
        default:
            printf("Invalid argument. Use stnc -h for help.\n");
            exit(1);
    }
}

if (!isServer && !isClient) {
    printf("Invalid argument. Use stnc -h for help.\n");
    exit(1);
}

if (isServer) {
    if (!port || !performTest) {
        printf("Invalid argument. Use stnc -h for help.\n");
        exit(1);
    }
    server(atoi(port), performTest, quietMode);
} else {
    if (!address || !port || !performTest) {
        printf("Invalid argument. Use stnc -h for help.\n");
        exit(1);
    }
    client(address, atoi(port), type, param, performTest);
}

return 0;
}

void generate_data(char *data, size_t size) {
    for(size_t i = 0; i < size; i++) {
        data[i] = i % 256;
    }
}

unsigned long long calculate_checksum(char *data, size_t size) {
    unsigned long long sum = 0;
    for(size_t i = 0; i < size; i++) {
        sum += (unsigned char) data[i];
    }
    return sum;
}

unsigned long long get_current_time_ms() {
    struct timespec ts;
    if(clock_gettime(CLOCK_MONOTONIC, &ts) == -1) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }
    return (unsigned long long) ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}