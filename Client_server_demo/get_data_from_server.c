#include "socket_header.h"
/*
 * This function get the file information from server
 * that is confirms whether the file exists or not.
 * Input:
 *      int client_fd   :       Specifies the connection identifier.
 *      char* file_name :       Specifies the name of the file.
 * Output:
 *      void.
 * */
void get_data_from_server (int client_fd, char* file_name)
{
        char* buffer;
        char* data;
        int success     =       0;
        int ret         =       0;

        /* Allocate the memory*/
        buffer = (char*) calloc (1, 1024);
        data = (char*) calloc (1, 100);

        /* Clears the buffer content */
        memset (buffer, 0, sizeof(buffer));

        /* Read the data sent from server */
        ret = read (client_fd, buffer, 1024);

        if(ret != 0)
        {
                memset (data, 0, sizeof(data));
                memcpy (data, buffer, 2);
                success = atoi (data);

                switch(success)
                {
                /*If file not exist in server*/
                        case ENOENT:
                                fprintf (stderr, "\tERROR: %s\n", 
                                                        strerror(ENOENT));
                                goto out;
                                break;
                        /*If File read fail */
                        case read_fail:
                                printf ("\tERROR: Read Fail\n");
                                goto out;
                                break;
                        case file_exist:
                                /* Calls the function to recieve the named file 
                                 * from the server only if the file exist. */
                                ret = get_doc_from_server(client_fd, file_name);
                                if(ret < 0)
                                {
                                        printf ("\tError in saving the file\n");
                                        goto out;
                                }
                                printf ("\tFile Successfully saved\n");
                                break;
                }
        }
        out:
                free (buffer);
                free (data);
                return;
}