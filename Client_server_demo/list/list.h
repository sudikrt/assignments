#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/*
 * Structure to hold data;
 * */
typedef struct list_node
{
        void *data;
        struct list_node *next;
        struct list_node *prev;
} l_node_t;

/*
 * Structure for the list;
 * */
typedef struct list
{
        pthread_mutex_t list_mutex;
        int max;
        l_node_t *first_node;
} list_t;

typedef char boolean_t;

/* Call back function */
typedef
boolean_t (*cbk_fun_t) (void *, void *);

/* function for Matching the arguments */
boolean_t match_arg_list (void *, void *);

/* Function for creating the new list */
list_t *
list_new ();

/* Function for free the queue */
void
list_free (list_t *list_obj);

/*Function for inserting data into the list */
l_node_t *
insert_into_list (list_t *list_obj, void *arg);

/*Function for removing data from the queue */
int
delete_from_list (list_t *list_obj, void *arg);

/* Function to search in the list */
l_node_t *
search_in_list (list_t *list_obj, cbk_fun_t match_function, void *arg);




