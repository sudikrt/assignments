#include "socket_header.h"

/* Function to get the length of the file_request type request. */
ssize_t calc_file_request_length (char* file_name) {
        ssize_t len     =       0;
        
        /* Length of request_type */
        len = sizeof (int);
        
        /* Length of Number of arguments */
        len += sizeof (int);
        
        /* Length of argument. */
        len += sizeof (int);
        
        /* Length of the file_name */
        len += strlen (file_name);
        
        /* Length of footer */
        len += sizeof (int);

out:
        return len;
}

/*
 * Client requesting the server for the file
 * Input:
 *      int     :       specifies the connection identifier.
 * Output:
 *      int     :       Specifies the success or failure.
 * */
int request (int port, char* server_name)
{
        int ret         =       -1;
        int len         =       0;
        int success      =       0;
        int exit_var    =       0;
        int type        =       0;
        int value       =       0;
        char* data;
        char* extra;
        char* buffer;
        char * file_name;
        int client_fd;

        /* Allocate the memory. */
        file_name = (char*) calloc (1, 500);
        data = (char*) calloc (1, 100);
        extra = (char*) calloc (1, 100); 
        buffer = (char*) calloc (1, 1024);
        
        while (1)
        {
        menu:
                printf ("\tPress \n\t1. Read\n \t2. Read in chenks \n \t3. Quit\n \t");
                scanf ("%d", &value);
                if (value > 3 || value < 1)
                {
                        printf ("\tEnter the proper option try again. \n");
                        goto menu;
                }
                switch (value)
                {
                        case 1:
                                exit_var = 0;
                                client_fd = sftp_rpc_client (port, server_name);
                                if(client_fd < 0)
                                {
                                        printf ("\tConnection failure re run\n");
                                        goto out;
                                }
                                printf ("\tConnection Success!!\n");

                                /* Clear the buffer. */
                                memset (buffer, 0, sizeof(buffer));

                                printf ("\tEnter the file name:");
                                scanf ("%s", file_name);

                                /* Calculate the length of the request. */
                                len = calc_file_request_length (file_name);
                                
                                /* Allocate the buffer based on the request 
                                 * length. */
                                buffer = (char*) calloc (1, len + sizeof (int));

                                /* Total length of the request. */
                                memcpy (buffer, &len, sizeof (int));
                                exit_var += sizeof (int);

                                /* Type of the request */
                                type = file_request;
                                memcpy (buffer + exit_var, &type, sizeof (int));
                                exit_var += sizeof (int);
                                
                                /* Number of arguments */
                                type = 1;
                                memcpy (buffer + exit_var, &type, sizeof (int));
                                exit_var += sizeof (int);
                                
                                /* Length of the arguments */
                                type = strlen (file_name);
                                memcpy (buffer + exit_var, &type, sizeof (int));
                                exit_var += sizeof (int);
                                
                                /* The argument */
                                memcpy (buffer + exit_var, file_name,
                                                        strlen(file_name));
                                exit_var += strlen (file_name);
                                
                                /* Footer */
                                type = 0xBAADF00D;
                                memcpy (buffer + exit_var, &type, sizeof (int));
                                exit_var += sizeof (int);

                                /* Send the request to server */
                                ret = write (client_fd, buffer, exit_var);
                                printf ("Sent bytes %d\n", ret);
                                if (ret < 0)
                                {
                                        printf ("\tSome error ocurred\n");
                                        goto menu;
                                }
                                get_data_from_server (client_fd, file_name);
                                close (client_fd);
                                break;
                        case 2:
                                client_fd = sftp_rpc_client (port, server_name);
                                if(client_fd < 0)
                                {
                                        printf ("\tConnection failure re run\n");
                                        goto out;
                                }
                                printf ("\tConnection Success!!\n");
                                
                                printf ("\tEnter the file name :");
                                file_name = (char*) calloc (1, 1024);
                                scanf ("%s", file_name);
                                
read_choice:                    printf ("\tEnter the Mode you want to open :\n");
                                printf ("1 for read_mode, 2 for write_mode :\n");
                                scanf ("%d", &choice);
                                if (choice != 1 || choice != 2) {
                                        goto read_choice;
                                }
                                
                                /* Calculate the length of the request */
                                len = calc_file_open_request_len (file_name);
                                
                                 /* Allocate the buffer based on the request 
                                 * length. */
                                buffer = (char*) calloc (1, len + sizeof (int));

                                /* Total length of the request. */
                                memcpy (buffer, &len, sizeof (int));
                                exit_var += sizeof (int);

                                /* Type of the request */
                                type = open_file_request;
                                memcpy (buffer + exit_var, &type, sizeof (int));
                                exit_var += sizeof (int);
                                
                                /* Number of arguments */
                                type = 1;
                                memcpy (buffer + exit_var, &type, sizeof (int));
                                exit_var += sizeof (int);
                                
                                /* Length of the arguments */
                                type = strlen (file_name);
                                memcpy (buffer + exit_var, &type, sizeof (int));
                                exit_var += sizeof (int);
                                
                                /* The argument */
                                memcpy (buffer + exit_var, file_name,
                                                        strlen(file_name));
                                exit_var += strlen (file_name);
                                
                                /* Footer */
                                type = 0xBAADF00D;
                                memcpy (buffer + exit_var, &type, sizeof (int));
                                exit_var += sizeof (int);
                                
                                //handle_file_operation (client_fd);
                                break;
                        case 3:
                                ret = 0;
                                goto out;
                        default:
                                ret = 0;
                                goto out;
                }

        }
        ret = 0;

        out:
                close (client_fd);
                free (buffer);
                free (extra);
                free (data);
                return ret;
}
