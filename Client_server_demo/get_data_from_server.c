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
        int success     =       0;
        ssize_t ret     =       0;
        int buf_ptr     =       4;

        /* Allocate the memory*/
        buffer = (char*) calloc (1, 1024);

        /* Clears the buffer content */
        memset (buffer, 0, sizeof(buffer));

        /* Read the data sent from server */
        ret = read (client_fd, buffer, 1024);

        if(ret != 0)
        {
                /* Copy the type of the response */
                memcpy (&ret, buffer + buf_ptr, sizeof (int));
                buf_ptr += sizeof (int);
                
                if (ret == fail_response) {
                        /* Copy the type of failure */
                        memcpy (&ret, buffer + buf_ptr, sizeof (int));

                        switch(ret)
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
                                /* If the queue was full */
                                case queue_ful:
                                        printf ("\tERROR: Queue full\n");
                                        goto out;
                                        break;
                        }
                }
                else {
                        /* Copy the Length of the file */
                        memcpy (&ret, buffer + buf_ptr, sizeof (ssize_t));

                        /* Calls the function to recieve the named file 
                         * from the server only if the file exist.*/
                        ret = get_doc_from_server(client_fd, file_name, ret);
                        if(ret < 0)
                        {
                                printf ("\tError in saving the file\n");
                                goto out;
                        }
                        printf ("\tFile Successfully saved\n");
                }
        }
        out:
                return;
}