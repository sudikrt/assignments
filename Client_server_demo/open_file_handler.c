#include "socket_header.h"
/* calculate the size of the response */
ssize_t
calc_file_open_response () {
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

/* claluate the size of the response if file already open */
ssize_t
calc_file_alredy_open () {
        ssize_t len     =      0;
        
        /* Copy the length of the response type */
        len = sizeof (int);
        
        /* Copy the length of response */
        len += sizeof (int);
        
        /* Copy the length return arg */
        len += sizeof (int);
        
        /* footer length */
        len += sizeof (int);
        
out:
        return len;
}
/* 
 * Function which performs opening of the requested file by the client
 * If the file not exist the appropriate error message saying file not exist
 * If the file already opened the the file fd and and an error saying file 
 * already opened
 * On success file fd will be returned 
 * */
void
open_file_handler (void* arg, int client_fd) {
        
        int ret         =       -1;
        int len         =       0;
        int res         =       0;
        int buf_ptr     =       0;
        int mode        =       0;
        int* desc       = (int*) calloc (1, sizeof(int));
        
        char* buffer;
        char* file_name;
        
        buffer = (char*) calloc (1, 1024);

        buffer = arg;
        
        /* Read the number of arguments */
        memcpy (&len, buffer, sizeof (int));
        buf_ptr += sizeof (int);
        
        /* Check the number of arguments*/
        if (len != 2) {
                len = calc_file_open_response ();
                                        
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

        /* Read the length of the first argument */
        memcpy (&len, buf_ptr + buffer, sizeof (int));
        buf_ptr += sizeof (int);

        /* Allocate the memory for the storing the file name. */
        file_name = (char*) calloc (1, len);

        /* Read the argument i.e. name of the file*/
        memcpy (file_name, buf_ptr + buffer, len);
        buf_ptr += len;

        /* Read the length of the second argument */
        memcpy (&len, buf_ptr + buffer, sizeof (int));
        buf_ptr += sizeof (int);

        /* Read the file open mode */
        memcpy (&mode, buf_ptr + buffer, len);
        buf_ptr += sizeof (int);

        ret = open_file (file_name, desc, mode);

        switch (ret) 
        {
                case file_open_error :
                                        len = calc_file_open_response ();
                                        
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
                case file_already_open :
                                        /* Find the length of the response */
                                        len = calc_file_alredy_open ();
                                        
                                        /* Allocate the memory */
                                        buffer = (char*) calloc (1, len + 4);
                                        
                                        /* Copy the length of the resposne. */
                                        memcpy (buffer, &len, sizeof (int));
                                        res += sizeof (int);
                                        
                                        /*Copy the type of the response */
                                        len = fail_response;
                                        memcpy (buffer + res, &len, sizeof (int));
                                        res += sizeof (int);
                                        
                                        /*Copy the error code */
                                        len = file_already_open;
                                        memcpy (buffer + res, &len, sizeof (int));
                                        res += sizeof (int);
                                        
                                        /* Copy the return arg */
                                        len = *desc;
                                        memcpy (buffer + res, &len, sizeof (int));
                                        res += sizeof (int);
                                        
                                        /* Footer */
                                        len = 0xBAADF00D;
                                        memcpy (buffer + res, &len, sizeof (int));
                                        res += sizeof (int);

                                        break;
                case ENOENT :
                                        len = calc_file_open_response ();
                                        
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
                                        len = ENOENT;
                                        memcpy (buffer + res, &len, sizeof (int));
                                        res += sizeof (int);
                                        break;
                                        
                                        /* Footer */
                                        len = 0xBAADF00D;
                                        memcpy (buffer + res, &len, sizeof (int));
                                        res += sizeof (int);
                                        break;
                case 0 :
                                        /* Find the length of the response */
                                        len = calc_file_open_response ();
                                        
                                        /* Allocate the memory */
                                        buffer = (char*) calloc (1, len + 4);
                                        
                                        /* Copy the length of the resposne. */
                                        memcpy (buffer, &len, sizeof (int));
                                        res += sizeof (int);
                                        
                                        /* Copy the type of the response */
                                        len = success_response;
                                        memcpy (buffer + res, &len, sizeof (int));
                                        res += sizeof (int);
                                        
                                        /* Copy the return arg */
                                        //len = *desc;
                                        printf ("Desc is %d, %d\n", *desc, len);
                                        memcpy (buffer + res, desc, sizeof (int));
                                        res += sizeof (int);

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

