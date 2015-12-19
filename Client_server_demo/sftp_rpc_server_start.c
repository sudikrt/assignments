#include "socket_header.h"

void sftp_rpc_server_start (int socket_fd)
{
        int ret             =       -1;
        int client_fd       =       0;
        //int socket_fd       =       *((int*) sock_fd);

        client_fd = accept (socket_fd, (struct sockaddr *) NULL, NULL);

        if(client_fd < 0)
        {
                fprintf (stderr, "\tERROR: %s\n", strerror(errno));
                ret = -1;
                goto out;
        }

        ret = client_fd;

        out:
            return ret;
}