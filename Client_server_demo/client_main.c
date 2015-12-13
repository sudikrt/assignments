#include "socket_header.h"

extern int errno;

int main (int argc, char * argv [])
{
    int socket_fd       =       0;
    int ret             =       -1;
    int port            =       0;
    char * server_name;
    char * destination;
    char * file_name;
    int next_option;
    
    struct sockaddr_in server_addr;
    /*Command line options*/
    const char* const short_option = "s:p:f:d:";

    const struct option long_option [] = {
            { "server",     1, NULL, 's'},
            { "port",       1, NULL, 'p'},
            { "file",       1, NULL, 'f'},
            { "destination",1, NULL, 'd'},
            { "NULL",       0, NULL,  0 } 
    };
    
    do
    {
        next_option = getopt_long (argc, argv, short_option, long_option, NULL);
        
        switch (next_option)
        {
            case 'f':
                    file_name = (char*) malloc (strlen(optarg + 1));
                    strcpy (file_name, optarg);
                    break;
            case 'p':
                    port = atoi (optarg);
                    break;
            case 's':
                    server_name = (char*) malloc (strlen (optarg + 1));
                    strcpy (server_name, optarg);
                    break;
            case 'd':
                    destination = (char*) malloc (strlen (optarg + 1));
                    strcpy (destination, optarg);
                    break;
            case -1:
                    break;
            default:
                    abort ();
        }
    }while (next_option != -1);
    
    /*Creates the socket*/
    socket_fd = socket (AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        fprintf (stderr, "\t%s\n", strerror(errno));
        ret = -1;
        goto out;
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons (port);
    server_addr.sin_addr.s_addr = inet_addr (server_name);
    
    /*Connects to server or host*/
    if(connect (socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        fprintf (stderr, "\t%s\n", strerror(errno));
        ret = -1;
        goto out;
    }
    printf ("\tConnection Success!!\n");

    /*Client request for the particular named file*/
    ret = request (socket_fd, file_name, destination);
    
    out:
        return ret;
    
}
