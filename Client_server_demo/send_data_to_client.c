#include "socket_header.h"
#include <signal.h>

ssize_t calc_file_size_response () {
        ssize_t len     =       0;
        
        /* Size of the response type */
        len = sizeof (int);
        
        /* Size of return value */
        len += sizeof (ssize_t);
        
        /* size of the footer */
        len += sizeof (int);

out:
        return len;
}

/* Handling the signal */
int sig = 0;
int new_ret = 0;
/* Signal Handler fuction which will be invoked when the client 
 * closes the connection abruptly
 * Input :
 *      int signal_numebr       : specifies the signal number
 * */
void handler (int signal_number)
{
        new_ret = -1;
        close(sig);
        return ;
}

/*This will send the file to client
 * Input:
 *      int client_fd   :       Specifies the connection identifier
 *      char* file_name :       Specifies the file_name that is sent to client
 * 
 * Output:
 *      int             :       Specifing success or failure
 * */
int send_data_to_client (int client_fd, char * file_name)
{
        int ret         =       -1;
        int fd;
        ssize_t len     =       0;
        int buf_ptr     =       0;
        char * buffer;
        int number_of_bytes = 0;
        struct stat stat_var;
        
        struct sigaction sa;
        memset (&sa, 0, sizeof (sa));
        sa.sa_handler = &handler;
        
        /* Assigning the client_fd tp sig to handle the signal gracefully */
        sig = client_fd;

        /* Change the current directory to the export directory */
        ret = chdir(exp_dir);
        if (ret == -1)
        {
                fprintf (stderr, "\tERROR: %s\n", strerror(errno));
                ret = file_not_exist;
                goto out;   
        }

        /* Opens the file in read_only mode */
        fd = open(file_name, O_RDONLY);
        if(fd == -1)
        {
                fprintf (stderr, "\tERROR: %s\n", strerror(errno));
                ret = -1;
                goto out;
        }
        
        /*Waiting for the signal*/
        sigaction (SIGPIPE, &sa, NULL);

        /* Read the size of the file and send it to the client */
        ret = stat(file_name, &stat_var);
        
        /* Calculate the length of the response */
        len = calc_file_size_response ();
        
        /* Allocate the memory */
        buffer = (char*) calloc (1, len + sizeof (int));
        
        /* Copy the length of the response */
        memcpy (buffer, &len, sizeof (int));
        buf_ptr += sizeof (int);
        
        len = success_response;
        memcpy (buffer + buf_ptr, &len, sizeof (int));
        buf_ptr += sizeof (int);
        
        len = stat_var.st_size;
        memcpy (buffer + buf_ptr, &len, sizeof (ssize_t));
        buf_ptr += sizeof (ssize_t);
        
        len = 0xbadf00d;
        memcpy (buffer + buf_ptr, &len, sizeof (int));
        buf_ptr += sizeof (int);

        ret = write (client_fd, buffer, buf_ptr);
        if (ret < 0)
        {
                ret = -1;
                fprintf (stderr, "\tERROR: %s\n", strerror(errno));
                goto out;
        }
        
        buffer = (char*) calloc (1,1024);
        memset (buffer, 0, 1024);
        
        printf ("\tSending file to client       : %d\n", client_fd);
        /*It will write the file data to socket and send it to client*/
        while(1)
        {
                
                number_of_bytes = read (fd, buffer, 1024);
                if (number_of_bytes < 0) {
                        ret = -1;
                        fprintf (stderr, "\tERROR: %s\n", strerror(errno));
                        goto out;
                }
                number_of_bytes = write (client_fd, buffer, number_of_bytes);
                if (number_of_bytes < 0)
                {
                        ret = -1;
                        fprintf (stderr, "\tERROR: %s\n", strerror(errno));
                        goto out;
                }
                if (number_of_bytes == 0)
                {
                        break;
                }
        }
        
        /*If Signal occures set the ret value to -1
         * for further processing*/
        if(new_ret < 0)
        {
                ret = -1;
                new_ret = 0;
                goto out;
        }
        
        ret = 0;
        out:
            if (fd >= 0) {
                    close (fd);
            }
            return ret;
}
