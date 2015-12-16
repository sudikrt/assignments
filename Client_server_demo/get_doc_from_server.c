#include "socket_header.h"

/*This will recieve the file from the server*/
int get_doc_from_server (int client_fd, char * file_name, char * dest)
{
        int ret                     =       -1;
        int number_of_bytes_read    =       0;
        int fd;
        char * buffer;
        
        buffer = (char*) malloc (1000);
        
        ret = chdir(dest);
        if(ret == -1)
        {
            fprintf (stderr,strerror(errno));
            goto out;
        }
        
        fd = open(file_name, O_WRONLY | O_CREAT);
        if(fd == -1)
        {
            fprintf (stderr, "\t%s\n", strerror(errno));
            ret = -1;
            goto out;
        }
         
        printf ("\tSaving the file...\n");
        /*Reads from the server and writes to file*/
        while (1)
        {
            number_of_bytes_read = read (client_fd, buffer, 1000);
            if (number_of_bytes_read == 0)
            {
                break;
            }
            ret = write (fd, buffer, number_of_bytes_read);
            if (ret <= 0)
            {
                ret = -1;
                fprintf (stderr, "\tERROR: %s\n", strerror(errno));
                goto out;
            }
        }
        
        printf ("\tFile Successfully saved\n");
        
        out:
            if (fd >= 0) {
                close (fd);
            }
            free (buffer);
            return ret;
}
