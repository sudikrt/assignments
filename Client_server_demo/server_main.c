#include "queue.h"

/* This function will initialize the mutex and condition variables*/
void initialize_flag ()
{
        /* Initializing the mutex and condition variable.*/
        pthread_mutex_init (&lock, NULL);
        pthread_mutex_init (&thread_flag_mutex, NULL);
        pthread_cond_init (&thread_flag_cv, NULL);
        /* Initializing the flag value. */
        thread_flag = 0;
}

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
        int next_option;
        pthread_t pid, t_id;

        struct sockaddr_in client_addr;

        /*Declare and initialze the Head of the queue*/
        head = (C_Queue*) malloc (sizeof(C_Queue));
        head->next = head;

        /* Initialize the condition and mutex variables*/
        initialize_flag ();
        
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

        /*Thread to servrice the request the each client_request*/
        pthread_create (&t_id, NULL, &delete_queue, NULL);

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
                server_response (client_fd);
        }
        
        ret = 0;
        out:
            close (socket_fd);
            return ret;
}

