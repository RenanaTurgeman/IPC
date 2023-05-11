#include <string.h>
#include "chat.h"
#include "client_b.h"
#include "server_b.h"

int main(int argc, char *argv[])
{
    if(strcmp(argv[1], "-c") == 0)
    {
        if(argc >= 5 && strcmp(argv[4], "-p")==0)
        {
            client_main_test(argc, argv);
        }
        else
        {
            main_chat(argc, argv);
        }
    }
    else if(strcmp(argv[1], "-s") == 0)
    {
        if(argc >= 4 && strcmp(argv[3], "-p")==0)
        {
            server_main_test(argc, argv);
        }
        else
        {
            main_chat(argc, argv);
        }
    }
    return 0;
}
