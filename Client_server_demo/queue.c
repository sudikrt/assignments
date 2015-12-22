#include "queue.h"

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



/*Function to add the client request to the queue
 * 
 * Input :
 *      void* arg       :       Structure contains the client reqest
 * */
void add_queue (void* arg)
{
        C_Queue *current;
        C_Request data;
        current = head;
        pthread_mutex_t lock;

        data = *((C_Request*) arg) ;

        while (current -> next != head) 
        {
                current = current -> next;
        }
        /*Allocate the memory the for the new data
         * and store the data in in*/
        current -> next = (C_Queue*) calloc (1, sizeof(C_Queue));
        current = current -> next;
        current -> c_req = data;
        current -> next = head;
        printf ("\tRequest is inserted\n");

        /* Set the thread_flag to indicate that request has inserted to
         * the queue which allows the thread to be unblocked
         */
        set_thread_flag (1);

        return;
}

/*Function to remove the request from the queue and process its request*/
void* delete_queue ()
{
        C_Queue *previous ;
        C_Request request;
       
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
                if(head -> next != head)
                {
                        /*Removes the request on the top of the queue*/
                        previous = head -> next;
                        head -> next = previous -> next;
                        request = previous -> c_req;
                        
                        bzero (previous, sizeof(previous));
                        /*Calls the request handler based of the type of 
                         * the request*/
                        switch (request.type)
                        {
                                case read_request:
                                                request.operation (request.buf, 
                                                                request.client_fd);
                                                break;
                                case write_request:
                                                request.operation (request.buf, 
                                                                request.client_fd);
                                                break;
                        }
                }
                else
                {
                        /* Set the flag to zero if the queue was empty 
                         * for blocking the thread*/
                        set_thread_flag (0);
                }
        }
}
