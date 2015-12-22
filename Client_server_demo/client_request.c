#include "socket_header.h"

/*
 * Client requesting the server for the file
 * Input:
 *      int     :       specifies the connection identifier.
 * Output:
 *      int     :       Specifies the success or failure.
 * */
int request (int client_fd)
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

        /* Allocate the memory. */
        file_name = (char*) calloc (1, 500);
        data = (char*) calloc (1, 100);
        extra = (char*) calloc (1, 100); 
        buffer = (char*) calloc (1, 1024);
        
        while (1)
        {
        menu:
                printf ("\tPress \n\t1. Read\n \t2. Write\n\t");
                scanf ("%d", &value);
                
                switch (value)
                {
                        case 1:
                                memset (buffer, 0, sizeof(buffer));
                                sprintf (data, "%d", read_request);
                                strcat (buffer, data);
                                printf ("\tEnter the file name:");
                                scanf ("%s", file_name);
                                len = strlen (file_name);
                                memset (extra, 0, sizeof (data));
                                memset (data, 0, sizeof (data));
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
                                /* Calls this function to retrive the data from
                                 * server. */
                                get_data_from_server (client_fd, file_name);
                                break;
                        case 2:
                                
                                break;
                        default:
                                ret = 0;
                                goto out;
                }

        }
        ret = 0;

        out:
            return ret;
}
