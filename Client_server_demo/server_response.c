#include "socket_header.h"

int server_response (int client_fd, char * p_dir)
{
    int ret         =       -1;
    int code        =       0;
    int len         =       0;
    char * file_name;
    
    ret = read (client_fd, &code, sizeof (code));
    if(ret != 0)
    {
        len = code;
        code = s_accept;
        ret = write (client_fd, &code, sizeof(code));
        
        file_name = (char*)malloc(len);
        
        /*Read the name of the file*/
        ret = read (client_fd, file_name, len);
        if (ret < 0)
        {
            ret = -1;
            fprintf (stderr, "\tERROR: %s\n", strerror(errno));
            goto out;
        }
        
        /*Check the existance of the file*/
        ret = check_file_exist(file_name, p_dir);
        
        /*If File exist*/
        if (ret == file_exist)
        {
            code = file_exist;
            /* Sends the conformation to the client that server is ready 
             * to send the file*/
            ret = write (client_fd, &code, sizeof(code));
            if(ret < 0)
            {
                ret = -1;
                fprintf (stderr, "\tERROR: %s\n", strerror(errno));
                goto out;
            }
            
            /*Send the file to client*/
            ret = send_data_to_client (client_fd, file_name, p_dir);
            if(ret < 0)
            {
                ret = -1;
                goto out;
            }
            printf("\tFinished sending\n");
            
            /*Closes the client connection*/
            close (client_fd);
                        
            ret = 0;
            goto out;
        }
        else
        {
            /* file not exists send the error message*/
            code = ENOENT;
            ret = write (client_fd, &code, sizeof(code));
            if(ret < 0)
            {
                ret = -1;
                fprintf (stderr, "\tERROR: %s\n", strerror(errno));
                goto out;
            }
            ret = code;
            goto out;
        }
    }
    ret = 0;
    
    out:
        return ret;
    
}