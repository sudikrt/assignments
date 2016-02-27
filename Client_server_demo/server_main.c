#include "socket_header.h"

/* This is the main fuction where server starts and initializes the socket
 * for listening the client.
 * Input:
 *      inputs are specied through commnad line arguments in which
 *      -p      :       specifies the port_no
 *      -d      :       specifies the export directory
 * 
 * Output:
 *      int     : return value:
 */
int main (int argc, char* argv [])
{
        int socket_fd       =       0;
        int ret             =       -1;
        int client_fd       =       0;
        int port            =       0;
        int max_thread      =       3;
        int next_option;
        pthread_t pid, t_id;

        struct sockaddr_in client_addr;
        C_Request* c_req;

        /* Get the linked list and initialize it */
        fd_node = NULL;

        /*Command line options*/
        const char* const short_option = "p:d:";

        const struct option long_option [] = {
                { "port",       1, NULL, 'p'},
                { "destination",1, NULL, 'd'},
                { "NULL",       0, NULL,  0 } 
        };

        do
        {
                next_option = getopt_long (argc, argv, short_option, 
                                                            long_option, NULL);

                switch (next_option)
                {
                        case 'p':
                                port = atoi (optarg);
                                break;
                        case 'd':
                                exp_dir = (char*)malloc(strlen (optarg + 1));
                                strcpy (exp_dir, optarg);
                                break;
                        case -1:
                                break;
                        default:
                                abort ();
                }
        }while (next_option != -1);

        printf ("\tPress ctrl+c to exit\n");

        /*Creates the server socket*/
        pthread_create (&pid, NULL, &sftp_rpc_server_init, &port);
        pthread_join (pid, (void*) &socket_fd);

        if (socket_fd < 0)
        {
                ret = -1;
                goto out;
        }

        /* Create the thread_pool and initialize the list */
        pthread_create (&t_id, NULL, &create_thread_pool, &max_thread);
        pthread_join (t_id, (void*) &pool);
        
        while (1)
        {
                /*Accept the incomming client connection*/
                client_fd = sftp_rpc_server_start (socket_fd);
                    
                if (client_fd < 0)
                {
                        ret = -1;
                        goto out;
                }
                printf ("\tClient : %d \t had connected\n", client_fd);

                /*Giving the service to client*/
                c_req = (C_Request*) calloc (1, sizeof (C_Request));

                server_response (client_fd, pool, c_req);
        }

        ret = 0;
        out:
            close (socket_fd);
            return ret;
}

