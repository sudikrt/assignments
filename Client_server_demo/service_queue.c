#include "socket_header.h"

/*Function to remove the request from the queue and process its request*/
void* service_queue(void* arg)
{
        queue_t* object = (queue_t*) arg;
        queue_cbk_t cbk_obj;
        int ret = 0;
        while (1){
                /* Lock the mutex for accessing the flag value. */
                pthread_mutex_lock (&thread_flag_mutex);
                while (!thread_flag)
                {
                        /*Wait for a signal on the condition variable,
                        indicating that the flag value has changed*/
                       pthread_cond_wait (&thread_flag_cv, &thread_flag_mutex);
                }
                /*Unlock the mutex*/
                pthread_mutex_unlock (&thread_flag_mutex);

                /*Do Further work after condition variable is set*/
                if(queue_empty (object) != 1)
                {
                        /* Assign the callback function */
                        cbk_obj = handle_request;
                        ret = queue_get (object, cbk_obj);
                }
                else
                {
                        /* Set the flag to zero if the queue was empty 
                         * for blocking the thread*/
                        set_thread_flag (0);
                }
        }
}