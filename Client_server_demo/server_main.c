#include "socket_header.h"
#include <errno.h>

extern int errno;

int main (int argc, char* argv [])
{
    int socket_fd       =       0;
    int ret             =       -1;
    int client_fd       =       0;
    int port            =       0;
    char * path;
    int next_option;
    
    struct sockaddr_in server_addr, client_addr;
    /*Command line options*/
    const char* const short_option = "p:d:";

    const struct option long_option [] = {
            { "port",       1, NULL, 'p'},
            { "destination",1, NULL, 'd'},
            { "NULL",       0, NULL,  0 } 
    };
    
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
    
    
    /*Creating the socket*/
    socket_fd = socket (PF_INET, SOCK_STREAM, 0);
    printf ("\tInitilizing\n");
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl (INADDR_ANY);
    server_addr.sin_port = htons (port);
    
    if (bind (socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        fprintf (stderr, "\t%s\n", strerror(errno));
        ret = -1;
        goto out;
    }
    /*Listen for the incomming connection*/
    if (listen (socket_fd, 5) == -1)
    {
        fprintf (stderr, "\t%s\n", strerror(errno));
        ret = -1;
        goto out;
    }
    while (1)
    {
        /*Accept the client connection*/
        client_fd = accept (socket_fd, (struct sockaddr *) NULL, NULL);
        if(client_fd < 0)
        {
            fprintf (stderr, "\t%s\n", strerror(errno));
            ret = -1;
            goto out;
        }
        printf ("\tClient Connected\n");
        
        /*Giving the service to client*/
        ret = server_response (client_fd, path);
    }
    ret = 0;
    out:
        return ret;
}

