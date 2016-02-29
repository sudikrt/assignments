#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h> 
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>

/* Structure of the thread_pool */
typedef struct thread_pool_t {
        /* Number of threads*/
        int num_of_threads;

        /* Number of active threads */
        int num_threads_working;
        
        /* pointer to the threads */
        pthread_t *threads;
        
        /* Condition Variable for thread */
        pthread_cond_t cond_t;
        
        /*Mutex lock variable */
        pthread_mutex_t q_lock;

        /* pointer to list */
        list_t *list;

} thread_pool_t;

/* Creating thread pool */
thread_pool_t * create_thread_pool(int num_threads);

/* Destroying thread pool */
void destroy_threadpool(thread_pool_t *thread_pool_obj);

/* Do the work */
void* do_work(void *pool_obj);
