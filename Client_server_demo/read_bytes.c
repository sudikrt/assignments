#include "socket_header.h"

/*
 * This function check whether the file is opened or not.
 * Input:
 *      int fd                  :       Specifies the file descriptior to be 
 *                                      checked.
 *      fd_data_t* fd_root      :       A reference to the queue containing the 
 *                                      list of opened fd.
 * Output:
 *      int ret                 :       if the file exist then 0 will be returned
 *                                      otherwise -1;
 * */
int
isOpen (int fd)
{
        int ret = -1;
        fd_data_t* temp;
        /*Traverse the queue untill the queue is null or the file exist. */
        for (temp = fd_node; temp != NULL; temp = temp -> next) {

                if (temp -> fd == fd) {
                        ret = 0;
                        goto out;
                }
        }
out:    
        return ret;
}
/*
 * This function reads the number of bytes from the file.
 * Input:
 *      int fd          :       File_descriptor of the file to be read.
 *      char* buffer    :       The read content will be stored.
 *      ssize_t no_read :       Specifies the number of bytes to read.
 * Output:
 *      ssize_t         :       If the read successful then no. of bytes read
 *                              will be returned otherwise the -1 will returned.
 * */
ssize_t
read_bytes (int fd, char* buffer, int* no_bytes)
{
        int ret = -1;
        int bytes_read = *no_bytes;
        
        /* Check whether the file is opened or not */
        if (isOpen (fd) != 0) 
        {
                ret = file_already_closed;
                goto out;
        }
        
        /* Read no_bytes to buffer */
        *no_bytes = read (fd, buffer, bytes_read);
        
        /* Check for the bytes_read */
        if (*no_bytes <= 0) 
        {
                goto out;
        }
        ret = 0;
out:
        return ret;
}