#include "socket_header.h"

void write_handler (void* arg, int client_fd)
{
        
}
/* Calculate the size of the failure response */
ssize_t
calc_file_open_response_fail () {
        ssize_t len     =       0;
        
        /* Length of response type */
        len = sizeof (int);
        
        /* Length of type of failure */
        len += sizeof (int);
        
        /* Length of the return arg type*/
        len += sizeof (int);
        
        /* Lenght of the footer */
        len += sizeof (int);

out:
        return len;
}

/* Calculate the lenght of the Success response */
ssize_t
calc_file_open_response_success () {
        ssize_t len     =       0;
        
        /* Length of response type */
        len = sizeof (int);
        
        /* Length of return arg. i.e fd*/
        len += sizeof (int);
        
        /* Length of the footer */
        len += sizeof (int);
out:
        return len;
}

void open_read_handler (void* arg, int client_fd)
{
        char* req_buf;
        char* file_name;
        int buf_ptr     =       0;
        int num_of_arg  =       0;
        int file_len    =       0;
        int file_mode   =       0;
        int ret         =       -1;
        int file_desc   =       0;
        
        
        /* Allocate the memory */
        req_buf = (char*) calloc (1, 1024);
        
        /* Copy the arguments */
        req_buf = arg;
        
        /* Copy the number of arguments */
        memcpy (&num_of_arg, req_buf, sizeof (int));
        buf_ptr += sizeof (int);
        
        /* Copy the length of the file_name */
        memcpy (&file_len, req_buf + buf_ptr, sizeof (int));
        buf_ptr += sizeof (int);
        
        /* Allocate the memory */
        file_name = (char*) calloc (1, file_len);
        
        /* Copy the file_name */
        memcpy (file_name, req_buf + buf_ptr, file_len);
        buf_ptr += file_len;
        
        /* Copy the file_open mode */
        memcpy (&file_mode, req_buf + buf_ptr, sizeof (int));
        buf_ptr += sizeof (int);

        /* Call the open file handler */
        ret = open_file (file_name, file_mode, &file_desc);
        if (ret == 0)
        {
                buf_ptr = 0;
                /* Calculate the length of the response */
                file_len = calc_file_open_response_success ();
                
                /* Allocate the memory */
                req_buf = (char*) calloc (1, file_len + sizeof (int));
                
                /* Copy the length of the response */
                memcpy (req_buf, &file_len, sizeof (int));
                buf_ptr += sizeof (int);
                
                /* Copy the response type */
                file_len = success_response;
                memcpy (req_buf + buf_ptr, &file_len, sizeof (int));
                buf_ptr += sizeof (int);
                
                /* Copy the response i.e fd */
                memcpy (req_buf + buf_ptr, &file_desc, sizeof (int));
                buf_ptr += sizeof (int);
                
                /* Copy the footer */
                file_len = 0xbadf00d;
                memcpy (req_buf + buf_ptr, &file_len, sizeof (int));
                buf_ptr += sizeof (int);
                
                goto out;
        }

        /* If the file is alredya opened */
        else if (ret == file_already_open) {
                buf_ptr = 0;
                /* Calculate the length of the response */
                file_len = calc_file_open_response_fail ();
                
                /* Allocate the memory */
                req_buf = (char*) calloc (1, file_len + sizeof (int));
                
                /* Copy the length of the response */
                memcpy (req_buf, &file_len, sizeof (int));
                buf_ptr += sizeof (int);
                
                /* Copy the response type */
                file_len = fail_response;
                memcpy (req_buf + buf_ptr, &file_len, sizeof (int));
                buf_ptr += sizeof (int);
                
                /* Copy the fail_type */
                file_len = file_already_open;
                memcpy (req_buf + buf_ptr, &file_len, sizeof (int));
                buf_ptr += sizeof (int);
                
                /* Copy the response i.e fd */
                memcpy (req_buf + buf_ptr, &file_desc, sizeof (int));
                buf_ptr += sizeof (int);
                
                /* Copy the footer */
                file_len = 0xbadf00d;
                memcpy (req_buf + buf_ptr, &file_len, sizeof (int));
                buf_ptr += sizeof (int);
                
                goto out;
        }
        /* If the file is not present in the directory */
        else if (ret == ENOENT) {
                 buf_ptr = 0;
                /* Calculate the length of the response */
                file_len = calc_file_open_response_success ();
                
                /* Allocate the memory */
                req_buf = (char*) calloc (1, file_len + sizeof (int));
                
                /* Copy the length of the response */
                memcpy (req_buf, &file_len, sizeof (int));
                buf_ptr += sizeof (int);
                
                /* Copy the response type */
                file_len = fail_response;
                memcpy (req_buf + buf_ptr, &file_len, sizeof (int));
                buf_ptr += sizeof (int);
                
                /* Copy the fail_type */
                file_len = ENOENT;
                memcpy (req_buf + buf_ptr, &file_len, sizeof (int));
                buf_ptr += sizeof (int);
                
                /* Copy the footer */
                file_len = 0xbadf00d;
                memcpy (req_buf + buf_ptr, &file_len, sizeof (int));
                buf_ptr += sizeof (int);
                
                goto out;
        }
        ret = write (client_fd, req_buf, buf_ptr);
        if (ret < 0) {
        
                printf ("\tCommunication error\n");
                goto out;
        }
out:
        return;
}


