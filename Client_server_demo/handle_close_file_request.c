#include "socket_header.h"

/*
 * This function will perform the close file request.
 * Input:
 *      int client_fd   :       Specifing the connection.
 * */
void 
handle_close_file_request (int client_fd) {
        
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
        
        /* Check the return value for the successful operation */
        if (ret == success_response) {
                printf ("\tOperation Successful \n");
        }
        else if (ret == fail_response) {
                printf ("\tOpreation unsuccessful\n");
        }

out:
        return;

}