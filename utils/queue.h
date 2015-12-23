#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/*
 * Structure to hold data;
 * */
typedef struct queue_node
{
        void* c_request;
        struct queue_node* next;
} q_node_t;

/*
 * Structure for the queue;
 * */
typedef struct queue
{
        pthread_mutex_t queue_mutex;
        int max;
        q_node_t* head;
        q_node_t* tail;
} queue_t;

/*
 * Decleration of call back function;
 * */
typedef int (*queue_cbk) (void *) queue_cbk_t ;