/* Function to calculate the request size */
ssize_t 
calc_response_queue_full () {
        ssize_t len     =       0;
        
        /* Length of Response type */
        len = sizeof (int);
        
        /* Size of the response */
        len += sizeof (int);
        
        /* Footer size. */
        len += sizeof (int);

out:
        return len;
}

/*This function set the Thread_flag value with specified value
 * Input :
 *      int flag_value          :       Specifies the value to be set 
 *                                      to the thread_flag variable
 */
void set_thread_flag (int flag_value)
{
        /* Lock the mutex for accesing the falg value */
        pthread_mutex_lock (&thread_flag_mutex);
        thread_flag = flag_value;
        pthread_cond_signal (&thread_flag_cv);
        /* Unlock the mutex.*/
        pthread_mutex_unlock (&thread_flag_mutex);
}

/*Recieves the client request and addes it to the queue
 * Input:
 *      int client_fd   :       specifies the connection identifier
 * */
void server_response (int client_fd, thread_pool_t *pool_obj, C_Request* c_request)
{
        char* buffer;
        char* data;
        int len         =       0;
        int ret         =       0;
        long res        =       0;
        int buf_ptr     =       0;
        buffer = (char*) calloc (1,1024);

        ret = read (client_fd, buffer, 1024);
        printf ("Recieved bytes %d\n", ret);
        if(ret > 0)
        {
                /* Read the footer. */
                memcpy (&res, (buffer + (ret - sizeof (int)) ), sizeof (int));

                /* Checking the message is valid or not .*/
                if (res != 0xbaadf00d) {
                        goto out;
                }
                
                /* Read the length of the message */
                memcpy (&len, buffer, sizeof (int));
                buf_ptr += sizeof (int);

                /* If valid message  read the type of the request. */
                memcpy (&res, buffer + buf_ptr, sizeof (int));
                buf_ptr += sizeof (int);

                switch (res)
                {
                        case file_request:
                                        /* Process the file request */
                                        c_request -> type = file_request;
                                        c_request -> buf = (char*) calloc (1, ret - buf_ptr);
                                        memcpy (c_request -> buf, buffer + buf_ptr, 
                                                                ret - buf_ptr) ;
                                        c_request -> client_fd = client_fd;
                                        c_request -> operation = (void*) file_handler;
                                        break;
                        case write_request:
                                        /* Process the write request */
                                        c_request -> type = write_request;
                                        c_request -> buf = buffer;
                                        c_request -> client_fd = client_fd;
                                        c_request -> operation = (void*) write_handler;
                                        break;
                        case open_file_request:
                                        /* Process the open_file_request */
                                        c_request -> type = open_file_request;
                                        c_request -> buf = (char*) calloc (1, ret - buf_ptr);
                                        memcpy (c_request -> buf, buffer + buf_ptr, 
                                                                ret - buf_ptr) ;
                                        c_request -> client_fd = client_fd;
                                        c_request -> operation = (void*) open_file_handler;
                                        break;
                        case close_file_request:
                                        /* Process the close file request */
                                        c_request -> type = close_file_request;
                                        c_request -> buf = (char*) calloc (1, ret - buf_ptr);
                                        memcpy (c_request -> buf, buffer + buf_ptr,
                                                                ret - buf_ptr);
                                        c_request -> client_fd = client_fd;
                                        c_request -> operation = (void*) close_file_handler;
                                        break;
                        case read_in_chunks:
                                        /* Process the read in chunks from the file 
                                         * and send it  back to the client */
                                        c_request -> type = read_in_chunks;
                                        c_request -> buf = (char*) calloc (1, ret - buf_ptr);
                                        memcpy (c_request -> buf, buffer + buf_ptr,
                                                                ret - buf_ptr);
                                        c_request -> client_fd = client_fd;
                                        c_request -> operation = (void*) read_chunks_handler;
                                        break;
                }
                /* Check whether the list is full or not 
                * if list is full then send feedback to client that
                * client is full otherwise the request is added to the
                * queue
                * */
        data_read:
                if (list_full (pool -> list) == 1)
                {
                        /* Clears the buffer */
                        memset (buffer, 0, 1024);
                        res = 0;
                        /* Gives the length of the response. */
                        len = calc_response_queue_full ();
                        
                        /* Allocate the memory */
                        buffer = (char*) calloc (1, len + 4);
                        
                        /* Copy the length of the request. */
                        memcpy (buffer, &len, sizeof (int));
                        res += sizeof (int);
                        
                        /*Copy the type of the response */
                        len = fail_response;
                        memcpy (buffer + res, &len, sizeof (int));
                        res += sizeof (int);
                        
                        /* Copy the response */
                        len = queue_ful;
                        memcpy (buffer + res, &len, sizeof (int));
                        res +=sizeof (int);
                        
                        /* Send the response to the client */
                        ret = write (client_fd, data, res);
                        if (ret < 0)
                        {
                                printf ("\tSome error ocurred\n");
                                goto data_read;
                        }
                        goto out;
                }

                /*Calls the function to add the client_request to the 
                * list */
                pool_obj -> list -> first_node = insert_into_list 
                                                (pool_obj -> list, c_request);
                printf ("\tClient : %d's request is inserted\n", client_fd);
                pthread_mutex_lock (& (pool_obj -> q_lock));
                pthread_cond_broadcast (& pool_obj -> cond_t);
                pthread_mutex_unlock (& (pool_obj -> q_lock));
                
        }
        out:
                return ;
}