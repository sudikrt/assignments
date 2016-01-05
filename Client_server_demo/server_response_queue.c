#include "socket_header.h"

void write_handler (void* arg, int client_fd)
{
        
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
void server_response (int client_fd, queue_t* queue_ref)
//void* server_response (void* arg)
{
        char* buffer;
        char* data;
        int ret         =       0;
        int res         =       0;
        C_Request c_request;

        /*service_obj_t* obj_service = (service_obj_t*) arg;
        int client_fd = obj_service -> client_fd;
        queue_t* queue_ref = obj_service -> obj;
        */
        buffer = (char*) calloc (1,1024);
        data = (char*) calloc (1,1024);
        //while (1)
        //{
                ret = read (client_fd, buffer, 1024);
                if(ret != 0)
                {
                        /*Read the type of the request from the client request*/
                        memset (data, 0, 1024);
                        memcpy (data, buffer, 2);
                        res = atoi (data);
                        
                        switch (res)
                        {
                                case read_request:
                                                c_request.type = read_request;
                                                c_request.buf = buffer;

                                                c_request.client_fd = client_fd;
                                                c_request.operation = 
                                                        (void*) read_handler;
                                                break;
                                case write_request:
                                                c_request.type = write_request;
                                                c_request.buf = buffer;
                                                c_request.client_fd = client_fd;
                                                c_request.operation = 
                                                        (void*) write_handler;
                                                break;
                                case quit:
                                        goto out;
                                        break;
                        }
                        /* Check whether the queue is full or not 
                         * if queue is full then send feedback to client that
                         * client is full otherwise the request is added to the
                         * queue
                         * */
                red:    if (queue_full (queue_ref) == 1)
                        {
                                memset (data, 0, 1024);
                                sprintf (data, "%d", queue_ful);
                                ret = write (client_fd, data, 1024);
                                if (ret < 0)
                                {
                                        printf ("\tSome error ocurred\n");
                                        goto red;
                                }
                        }
                        else
                        {
                                memset (data, 0, 1024);
                                sprintf (data, "%d", queue_nful);
                                ret = write (client_fd, data, 1024);
                                if (ret < 0)
                                {
                                        printf ("\tSome error ocurred\n");
                                        goto red;
                                }
                                /*Calles the function to add the client_request to the 
                                 * queue */
                                ret = queue_put (queue_ref, &c_request);
                                printf ("\tClient : %d's request is inserted\n",
                                                                client_fd);
                                set_thread_flag (1);
                        }
                }

        //}
        out:
                //close (client_fd);
                //printf ("\tClient : %d \t has disconnected \n", client_fd);
                //free (data);
                //free (buffer);
                return ;
}