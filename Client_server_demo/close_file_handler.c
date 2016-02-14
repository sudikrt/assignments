#include "socket_header.h"

ssize_t
calc_close_file_success () {
        ssize_t len = 0;
        
        /* Size of the total length*/
        len = sizeof (int);
        
        /* Size of the response type */
        len += sizeof (int);
        
        /* Size of the footer */
        len += sizeof (int);
out:
        return len;
}

ssize_t
calc_insuff_arg_len () {
        ssize_t len = 0;
        
        /* Size of the total length*/
        len = sizeof (int);
        
        /* Size of the response type */
        len += sizeof (int);
        
        /* Size o the response code */
        len += sizeof (int);

        /* Size of the footer */
        len += sizeof (int);
out:
        return len;
}

void
close_file_handler (void* arg, int client_fd) {
        
        int ret         =       0;
        int len         =       0;
        int fd          =       0;
        int res         =       0;
        int err_code    =       0;
        int buf_ptr     =       0;
        char* req_buf;
        char* buffer;

        
        req_buf = (char*) calloc (1, 1024);
        
        req_buf =  arg;

        /* Read the number of arguments. */
        memcpy (&len, req_buf, sizeof (int));
        buf_ptr += sizeof (int);

        /* Check the number of argument. */
        if (len != 1) {
                /* Clear the buffer*/
                memset (req_buf, 0, 1024);
                
                /* find the length of the response */
                len = calc_insuff_arg_len ();
                
                /* Allocate the memory */
                buffer = (char*) calloc (1, len);
                
                /* Copy the length of the response. */
                memcpy (buffer, &len, sizeof (int));
                res += sizeof (int);

                /*Copy the type of the response */
                len = fail_response;
                memcpy (buffer + res, &len, sizeof (int));
                res += sizeof (int);

                /* Copy the error code */
                len = insufficient_arg;
                memcpy (buffer + res, &len, sizeof (int));
                res += sizeof (int);
                
                /* Footer */
                len = 0xBAADF00D;
                memcpy (buffer + res, &len, sizeof (int));
                res += sizeof (int);
                
                goto send_err;
        }
        
        /* Copy the file_descriptor */
        memcpy (&len, req_buf + buf_ptr, sizeof (int));
        buf_ptr += sizeof (int);
        
        ret = close_file (&len);
        
        
        if (ret == 0) {
                /* Clear the buffer*/
                memset (req_buf, 0, 1024);
                
                /* find the length of the response */
                len = calc_close_file_success ();
                
                /* Allocate the memory */
                buffer = (char*) calloc (1, len);
                
                /* Copy the length of the response. */
                memcpy (buffer, &len, sizeof (int));
                res += sizeof (int);

                /*Copy the type of the response */
                len = success_response;
                memcpy (buffer + res, &len, sizeof (int));
                res += sizeof (int);
                
                /* Footer */
                len = 0xBAADF00D;
                memcpy (buffer + res, &len, sizeof (int));
                res += sizeof (int);
        }
        else if (ret == file_already_closed) {
                
                /* Clear the buffer*/
                memset (req_buf, 0, 1024);
                
                /* find the length of the response */
                len = calc_close_file_success ();
                
                /* Allocate the memory */
                buffer = (char*) calloc (1, len);
                
                /* Copy the length of the response. */
                memcpy (buffer, &len, sizeof (int));
                res += sizeof (int);

                /*Copy the type of the response */
                len = fail_response;
                memcpy (buffer + res, &len, sizeof (int));
                res += sizeof (int);
                
                /* Footer */
                len = 0xBAADF00D;
                memcpy (buffer + res, &len, sizeof (int));
                res += sizeof (int);
        }

send_err:
        ret = write (client_fd, buffer, res);
        
        if (ret < 0) {
                printf ("\tCommunication error\n");
                return;
        }
        
        close (client_fd);
        return;
}