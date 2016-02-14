#include "socket_header.h"

ssize_t calc_response_file_not_exist () {
        ssize_t len     =       0;

        /* Response type */
        len = sizeof (int);

        /* Size of the response */
        len += sizeof (int);

        /* Footer size. */
        len += sizeof (int);

out:
        return len;
}

void file_handler (void* arg, int client_fd)
{
        int ret         =       0;
        int len         =       0;
        int res         =       0;
        int err_code    =       0;
        int buf_ptr     =       0;
        char* buffer;
        char* data;
        char* req_buf;

        
        req_buf = (char*) calloc (1, 1024);
        
        req_buf =  arg;
        
        
        /* Read the number of arguments. */
        memcpy (&len, req_buf, sizeof (int));
        buf_ptr += sizeof (int);

        /* Check the number of argument. */
        if (len != 1) {
                err_code = insufficient_arg;
                goto send_err;
        }
        
        /* Read the length of the arguments */
        memcpy (&len, req_buf + buf_ptr, sizeof (int));
        buf_ptr += sizeof (int);

        /* Read the name of the file */
        data = (char*) calloc (1,len);
        memcpy (data, req_buf + buf_ptr, len);

        /* */
        /*Check the file exist in the exp_dir*/
        ret = check_file_exist(data);

        /*If file exists*/
        if (ret == file_exist)
        {
                /* */
                ret = send_data_to_client (client_fd, data);
                if(ret < 0)
                {
                        printf ("\tError in sending file to client : %d\n", 
                                                                client_fd);
                        return;
                }
                printf("\tFinished sending file to client : %d\n", 
                                                                client_fd);
        }
        /*File not exists send the error message to client*/
        else
        {
                err_code = ENOENT;
                /* Gives the length of the response. */
send_err:       len = 0;
                res = 0;
                len = calc_response_file_not_exist ();
                
                /* Allocate the memory */
                buffer = (char*) calloc (1, len + 4);
                
                /* Copy the length of the request. */
                memcpy (buffer, &len, sizeof (int));
                res += sizeof (int);
                
                /*Copy the type of the response */
                len = fail_response;
                memcpy (buffer + res, &len, sizeof (int));
                res += sizeof (int);
                
                /* Copy the error code */
                len = err_code;
                memcpy (buffer + res, &len, sizeof (int));
                res +=sizeof (int);
                
                /* Footer */
                len = 0xBAADF00D;
                memcpy (buffer + res, &len, sizeof (int));
                res += sizeof (int);

                /* Send the response back to the client */
                ret = write (client_fd, buffer, res);
                printf ("Sent Bytes is %d\n", ret);
                if (ret < 0)
                {
                        printf ("\tCommunication error\n");
                        return;
                }
        }
        close (client_fd);
        return;
}