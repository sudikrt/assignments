#include "socket_header.h"
#include <signal.h>

/*Handling the signal*/
int sig = 0;
int new_ret = 0;

void handler (int signal_number)
{
    new_ret = -1;
    close(sig);
    return ;
}

/*This will send the file to client*/
int send_data_to_client (int client_fd, char * file_name, char * p_dir)
{
    int ret = -1;
    int fd;
    char * buffer;
    int number_of_bytes = 0;
    
    struct sigaction sa;
    memset (&sa, 0, sizeof (sa));
    sa.sa_handler = &handler;
    
    /*Assigning the client_fd tp sig to handle the signal gracefully*/
    sig = client_fd;
    
    buffer = (char *) malloc (1000);
    
    ret = chdir(p_dir);
    if (ret == -1)
    {
        fprintf (stderr, "\tERROR: %s\n", strerror(errno));
        ret = file_not_exist;
        goto out;   
    }
    
    fd = open(file_name, O_RDONLY);
    if(fd == -1)
    {
        fprintf (stderr, "\tERROR: %s\n", strerror(errno));
        ret = -1;
        goto out;
    }
    
    /*Waiting for the signal*/
    sigaction (SIGPIPE, &sa, NULL);
    
    printf ("\tSending\n");
    /*It will write the file data to socket and send it to client*/
    while(1)
    {
        number_of_bytes = read (fd, buffer, 1000);
        if(number_of_bytes == 0)
        {
            close (client_fd);
            break;
        }
        number_of_bytes = write (client_fd, buffer, number_of_bytes);
        
        if (number_of_bytes == 0)
        {
            ret = -1;
            fprintf (stderr, "\tERROR: %s\n", strerror(errno));
            goto out;
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
        free (buffer);
        return ret;
    
}
