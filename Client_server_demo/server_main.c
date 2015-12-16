#include "socket_header.h"

int main (int argc, char* argv [])
{
    int socket_fd       =       0;
    int ret             =       -1;
    int client_fd       =       0;
    int port            =       0;
    char * path;
    int next_option;
    pthread_t pid;
    
    struct sockaddr_in client_addr;
    
    /*Command line options*/
    const char* const short_option = "p:d:";

    const struct option long_option [] = {
            { "port",       1, NULL, 'p'},
            { "destination",1, NULL, 'd'},
            { "NULL",       0, NULL,  0 } 
    };
    printf ("Thread Id at main  %d\n", (int)pthread_self ());
    do
    {
        next_option = getopt_long (argc, argv, short_option, long_option, NULL);
        
        switch (next_option)
        {
            case 'p':
                    port = atoi (optarg);
                    break;
            case 'd':
                    path = (char*)malloc(strlen (optarg + 1));
                    strcpy (path, optarg);
                    break;
            case -1:
                    break;
            default:
                    abort ();
        }
    }while (next_option != -1);
    
    printf ("\tPress ctrl+c to exit\n");
        
    /*Creates the server socket*/
    socket_fd = sftp_rpc_server_init (port);
    if (socket_fd < 0)
    {
        ret = -1;
        goto out;
    }
    
    while (1)
    {
        /*Accept the incomming client connection*/
        client_fd = sftp_rpc_server_start (socket_fd);
        if (client_fd < 0)
        {
            ret = -1;
            goto out;
        }
        printf ("\tClient Connected\n");
        
        /*Giving the service to client*/
        ret = server_response (client_fd, path);
    }
    
    ret = 0;
    out:
        close (socket_fd);
        return ret;
}

