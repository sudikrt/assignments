#include "socket_header.h"

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
        int exit_var    =       1;
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
                printf ("\tPress \n\t1. Read\n \t2. Write\n \t3. Quit\n \t");
                scanf ("%d", &value);
                if (value > 3 || value < 1)
                {
                        printf ("\tEnter the proper option try again. . \n");
                        goto menu;
                }
                switch (value)
                {
                        case 1:
                                client_fd = sftp_rpc_client (port, server_name);
                                if(client_fd < 0)
                                {
                                        printf ("\tConnection failure try again\n");
                                        goto menu;
                                }
                                printf ("\tConnection Success!!\n");
                                memset (buffer, 0, sizeof(buffer));
                                sprintf (data, "%d", read_request);
                                strcat (buffer, data);
                                printf ("\tEnter the file name:");
                                scanf ("%s", file_name);
                                len = strlen (file_name);
                                
                                memset (extra, 0, 100);
                                memset (data, 0, 100);
                                sprintf (extra, "%d", 0);
                                sprintf (data, "%d", len);

                                if (strlen (data) == 1)
                                {
                                        strcat (extra, data);
                                        strcpy (data, extra);
                                }

                                strcat (buffer, data);
                                strcat (buffer, file_name);
                                ret = write (client_fd, buffer, 1024);
                                if (ret < 0)
                                {
                                        printf ("\tSome error ocurred\n");
                                        goto menu;
                                }
                                
                                memset (buffer, 0, 1024);
                                ret = read (client_fd, buffer, 1024);
                                if (ret < 0)
                                {
                                        printf ("\tSome error ocurred\n");
                                        goto out;
                                }
                                
                                memset (data, 0, 100);
                                memcpy (data, buffer, 2);
                                len = atoi (data);
                                /* Check for the server busy */
                                if (len == queue_ful)
                                {
                                        printf ("\tTo many requests try later\n");
                                        goto menu;
                                }
                                if (len == queue_nful)
                                {
                                        /* Calls this function to retrive the
                                         * data from server.
                                         * */
                                        printf ("\tRequest in the queue..processing\n");
                                        get_data_from_server (client_fd, file_name);
                                        //close (client_fd);
                                }
                                break;
                        case 2:
                                break;
                        case 3:
                                memset (buffer, 0, 1024);
                                sprintf (buffer, "%d", quit);
                                ret = write (client_fd, buffer, 1024);
                                if (ret < 0)
                                {
                                        printf ("\tSome error ocurred\n");
                                        goto menu;
                                }
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
