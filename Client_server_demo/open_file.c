#include "socket_header.h"

/*
 * newNode () :
 *              Which allocates the memory for storng the file_fd,
 *              file_name, and for the new_node;
 * Output:
 *      fd_data_t* obj:
 *                      on allocation of memory its reference is returned.
 * */
fd_data_t*
newNode () {

        fd_data_t* obj;
        /* Allocate the memory. */
        obj = (fd_data_t*) calloc (1, sizeof (fd_data_t));
        obj -> file_name = (char*) calloc (1, 1024);
        obj -> next = NULL;

        return obj;
}


/*
 * check_isOpen ():
 *              This fuction check whether the given file is already open or not
 *Input:
 *      char* file_name         :       Specifies the file name;
 *      unsigned int mode       :       Specifies the mode at which it will open.
 * Output:
 *      int ret                 :       0 - Specifies the file already open.
 *                                      otherwise -1;
 * 
 * */
int
check_isOpen (char* file_name, unsigned int mode, int* file_desc) {
        
        int ret = -1;
        fd_data_t* temp;
        
        /*Traverse the queue untill the queue is null or the file exist. */
        for (temp = fd_node; temp != NULL; temp = temp -> next) {

                if (strcmp (temp -> file_name, file_name) == 0 &&
                                temp -> mode == mode) {
                        *file_desc = temp -> fd;
                        ret = 0;
                        goto out;
                }
        }

        out:
                return ret;
}

/*
 * open_file ():
 *              This function opens the named file and assign the fd to the
 *              file_desc variable on successful opening.and add this info.
 *              to the queue;
 * Input:
 *      char* file_name         :       Specifies the name of the file;
 *      unsigned int mode       :       Specifies the mode at which the file 
 *                                      should open;
 *      int* file_desc          :       contains the file descriptor on successful
 *                                      opening of the file which will return 
 *                                      back to the client.
 *Output:
 *      int ret                 :       Specifies the operation is success or
 *                                      failure;
 * */
int 
open_file (char* file_name, int* desc, unsigned int mode) {

        int ret = -1;
        int fd;
        fd_data_t* temp;
        fd_data_t* index;

        /* Determines whether the file is opened or not */
        if (check_isOpen (file_name, mode, desc) == 0) {
                /* Already opened then ..*/
                ret = file_already_open;
                goto out;
        }
        else {
                /* Check file is exist */
                ret = check_file_exist(file_name);
                if (ret != file_exist)
                {
                        ret = ENOENT;
                        goto out;
                }

                /* If the file is not already opened*/
                if (mode == read_mode) {
                        /* Openes the file in read mode. */
                        fd = open (file_name, O_RDONLY);
                        if (fd < 0) {
                                ret = file_open_error;
                                goto out;
                        }
                }
                else {
                        /* Openes the file in write mode */
                        fd = open (file_name, O_WRONLY);
                        if (fd < 0) {
                                ret = file_open_error;
                                goto out;
                        }
                }

                /* Create the new node and assign the file_name, fd to it. */
                temp = newNode ();
                temp -> fd = fd;
                temp -> file_name = file_name;
                temp -> mode = mode;
                temp -> next = NULL;
                /* Check if the queue is empty */
                if (fd_node == NULL) {
                        fd_node = temp;
                        ret = 0;
                        *desc = fd;
                        goto out;
                }
                /* If the queue is not empty then the traverse through it 
                 * and insert the new node at the end.*/
                for (index = fd_node; index -> next != NULL; index = index -> next);
                index -> next = temp;
                desc = &fd;
        }
        ret = 0;
        out:
                return ret;
}

/*
 * check_isNotClosed ():
 *              This function checks whether the file is closed or not
 * Inputs:
 *      char* file_name         :       Specifies the name of the file.
 *      unsigned int file_desc  :       Specifies the file descriptor.
 * Output:
 *      int ret                 :       0-Specifies the file is not closed 
 *                                      otherwise -1;
 * */
int 
check_isNotClosed (int* file_desc) {
        int ret = -1;
        fd_data_t* temp;
        
        for (temp = fd_node; temp != NULL; temp = temp -> next) {

                if (temp -> fd == *file_desc) {
                        ret = 0;
                        goto out;
                }
        }
        out:
                return ret;

}

/*
 * close_fd ():
 *              This function close the opened file descriptor
                and deallocate the memory allocated in the queue.
 * Input:
 *      char* file_name         :       Specifies the name of the file.
 *      unsigned int file_desc  :       Specifies the file descriptor.
 * 
 * */
void 
close_fd (int* file_desc) {
        
        fd_data_t* temp;
        while (fd_node -> next != NULL && (fd_node -> next) -> fd != *file_desc)
        {
                fd_node = fd_node -> next;
        }
        if (fd_node == NULL) {
                goto out;
        }
        if (fd_node -> next == NULL) {
                close (fd_node -> fd);
                free (fd_node -> file_name);
                fd_node = NULL;
                goto out;
        }
        temp = fd_node -> next;
        
        fd_node -> next = temp -> next;

        close (temp -> fd);
        free (temp -> file_name);
        free (temp);
out:
        return;
}

/*
 * close_file ():
 *              This function checks whether the file is opened if opened then
 *              closes it.
 * Inputs:
 *      char* file_name         :       Specifies the name of the file.
 *      unsigned int file_desc  :       Specifies the file descriptor.
 * Output:
 *      int ret                 :       Specifies the successful closing of 
 *                                      the file.
 * */
int 
close_file (int* file_desc) {

        int ret = -1;

        if (check_isNotClosed (file_desc) == 0) {

                close_fd (file_desc);
                ret = 0;
                goto out;
        }

        ret = file_already_closed;
        
        out:
                return ret;
}