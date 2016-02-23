#include "list.h"

/*
 * Function creates a new list which initializes the value of mutex and
 * head and tail to NULL
 * Output:
 *      list_t* list_obj        :       reference for the initialized queue
 * */
list_t *
list_new () {
        /* Allocate the memory and initialize the values */
        list_t *list_obj = (list_t *) calloc (1, sizeof (list_t));
        pthread_mutex_init (&list_obj -> list_mutex, NULL);
        
        list_obj -> first_node = (l_node_t*) calloc (1, sizeof (l_node_t));
        list_obj -> first_node-> data = (void*) calloc (1, sizeof (void));
        list_obj -> first_node = NULL;
        
        list_obj -> max = 0;
out:
        return list_obj;
}

/*
 * This function which insert data to the list.
 * Input:
 *      list_t* list_obj        :       Pointer to the list.
 *      void* arg               :       Specifies the data to be stored 
 *                                      in the list.
 * Output:
 *      int                     :       specifies the successful insertion.
 * */
l_node_t*
insert_into_list (list_t *list_obj, void *arg) {
        int ret         =       -1;
        l_node_t *obj_node;

        /* Allocate the memory */
        obj_node = (l_node_t *) calloc (1, sizeof (l_node_t));
        obj_node -> data = (void *) calloc (1, sizeof (arg));
        obj_node -> data = arg;
        obj_node -> next = NULL;
        obj_node -> prev = NULL;

        /* Aquire the lock */
        pthread_mutex_lock (&list_obj -> list_mutex);

        /* If the list is empty */
        if (list_obj -> first_node == NULL) {
                list_obj -> first_node = obj_node;
        }
        else {
                obj_node -> prev = list_obj -> first_node -> prev;
                list_obj -> first_node -> prev = obj_node;
                obj_node -> next = list_obj -> first_node;
                list_obj -> first_node = obj_node;
        }
        list_obj -> max ++;
        
        /* Release the lock */
        pthread_mutex_unlock (&list_obj -> list_mutex);
        
out:
        return list_obj -> first_node;
}

/* Function to delete from list */
int
delete_from_list (list_t *list_obj, void *data) {

        l_node_t *ptr;
        int ret = -1;
        ptr = list_obj -> first_node;
        
        while (ptr -> data != data && ptr -> next != NULL) {
                ptr = ptr -> next;
        }
        if (ptr -> next == NULL && ptr -> data != data) {
                printf ("\tERROR: No Match Found\n");
                ret = -1;
                goto out;
        }
        
        /* Aquire the lock */
        pthread_mutex_lock (&list_obj -> list_mutex);
        
        if (ptr == list_obj -> first_node) {
                list_obj -> first_node = ptr -> next;
        }
        
        if (ptr -> next != NULL) {
                ptr -> next -> prev = ptr -> next;
        }
        
        if (ptr -> prev != NULL) {
                ptr -> prev -> next = ptr -> next;
        }
        list_obj -> max --;
        
        /* Release the lock */
        pthread_mutex_unlock (&list_obj -> list_mutex);
        free (ptr);
        ret = 0;
out:
        return ret;
}

/*
 * Function is to search from the list for the particular arg
 * if found then that particulat calback function will be called.
 * Input:
 *      list_t *list_obj        :       A reference to the list;
 *      cbk_fun_t match_function :      The calback function;
 *      void *arg               :       The argument to the call back 
 *                                      function as well as matching parameter.
 * Output:
 *      l_node_t *              :       A reference to the matched argument in 
 *                                      the list.
 * 
 * */
l_node_t *
search_in_list (list_t *list_obj, cbk_fun_t match_function, void *arg) {
        
        l_node_t *ret   =       NULL;
        
        boolean_t var;
        
        /* Assign the first link to the temp */
        l_node_t *temp  =       list_obj -> first_node;
        
        /* If the list is empty */
        if (temp == NULL) {
                goto out;
        }
        
        /* Assign the function name */
        match_function = match_arg_list;


        while (temp -> next == NULL) {
                /* If arg matches */
                if (match_function (temp -> data, arg) == 1) {
                        ret = temp;
                        goto out;
                }
                temp = temp -> next;
        }

        if (temp -> next == NULL) {
                ret = NULL;
                goto out;
        }
out:
        return ret;
}

/* 
 * The function to free the list 
 * Input:
 *      list_t *list_obj        :       A reference to the list
 * */
void
list_free (list_t *list_obj) {
        int index;
        l_node_t *temp;

        if (list_obj != NULL) {
                for (index = 0; index < list_obj -> max; index ++) {
                        temp = list_obj -> first_node;
                        list_obj -> first_node = temp -> next;

                        if (temp != NULL) {
                                free (temp -> data);
                                free (temp);
                        }
                }
                if (list_obj != NULL) {
                        free (list_obj);
                }
        }
out:
        return;
}

/* 
 * Function to check the argument whether two arguments are same or not 
 * Input:
 *      void *arg1
 *      void *arg2      :       Specifies the argument to be checked.
 * Output:
 *      boolean_t       :       returns 1 if match found 0 otherwise.
 * */
boolean_t
match_arg_list (void *arg1, void *arg2) {
        int ret = 0;
out:
        return ret;
}

void disp (list_t *list_obj) {
        
        int i;
        l_node_t *temp = list_obj -> first_node;
        
        if (temp == NULL) {
                printf ("List is empty\n");
                goto out;
        }
        
        for (i = 0; i < list_obj -> max; i++) {
                printf ("%s\n", (char*) temp -> data);
                temp = temp -> next;
        }
out:
        return;
}