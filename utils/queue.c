#include "queue.h"
/*
 * queue_new () : Function creates a new queue which initializes the value of 
 * mutex, head and tail to null and max specifies the maximum number 
 * of request this queue contains, it is initialized to zero.
 * 
 * Output:
 *      queue_t obj     :       reference for the initalized queue.
 * */
queue_t* queue_new ()
{
        /* Allocating the memory and initialize the values*/
        queue_t* obj = (queue_t*) calloc (1, sizeof (queue_t));
        pthread_mutex_init (&obj -> queue_mutex, NULL);
        obj -> head = NULL;
        obj -> tail = NULL;
        obj -> max = 0;
        return obj;
}

/*
 * Queue_full () : function which checks whether the queue is full or not.
 * Input:
 *      queue_t* arg    :       pointer to the queue.
 *Output:
 *      int ret         :       If queue is full it returns 1, otherwise 0;
 * */
int queue_full (queue_t* arg)
{
        int ret = 0;
        if (arg -> max >= 1024)
        {
                ret = 1;
        }
        return ret;
}

/*
 * Queue_empty () : function which checks whether the queue is empty or not.
 * Input:
 *      queue_t* arg    :       pointer to the queue.
 *Output:
 *      int ret         :       If queue is empty it returns 1, otherwise 0.
 * */
int queue_empty (queue_t* arg)
{
        int ret = 0;
        if (arg -> max <= 0)
        {
                ret = 1;
        }
        return ret;
}

/*
 * queue_put () : This function which insert data to the queue.
 * Input:
 *      queue_t obj_queue       :       pointer to the queue.
 *      void* arg               :       Specifies the data to be stored
 *                                      in the queue.
 * Output:
 *      int                     :       Specifies the successful insertion.
 * */
int queue_put (queue_t* obj_queue , void* arg)
{
        int ret         =        -1;
        q_node_t* obj_q_node;

        /* Allocate the new node */
        obj_q_node = (q_node_t*) calloc (1, sizeof (q_node_t));
        obj_q_node -> c_request = arg;
        obj_q_node -> next = NULL;

        /*Lock the mutex*/
        pthread_mutex_lock (&obj_queue -> queue_mutex);

        /* If Queue is empty */
        if (obj_queue -> head == NULL && obj_queue -> tail == NULL)
        {
                obj_queue -> head = obj_queue -> tail = obj_q_node;
                obj_queue -> max++;
        }
        else
        {
                obj_queue -> tail -> next = obj_q_node;
                obj_queue -> tail = obj_q_node;
                obj_queue -> max++;
        }

        /* Unlock the mutex */
        pthread_mutex_unlock (&obj_queue -> queue_mutex);
        printf ("\t Inserted\n");
        ret = 0;
        out:
                return ret;
}
/*
 * queue_get (): This function removes the  data to the queue.
 * Input:
 *      queue_t obj_queue       :       pointer to the queue.
 *      queue_cbk_t arg         :       Specifies the calback function pointer.
 * Output:
 *      int                     :       Specifies the successful removal of data.
 * */
int queue_get (queue_t* obj_queue, queue_cbk_t arg)
{
        int ret         =        -1;
        void* data;
        q_node_t* node;
        
        /* Lock the mutex */
        pthread_mutex_lock(&obj_queue -> queue_mutex);
        if (obj_queue -> head == NULL && obj_queue -> tail == NULL)
        {
                ret = -1;
                pthread_mutex_unlock (&obj_queue -> queue_mutex);
                goto out;
        }
        
        if (obj_queue -> head == obj_queue -> tail && obj_queue -> head != NULL)
        {
                node = obj_queue -> head;
                obj_queue -> head = NULL;
                obj_queue -> tail = NULL;
                obj_queue -> max--;
        }
        else
        {
                node = obj_queue -> head;
                obj_queue -> head = obj_queue -> head -> next;
                obj_queue -> max--;
        }
        /* Unlock the mutex */
        pthread_mutex_unlock (&obj_queue -> queue_mutex);
        
        data = node -> c_request;
        
        ret = arg (data);
        
        free (node);

        out:
                return ret;
}