#include "socket_header.h"

int s_ftp_rpc_client (int port, char * server_name)
{
    int ret             =       -1;
    int socket_fd       =       0;
    struct sockaddr_in server_addr;
    
    /*Creates the socket*/
    socket_fd = socket (AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        fprintf (stderr, "\tERROR: %s\n", strerror(errno));
        ret = -1;
        goto out;
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons (port);
    server_addr.sin_addr.s_addr = inet_addr (server_name);
    
    /*Connects to server or host*/
    if(connect (socket_fd, (struct sockaddr *) &server_addr, 
                                                    sizeof(server_addr)) < 0)
    {
        fprintf (stderr, "\tERROR: %s\n", strerror(errno));
        ret = -1;
        goto out;
    }
    
    ret = socket_fd;
    
    out:
        return ret;
}