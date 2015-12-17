#include "socket_header.h"
#define BACK_LOG 5

int sftp_rpc_server_init (int port)
{
        int ret             =       -1;
        int socket_fd       =       0;
        
        struct sockaddr_in server_addr;
        
        /*Creating the socket*/
        socket_fd = socket (PF_INET, SOCK_STREAM, 0);
        if (socket_fd < 0)
        {
                fprintf (stderr, "\tERROR: %s\n", strerror(errno));
                ret = -1;
                goto out;
        }
        printf ("\tInitilizing\n");
        
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl (INADDR_ANY);
        server_addr.sin_port = htons (port);
        
        if (bind (socket_fd, (struct sockaddr *) &server_addr,
                                                    sizeof(server_addr)) < 0)
        {
                fprintf (stderr, "\tERROR: %s\n", strerror(errno));
                ret = -1;
                goto out;
        }
        /*Listen for the incomming connection*/
        if (listen (socket_fd, BACK_LOG) == -1)
        {
                fprintf (stderr, "\tERROR: %s\n", strerror(errno));
                ret = -1;
                goto out;
        }
        
        ret = socket_fd;
        
        out:
            return ret;
}