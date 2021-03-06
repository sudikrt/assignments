#include "socket_header.h"

/*
 * This will recieve the file from the server
 * Input:
 *      int client_fd   :       Specifies the connection identifier.
 *      char* file_name :       Specifies the name of the file to be recieved.
 * Output:
 *      int             :       Specifies the Success or failure.
 * */
int get_doc_from_server (int client_fd, char* file_name, ssize_t len)
{
        int ret                         =       -1;
        int number_of_bytes_read        =       0;
        int fd;
        char* buffer;
        struct stat stat_var;

        /* Allocating the memory. */
        buffer = (char*) calloc (1,1024);
        memset (buffer, 0, 1024);
        
        /* Changes the current directory to the user defined export 
         * directory. */
        ret = chdir(dest_dir);
        if(ret == -1)
        {
                fprintf (stderr,strerror(errno));
                goto out;
        }

        /* Opens the file for writing the recieved data. */
        fd = open(file_name, O_TRUNC | O_WRONLY | O_CREAT);
        if(fd == -1)
        {
                fprintf (stderr, "\t%s\n", strerror(errno));
                ret = -1;
                goto out;
        }
        
        printf ("\tSaving the file...\n");
        while (1)
        {
                number_of_bytes_read = read (client_fd, buffer, 1024);
                if (number_of_bytes_read < 0)
                {
                        ret = -1;
                        break;
                }
                ret = write (fd, buffer, number_of_bytes_read);
                if (ret < 0)
                {
                        ret = -1;
                        fprintf (stderr, "\tERROR: %s\n", strerror(errno));
                        goto out;
                }
                ret = stat(file_name, &stat_var);

                if(stat_var.st_size == len || number_of_bytes_read == 0)
                {
                        ret = write (fd, buffer, number_of_bytes_read);
                        ret = 0;
                        break;
                }
        }
        
        
        out:
            if (fd >= 0) {
                    close (fd);
            }
            free (buffer);
            return ret;
}
