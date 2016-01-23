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
 *      fd_data_t* fd_node      :       A reference to the queue containing the
 *                                      fd list.
 * Output:
 *      int ret                 :       0 - Specifies the file already open.
 *                                      otherwise -1;
 * 
 * */
int
check_isOpen (char* file_name, unsigned int mode, fd_data_t* fd_node, 
                                                        int* file_desc) {
        
        int ret = -1;
        fd_data_t* temp;
        
        /*Traverse the queue untill the queue is null or the file exist. */
        for (temp = fd_node; temp != NULL; temp = temp -> next) {

                if (strcmp (temp -> file_name, file_name) == 0 &&
                                temp -> mode == mode) {
                        file_desc = temp -> fd;
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
 *      fd_data_t* fd_root      :       A reference to the queue containing the
 *                                      list of fd.
 *      int* file_desc          :       contains the file descriptor on successful
 *                                      opening of the file which will return 
 *                                      back to the client.
 *Output:
 *      int ret                 :       Specifies the operation is success or
 *                                      failure;
 * */
int 
open_file (char* file_name, unsigned int mode, fd_data_t* fd_root,
                                                int* file_desc) {

        int ret = -1;
        int fd;
        fd_data_t* temp;
        fd_data_t* index;

        /* Determines whether the file is opened or not */
        if (check_isOpen (file_name, mode, fd_root, file_desc) == 0) {
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
                                ret = errno;
                                goto out;
                        }
                }
                else {
                        /* Openes the file in write mode */
                        fd = open (file_name, O_WRONLY);
                        if (fd < 0) {
                                ret = errno;
                                goto out;
                        }
                }
                /* Create the new node and assign the file_name, fd to it. */
                temp = newNode ();
                temp -> fd = fd;
                temp -> file_name = file_name;
                temp -> mode = mode;
                temp -> next = null;
                /* Check if the queue is empty */
                if (fd_root == NULL) {
                        fd_root = temp;
                        ret = 0;
                        file_desc = fd;
                        goto out;
                }
                /* If the queue is not empty then the traverse through it 
                 * and insert the new node at the end.*/
                for (index = fd_node; index != NULL; index = index -> next)
                        ;
                index -> next = temp;
                file_desc = fd;
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
 *      fd_data_t* fd_root      :       A reference to the queue containing
 *                                      the list of fds.
 * Output:
 *      int ret                 :       0-Specifies the file is not closed 
 *                                      otherwise -1;
 * */
int 
check_isNotClosed (char* file_name, unsigned int file_desc, 
                                                        fd_data_t* fd_root) {
        int ret = -1;
        fd_data_t* temp;
        for (temp = fd_node; temp != NULL; temp = temp -> next) {

                if (strcmp (temp -> file_name, file_name) == 0 &&
                                temp -> fd == fd) {
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
 * Inputs:
 *      char* file_name         :       Specifies the name of the file.
 *      unsigned int file_desc  :       Specifies the file descriptor.
 *      fd_data_t* fd_root      :       A reference to the queue containing
 *                                      the list of fds.
 * 
 * */
void 
close_fd (char* file_name, unsigned int file_desc, fd_data_t* fd_root) {
        
        fd_data_t* temp;
        while (fd_root -> next != NULL && (fd_root -> next) -> fd != file_desc)
        {
                fd_root = fd_root -> next;
        }
        
        if (fd_root -> next == NULL) {
                return ;
        }
        temp = fd_root -> next;
        
        fd_root -> next = temp -> next;

        close (temp -> fd);
        free (temp -> file_name);
        free (temp);
        return;
}

/*
 * close_file ():
 *              This function checks whether the file is opened if opened then
 *              closes it.
 * Inputs:
 *      char* file_name         :       Specifies the name of the file.
 *      unsigned int file_desc  :       Specifies the file descriptor.
 *      fd_data_t* fd_root      :       A reference to the queue containing
 *                                      the list of fds.
 * Output:
 *      int ret                 :       Specifies the successful closing of 
 *                                      the file.
 * */
int 
close_file (char* file_name, unsigned int file_desc, fd_data_t* fd _root) {

        int ret = -1;

        if (check_isNotClosed (file_name, file_desc, fd_root) == 0) {

                close_fd (file_name, file_desc, fd_root);
                ret = 0;
                goto out;
        }

        ret = file_already_closed;
        
        out:
                return ret;
}