#include "socket_header.h"

/*
 * This function accept the incoming client connection
 * Input:
 *      int socket_fd   :       Specifies the connection identifer of server 
 *                              socket.
 * Output:
 *      int             :       On success client_fd is returned 
 *                              else the connection unsuccessful.
 * */
int sftp_rpc_server_start (int socket_fd)
{
        int ret             =       -1;
        int client_fd       =       0;

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