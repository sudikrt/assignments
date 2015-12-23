#include "queue.h"

void read_handler (void* arg, int client_fd)
{
        int ret         =       0;
        int len         =       0;
        char* buffer;
        char* data;
        char* req_buf;

        data = (char*) calloc (1,100);
        req_buf = (char*) calloc (1, 1024);
        
        req_buf = (char*) arg;
        /*Clear the contents of data*/
        memset (data, 0, 100);

        /*Read the length for the file_name*/
        memcpy (data, &req_buf [2], 2);
        len = atoi (data);

        /*Clear the content of data and read the file_name*/
        memset (data, 0, 100);
        memcpy (data, &req_buf [4], len);

        /*Check the file exist in the exp_dir*/
        ret = check_file_exist(data);

        /*If file exists*/
        if (ret == file_exist)
        {
                memset (req_buf, 0, 1024);
                sprintf (req_buf, "%d", file_exist);
                ret = write (client_fd, req_buf, 1024);
                if (ret < 0)
                {
                        printf ("\tCommunication error\n");
                        return;
                }
                
                ret = send_data_to_client (client_fd, data);
                if(ret < 0)
                {
                        printf ("\tError in sending file \n");
                        return;
                }
                printf("\tFinished sending\n");
        }
        /*File not exists send the error message to client*/
        else
        {
                /*Set the the erro code*/
                memset (req_buf, 0, sizeof(buffer));
                sprintf (req_buf, "%d", ENOENT);
                ret = write (client_fd, req_buf, 1024);
                if (ret < 0)
                {
                        printf ("\tCommunication error\n");
                        return;
                }
        }
        //close (client_fd);
        return;
}