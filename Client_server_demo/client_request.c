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

/* Function to get the length of the open file request */
ssize_t
calc_file_open_request_len (char* file_name) {
        
        ssize_t len     =       0;
        
        /* Length of request_type */
        len = sizeof (int);
        
        /* Length of Number of arguments */
        len += sizeof (int);
        
        /* Length of argument. */
        len += sizeof (int);
        
        /* Length of the file_name */
        len += strlen (file_name);
        
        /* Length of the second argument */
        len += sizeof (int);
        
        /* The argument itself */
        len += sizeof (int);
        
        /* Length of footer */
        len += sizeof (int);
}

/* 
 * Function to find the length of the close file request as well as 
 * read bytes in chunks.
 */
ssize_t
calc_file_close_request_len () {
        ssize_t len;
        
        /* Length of request_type */
        len = sizeof (int);
        
        /* Length of Number of arguments */
        len += sizeof (int);
        
        /* Length of argument. */
        len += sizeof (int);
        
        /* Length of the file_name */
        len += sizeof (int);
        
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
        int choice      =       0;
        int num_bytes   =       0;

        char* buffer;
        char * file_name;
        int client_fd;

        /* Allocate the memory. */
        file_name = (char*) calloc (1, 500);
        buffer = (char*) calloc (1, 1024);
        
        while (1)
        {
        menu:
                printf ("\tPress \n\t1. Read\n \t2. Open a file \n ");
                printf ("\t3. Close a file\n \t4. Read in chunks\n ");
                printf ("\t5. Exit\n");
                
                scanf ("%d", &value);
                if (value > 5 || value < 1)
                {
                        printf ("\tEnter the proper option try again. \n");
                        goto menu;
                }
                switch (value)
                {
                        case 1:
                                exit_var = 0;
                                
                                /* Initiates the connection */
                                client_fd = sftp_rpc_client (port, server_name);
                                if(client_fd < 0)
                                {
                                        printf ("\tConnection failure re run\n");
                                        goto out;
                                }
                                printf ("\tConnection Success!!\n");

                                printf ("\tEnter the file name:");
                                scanf ("%s", file_name);

                                /* Calculate the length of the request. */
                                len = calc_file_request_length (file_name);
                                
                                /* Allocate the buffer based on the request 
                                 * length. */
                                buffer = (char*) calloc (1, len + sizeof (int));

                                 /* Clear the buffer. */
                                memset (buffer, 0, len + sizeof (int));

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
                                printf ("\tSent bytes %d\n", ret);
                                if (ret < 0)
                                {
                                        printf ("\tSome error ocurred\n");
                                        goto menu;
                                }
                                
                                /* Process the client_request*/
                                get_data_from_server (client_fd, file_name);
                                
                                /* Clears the buffer */
                                free (buffer);
                                
                                /* Close the connection */
                                close (client_fd);
                                break;
                        case 2:
                                exit_var = 0;
                                
                                /* Initiates the connection */
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
                                printf ("\t1 for read_mode, 2 for write_mode :\n");
                                scanf ("%d", &choice);
                                if (choice != 1 && choice != 2) {
                                        goto read_choice;
                                }
                                
                                /* Calculate the length of the request */
                                len = calc_file_open_request_len (file_name);
                                
                                 /* Allocate the buffer based on the request 
                                 * length. */
                                buffer = (char*) calloc (1, len + sizeof (int));
                                
                                /* Clear the buffer. */
                                memset (buffer, 0, sizeof(buffer));

                                /* Total length of the request. */
                                memcpy (buffer, &len, sizeof (int));
                                exit_var += sizeof (int);

                                /* Type of the request */
                                type = open_file_request;
                                memcpy (buffer + exit_var, &type, sizeof (int));
                                exit_var += sizeof (int);
                                
                                /* Number of arguments */
                                type = 2;
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
                                
                                /* Length of the arguments */
                                type = sizeof (int);
                                memcpy (buffer + exit_var, &type, sizeof (int));
                                exit_var += sizeof (int);
                                
                                /* The argument */
                                memcpy (buffer + exit_var, &choice, sizeof (int));
                                exit_var += sizeof (int);
                                
                                /* Footer */
                                type = 0xBAADF00D;
                                memcpy (buffer + exit_var, &type, sizeof (int));
                                exit_var += sizeof (int);
                                
                                /* Send the request to server */
                                ret = write (client_fd, buffer, exit_var);
                                printf ("\tSent bytes %d\n", ret);
                                if (ret < 0)
                                {
                                        printf ("\tSome error ocurred\n");
                                        goto menu;
                                }
                                /* Handle the open file_request */
                                handle_open_file_request (client_fd);
                                
                                /* de allocate the buffer */
                                free (buffer);
                                
                                /* Closes the connection */
                                close (client_fd);
                                break;
                        case 3:
                                exit_var = 0;
                                
                                /* Initiates the connection */
                                client_fd = sftp_rpc_client (port, server_name);
                                if(client_fd < 0)
                                {
                                        printf ("\tConnection failure re run\n");
                                        goto out;
                                }
                                printf ("\tConnection Success!!\n");
                                
                                printf ("\t Enter the file descriptor to close :");
                                scanf ("%d", &choice);
                                
                                /* Calculate the length of the request */
                                len = calc_file_close_request_len ();
                                
                                 /* Allocate the buffer based on the request 
                                 * length. */
                                buffer = (char*) calloc (1, len + sizeof (int));
                                
                                /* Clear the buffer. */
                                memset (buffer, 0, sizeof(buffer));

                                /* Total length of the request. */
                                memcpy (buffer, &len, sizeof (int));
                                exit_var += sizeof (int);
                                
                                /* Type of the request */
                                type = close_file_request;
                                memcpy (buffer + exit_var, &type, sizeof (int));
                                exit_var += sizeof (int);
                                
                                /* Number of arguments */
                                type = 1;
                                memcpy (buffer + exit_var, &type, sizeof (int));
                                exit_var += sizeof (int);
                                
                                /* The arguments */
                                memcpy (buffer + exit_var, &choice, sizeof (int));
                                exit_var += sizeof (int);
                                
                                /* Footer */
                                type = 0xBAADF00D;
                                memcpy (buffer + exit_var, &type, sizeof (int));
                                exit_var += sizeof (int);
                                
                                /* Send the request to server */
                                ret = write (client_fd, buffer, exit_var);
                                
                                printf ("\tSent bytes %d\n", ret);
                                if (ret < 0)
                                {
                                        printf ("\tSome error ocurred\n");
                                        goto menu;
                                }
                                
                                /* Handle the close operation of the file */
                                handle_close_file_request (client_fd);
                                
                                /* Closes the connection */
                                close (client_fd);
                                break;
                        case 4:
                                exit_var = 0;
                                
                                /* Initiates the connection */
                                client_fd = sftp_rpc_client (port, server_name);
                                if(client_fd < 0)
                                {
                                        printf ("\tConnection failure re run\n");
                                        goto out;
                                }
                                printf ("\tConnection Success!!\n");
                                
                                printf ("\tEnter the file descriptor to read :");
                                scanf ("%d", &choice);
                                
                                printf ("\tEnter number of bytes to read :");
                                scanf ("%d", &num_bytes);
                                
                                /* Calculate the length of the request */
                                len = calc_file_close_request_len ();
                                
                                 /* Allocate the buffer based on the request 
                                 * length. */
                                buffer = (char*) calloc (1, len + sizeof (int));
                                
                                /* Clear the buffer. */
                                memset (buffer, 0, sizeof(buffer));

                                /* Total length of the request. */
                                memcpy (buffer, &len, sizeof (int));
                                exit_var += sizeof (int);
                                
                                /* Type of the request */
                                type = read_in_chunks;
                                memcpy (buffer + exit_var, &type, sizeof (int));
                                exit_var += sizeof (int);
                                
                                /* Number of arguments */
                                type = 2;
                                memcpy (buffer + exit_var, &type, sizeof (int));
                                exit_var += sizeof (int);
                                
                                /* 1st arguments */
                                memcpy (buffer + exit_var, &choice, sizeof (int));
                                exit_var += sizeof (int);

                                /* 2nd argument */
                                memcpy (buffer + exit_var, &num_bytes, sizeof (int));
                                exit_var += sizeof (int);

                                /* Footer */
                                type = 0xBAADF00D;
                                memcpy (buffer + exit_var, &type, sizeof (int));
                                exit_var += sizeof (int);
                                
                                /* Send the request to server */
                                ret = write (client_fd, buffer, exit_var);
                                
                                printf ("\nSent bytes %d\n", ret);
                                if (ret < 0)
                                {
                                        printf ("\tSome error ocurred\n");
                                        goto menu;
                                }
                                
                                /* handle the reading chunk request */
                                handle_read_chunk_request (client_fd);
                                
                                /* Closes the connection */
                                close (client_fd);

                                break;
                        case 5:
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
                return ret;
}
