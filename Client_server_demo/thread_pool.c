#include "socket_header.h"


/*
 * function to create the thread_pool
 * Input:
 *      int num_threads         :       Spectifies the number of threads to be 
 *                                      created.
 * Output:
 *      reference of the thread_pool_t specifies that number of threads created.
 * */
void *
create_thread_pool (void* arg) {
        
        thread_pool_t *threadpool;
        thread_pool_t *ret;
        list_cbk_t cbk_obj;
        
        int num_threads = *((int*) arg);

        int index = 0;

        /* Check the number of threads */
        if (num_threads <= 0) {
                ret = NULL;
                goto out;
        }
        
        /* Create the thtread pool by allocating memory */
        threadpool = (thread_pool_t*) calloc (1, sizeof (thread_pool_t));
        if (threadpool == NULL) {
                fprintf (stderr, "\t%s  NO Much Memory to hold\n", 
                                                        strerror(errno));
                ret = NULL;
                goto out;
        }

        /* Allocate the memory for the thread id pointers */
        threadpool -> threads = (pthread_t*) calloc (num_threads, 
                                                        sizeof (pthread_t));
        
        /* Initialize the number of working thread */
        threadpool -> num_threads_working = 0;
        
        if (threadpool -> threads == NULL) {
                fprintf (stderr, "\t%s\t NO Much Memory to hold\n", 
                                                        strerror(errno));
                ret = NULL;
                goto out;
        }

        /* Assign the number of threads */
        threadpool -> num_of_threads = num_threads;

        /* Initialize the condition variables */
        if(pthread_cond_init( &(threadpool -> cond_t), NULL)) {
                fprintf (stderr, "Condition veriable Initiation not done!\n", 
                                                        strerror(errno));
                ret = NULL;
                goto out;
        }
        
        if (pthread_mutex_init (&threadpool -> q_lock, NULL)) {
                fprintf (stderr, "Mutex initialization error\n", 
                                                        strerror (errno));
                ret = NULL;
                goto out;
        }
        /* Initialize the list */
        threadpool -> list =  list_new ();

        for (index = 0; index < num_threads; index++) {
                if ( pthread_create ( &(threadpool -> threads [index]), NULL, 
                                                        &do_work, threadpool)) {
                        fprintf (stderr, "\t%s\tThread initiation error!\n", 
                                                        strerror(errno));
                        ret = NULL;
                        goto out;
                }
                
                threadpool -> num_threads_working ++;
        }

        ret = threadpool;

out:
        return (void*)ret;
}

/*
 * This function will deallocate the memory allocated for the thread pool
 * Input:
 *      threadpool *obj         :       Specifies the reference to the threadpool.
 * */
void destroy_threadpool(thread_pool_t *obj) {

        /* Free the memory allocated for threads */
        if (obj -> threads != NULL) {
                free (obj -> threads);
        }

        /* Free the list */
        list_free (obj -> list);

        /* Destroy the condition variables */
        pthread_cond_destroy ( &(obj -> cond_t));
        pthread_mutex_destroy( &(obj -> q_lock));

        /* Free the thread pool */
        if (obj != NULL) {
                free (obj);
        }
out:
        return;
}

/* Function which will do the work */
void* do_work(void *pool_obj) {
        
        list_cbk_t cbk_obj;
        thread_pool_t *pool = (thread_pool_t *) pool_obj;
        while (1) {
                pthread_mutex_lock (& (pool -> q_lock));
                while (pool -> list -> node_count == 0) {
                        /*
                         * Code for handling the work load
                         * Wait untill condition says there is a new request */
                        printf ("I m Sleeping ....\n");
                        pthread_cond_wait( & (pool -> cond_t),
                                                        & (pool -> q_lock));
                }
                /* Release the lock */
                pthread_mutex_unlock( &(pool->q_lock));

                printf ("\t Thread id %d is ...\t ", pthread_self());
                cbk_obj = handle_request;
                /* remove one request from the list and process it */
                if (pool -> list -> node_count) {
                        delete_from_list (pool -> list, cbk_obj);
                }
        }
}
