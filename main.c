


int main(int argc, char *argv[])
{
    if(strcmp(argv[1], "-c") == 0)
    {
        if(argc >= 5 && strcmp(argv[4], "-p"))
        {
            client_main_test(argc, argv);
        }
        else
        {
            main_chat(argc, argv);
        }
    }
    else(strcmp(argv[1], "-s") == 0)
    {
        if(argc >= 5 && strcmp(argv[3], "-p"))
        {
            server_main_test(argc, argv);
        }
        else
        {
            main_chat(argc, argv);
        }
    }

}