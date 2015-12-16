#include "socket_header.h"

/*Client requesting the server for the file*/
int request (int client_fd, char * file_name, char * dest)
{
        int ret         =       -1;
        int len         =       0;
        int success      =       0;
        int exit_var    =       1;
        
        len = strlen (file_name) + 1;
        ret = write (client_fd, &len, sizeof(len));
        
        if (ret != 0)
        {
            do{
                ret = read (client_fd, &success, sizeof(success));
                if(ret != 0)
                {
                    switch(success)
                    {
                        /*If file not exist in server*/
                        case ENOENT:
                                    fprintf (stderr, "\tERROR: %s\n", strerror(ENOENT));
                                    ret = -1;
                                    exit_var = 0;
                                    goto out;
                                    break;
                        /*If File read fail */
                        case read_fail:
                                    printf ("\tERROR: Read Fail\n");
                                    exit_var = 0;
                                    ret = -1;
                                    goto out;
                                    break;
                        /*Send the acknowledgment to server that it is ready to 
                         * recieve the file*/
                        case s_accept:
                                    ret = write (client_fd, file_name, len);
                                    break;
                        /*If file exists it will recieve the file from the 
                         * server*/ 
                        case file_exist:
                                    ret = get_doc_from_server(client_fd,
                                                            file_name, dest);
                                    if(ret < 0)
                                    {
                                        printf ("\tError in saving the file\n");
                                        goto out;
                                    }
                                    printf ("\tFile Successfully saved\n");
                                    exit_var = 0;
                                    break;
                    }
                }
            }while (exit_var == 1);
            
        }
        
        ret = 0;
        
        out:
            return ret;
        
}
