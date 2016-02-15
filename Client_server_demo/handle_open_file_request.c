#include "socket_header.h"
/* 
 * This function is to handle the opening of the file request.
 * Input:
 *      int client_fd   :       specifies the connection identifier
 * */
void
handle_open_file_request (int client_fd) {
        
        int buf_ptr     =        0;
        char* rec_buf;
        int data        =       0;
        int ret         =       0;
        int len         =       0;
        
        /* Allocate the memory */
        rec_buf = (char*) calloc (1, 1024);
        
        /* Read the data from the socket */
        ret = read (client_fd, rec_buf, 1024);
        
        if (ret <= 0)
        {
                goto out;
        }
        
        /* Copy the size of the response */
        memcpy (&len, rec_buf, sizeof (int));
        buf_ptr += sizeof (int);

        /* Copy the type of the response */
        memcpy (&ret, rec_buf + buf_ptr, sizeof (int));
        buf_ptr += sizeof (int);
        
        /* Check the response type */
        if (ret == fail_response) {
                /* Copy the type of the fail response */
                memcpy (&ret, rec_buf + buf_ptr, sizeof (int));
                buf_ptr += sizeof (int);
                
                switch (ret) {
                        case insufficient_arg :
                                        printf ("ERROR: Insufficient argument\n");
                                        break;
                        case file_open_error :
                                        printf ("ERROR: File Open error \n");
                                        break;
                        case file_already_open :
                                        printf ("ERROR: File is already opened\n");
                                        
                                        /* Copy the file descriptor */
                                        memcpy (&ret, rec_buf + buf_ptr,
                                                sizeof (int));
                                        printf ("The fd is %d\n", ret);
                                        //file_desc = ret;
                                        buf_ptr += sizeof (int);
                                        break;
                        case ENOENT :
                                        printf ("ERROR: No such file or directory\n");
                                        break;
                }
        }
        else {
                /* Copy the file descriptor */
                memcpy (&ret, rec_buf + buf_ptr, sizeof (int));
                printf ("File Descriptor is %d\n", ret);
                buf_ptr += sizeof (int);
        }
out:
        return;
}