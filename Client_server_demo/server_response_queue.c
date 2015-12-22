#include "queue.h"

void write_handler (void* arg, int client_fd)
{
        
}

/*Recieves the client request and addes it to the queue
 * Input:
 *      int client_fd   :       specifies the connection identifier
 * */
void server_response (int client_fd)
{
        char* buffer;
        char* data;
        int ret         =       0;
        int res         =       0;
        C_Request c_request;

        buffer = (char*) calloc (1,1024);
        data = (char*) calloc (1,100);

        while (1)
        {
                ret = read (client_fd, buffer, 1024);
                if(ret != 0)
                {
                        /*Read the type of the request from the client request*/
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
                        }
                        /*Lockes the mutex*/
                        pthread_mutex_lock(&lock);
                        /*Calles the function to add the client_request to the 
                         * queue */
                        add_queue ((void*) &c_request);
                        /*Unlocks the mutex*/
                        pthread_mutex_unlock(&lock);

                }
        }

        return ;
}