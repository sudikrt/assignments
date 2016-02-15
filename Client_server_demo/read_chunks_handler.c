#include "socket_header.h"

/* calculate the size of the response */
ssize_t
calc_read_error_response () {
        ssize_t len     =       0;

        /* Copy the length of the response type */
        len = sizeof (int);

        /* Copy the length of return argument */
        len += sizeof (int);

        /* footer length */
        len += sizeof (int);
out:
        return len; 
}

/* claculate the length of the responese containing the read bytes */
ssize_t
calc_read_success_response (int read_byte) {
        
        ssize_t len = 0;
        
        /* Copy the length of the response type */
        len = sizeof (int);

        /* Copy the length of return argument */
        len += sizeof (int);
        
        /* Copy the read_bytes */
        len += sizeof (int);

        /* footer length */
        len += sizeof (int);
out:
        return len; 
}

/*
 * This function is used to read the number of bytes from the particular file.
 * */
void
read_chunks_handler (void* arg, int client_fd) {
        int ret         =       -1;
        int len         =       0;
        int res         =       0;
        int buf_ptr     =       0;
        int mode        =       0;
        
        unsigned int fd;
        char* buffer;
        char* read_buf;
        
        /* Allocates the memory */
        buffer = (char*) calloc (1, 1024);

        /* Copy the arguments */
        buffer = arg;
        
        /* Read the number of arguments */
        memcpy (&len, buffer, sizeof (int));
        buf_ptr += sizeof (int);
        
        /* Check the number of arguments*/
        if (len != 2) {
                len = calc_read_error_response ();
                                        
                /* Allocate the memory */
                buffer = (char*) calloc (1, len + 4);
                
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
        
        
        /* Read the file_desc */
        memcpy (&fd, buf_ptr + buffer, sizeof (int));
        buf_ptr += sizeof (int);
        
        /* Read the length of bytes to be read */
        memcpy (&len, buf_ptr + buffer, sizeof (int));
        buf_ptr += sizeof (int);
        
        read_buf = (char*) calloc (1, len);
        
        ret = read_bytes (fd, read_buf, &len);
        
        switch (ret) {
                case -1:
                        /* Send the error message for the read fail */
                        
                        /* Claculate the length of the error message */
                        len = calc_read_error_response ();
                        
                        /* Allocate the memory */
                        buffer = (char*) calloc (1, len + 4);
                        
                        /* Copy the length of the response. */
                        memcpy (buffer, &len, sizeof (int));
                        res += sizeof (int);

                        /*Copy the type of the response */
                        len = fail_response;
                        memcpy (buffer + res, &len, sizeof (int));
                        res += sizeof (int);

                        /* Copy the error code */
                        len = file_open_error;
                        memcpy (buffer + res, &len, sizeof (int));
                        res += sizeof (int);

                        /* Footer */
                        len = 0xBAADF00D;
                        memcpy (buffer + res, &len, sizeof (int));
                        res += sizeof (int);
                        
                        break;
                case file_already_closed:
                        /*Send the error message if the file is closed */
                        
                        /* Calculate the response length */
                        len = calc_read_error_response ();
                        
                        /* Allocate the memory */
                        buffer = (char*) calloc (1, len + 4);
                        
                        /* Copy the length of the response. */
                        memcpy (buffer, &len, sizeof (int));
                        res += sizeof (int);

                        /*Copy the type of the response */
                        len = fail_response;
                        memcpy (buffer + res, &len, sizeof (int));
                        res += sizeof (int);

                        /* Copy the error code */
                        len = file_already_closed;
                        memcpy (buffer + res, &len, sizeof (int));
                        res += sizeof (int);

                        /* Footer */
                        len = 0xBAADF00D;
                        memcpy (buffer + res, &len, sizeof (int));
                        res += sizeof (int);
                        break;
                case 0:
                        /* On successful read operation send the read data to the client */
                        
                        /* Calculates the length of the response */
                        ret = calc_read_success_response (len);
                        
                        /* Allocate the memory */
                        buffer = (char*) calloc (1, ret + 4);
                        
                        /* Copy the length of the response. */
                        memcpy (buffer, &ret, sizeof (int));
                        res += sizeof (int);

                        /*Copy the type of the response */
                        ret = success_response;
                        memcpy (buffer + res, &ret, sizeof (int));
                        res += sizeof (int);

                        /* Copy the no. of bytes read */
                        ret = len;
                        memcpy (buffer + res, &ret, sizeof (int));
                        res += sizeof (int);

                        /* Copy the bytes read */
                        memcpy (buffer + res, read_buf, len);
                        res += len;

                        /* Footer */
                        len = 0xBAADF00D;
                        memcpy (buffer + res, &len, sizeof (int));
                        res += sizeof (int);
                        break;
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