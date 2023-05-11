#include <string.h>
#include "chat.h"
#include "client_b.h"
#include "server_b.h"

#define FILE_SIZE 100000000 // 100MB file size in bytes
#define STR_SIZE 15 // length of the string "RENANA_AND_TAMAR"
int create_file(){
    FILE* fp;
    char* buffer = malloc(FILE_SIZE);
    if (buffer == NULL) {
        perror("Failed to allocate memory for buffer");
        return EXIT_FAILURE;
    }
    memset(buffer, 0, FILE_SIZE);
    int i;
    for (i = 0; i < FILE_SIZE; i += STR_SIZE) {
        memcpy(&buffer[i], "RENANA_AND_TAMAR", STR_SIZE);
    }
    fp = fopen("file.txt", "w");
    if (fp == NULL) {
        perror("Failed to create file");
        return EXIT_FAILURE;
    }
    if (fwrite(buffer, sizeof(char), FILE_SIZE, fp) != FILE_SIZE) {
        perror("Failed to write to file");
        return EXIT_FAILURE;
    }
    fclose(fp);
    free(buffer);
    return EXIT_SUCCESS;
}
int main(int argc, char *argv[])
{
    int q=0;
    if(strcmp(argv[4], "-q") == 0){ //quit mode
        q=1;
    }
    if(strcmp(argv[1], "-c") == 0)
    {
        if(argc >= 5 && strcmp(argv[4], "-p")==0)
        {
            client_main_test(argc, argv,q);
        }
        else
        {
            main_chat(argc, argv);
        }
    }
    else if(strcmp(argv[1], "-s") == 0)
    {
        create_file(); //we run the server first so when we enter the server create the "file.txt"

        if(argc >= 5 && strcmp(argv[3], "-p")==0)
        {
            server_main_test(argc, argv,q);
        }
        else
        {
            main_chat(argc, argv);
        }
    }
    return 0;
}
