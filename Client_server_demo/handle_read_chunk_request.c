#include "socket_header.h"

void
handle_read_chunk_request (int client_fd) {
        int buf_ptr     =        0;
        char* rec_buf;
        char* buf;
        int fd;
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
        
        if (ret == fail_response) {
                /* Copy the type of the fail response */
                memcpy (&ret, rec_buf + buf_ptr, sizeof (int));
                buf_ptr += sizeof (int);
                
                switch (ret) {
                        case insufficient_arg :
                                        printf ("ERROR: Insufficient argument\n");
                                        break;
                        case file_open_error :
                                        printf ("ERROR: File Open/read error \n");
                                        break;
                        case file_already_closed :
                                        printf ("ERROR: File is closed\n");
                                        break;
                }
        }
        else
        {
                /* Copy the length of bytes to read */
                memcpy (&ret, rec_buf + buf_ptr, sizeof (int));
                buf_ptr += sizeof (int);
                
                buf = (char*) calloc (1, ret);
                memcpy (buf, rec_buf + buf_ptr, ret);
                buf_ptr += ret;
                
                fd = open("read_file", O_WRONLY | O_CREAT);
                if(fd == -1)
                {
                        fprintf (stderr, "\t%s\n", strerror(errno));
                        ret = -1;
                        goto out;
                }
                
                ret = write (fd, buf, ret);
                if (ret < 0)
                {
                        ret = -1;
                        fprintf (stderr, "\tERROR: %s\n", strerror(errno));
                        goto out;
                }
        }
out:
        return;
}