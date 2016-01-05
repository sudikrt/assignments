#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/*
 * Structure to hold data;
 * */
struct queue_node
{
        void* c_request;
        struct queue_node* next;
}; 
typedef struct queue_node q_node_t;

pthread_mutex_t queue_mutex;
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


typedef struct c_r
{
        int type;
        int client_fd;
        char* buf;
        void (*operation) (void*, int);
} C_R;
/*
 * Decleration of call back function;
 * */
typedef int (*queue_cbk_t) (void *) ;

/*Function for new queue */
queue_t* queue_new();

/* Function to check the queue is full or not */
int queue_full (queue_t* arg);

/* Function to check the queue is empty or not */
int queue_empty (queue_t* arg);

/* Function to put data to the queue */
int queue_put (queue_t* obj_queue , void* arg);

/* Function to get the data from the queue */
int queue_get (queue_t* obj_queue, queue_cbk_t arg);

void node_free (q_node_t* node);




