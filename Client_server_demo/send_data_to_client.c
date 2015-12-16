#include "socket_header.h"


/*This will send the file to client*/
int send_data_to_client (int client_fd, char * file_name, char * p_dir)
{
    int ret = -1;
    int fd;
    char * buffer;
    int number_of_bytes = 0;
    
    
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
    
    printf ("\tSending\n");
    /*It will write the file data to socket and send it to client*/
    while(1)
    {
        number_of_bytes = read (fd, buffer, 1000);
        if(number_of_bytes == 0)
        {
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
    
    ret = 0;
    out:
        if (fd >= 0) {
            close (fd);
        }
        free (buffer);
        return ret;
    
}
