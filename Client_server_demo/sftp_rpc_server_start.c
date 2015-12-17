#include "socket_header.h"

int sftp_rpc_server_start (int socket_fd)
{
        int ret             =       -1;
        int client_fd       =       0;
        pthread_t pid;
        
        client_fd = accept (socket_fd, (struct sockaddr *) NULL, NULL);
        if(client_fd < 0)
        {
                fprintf (stderr, "\tERROR: %s\n", strerror(errno));
                ret = -1;
                goto out;
        }
        
        pid = pthread_self();
        printf ("Thread Id at initilize %d\n", pid);
        
        ret = client_fd;
        
        out:
            return ret;
}