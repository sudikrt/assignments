#include "socket_header.h"
/*
 * this is the main fuction where client starts and initializes the socket
 * for the connection
 * Input :
 *      Inputs are given through command line in which 
 *      -p      :       Specifies the port number.
 *      -s      :       Specifies the host name.
 *      -d      :       Specifies the export directory where the recieved files
 *                      are stored.
 * Output:
 *      int     :       return value.
 * */
int main (int argc, char * argv [])
{
        int ret             =       -1;
        int socket_fd       =       0;
        int port            =       0;
        char * server_name;
        char * destination;
        char * file_name;
        int next_option;
        struct sockaddr_in server_addr;
        
        /*Command line options*/
        const char* const short_option = "s:p:f:d:";

        const struct option long_option [] = {
                { "server",     1, NULL, 's'},
                { "port",       1, NULL, 'p'},
                { "file",       1, NULL, 'f'},
                { "destination",1, NULL, 'd'},
                { "NULL",       0, NULL,  0 } 
        };
        do
        {
                next_option = getopt_long (argc, argv, short_option, 
                                                            long_option, NULL);
                
                switch (next_option)
                {
                        case 'f':
                                file_name = (char*) malloc (strlen(optarg + 1));
                                strcpy (file_name, optarg);
                                break;
                        case 'p':
                                port = atoi (optarg);
                                break;
                        case 's':
                                server_name = (char*) malloc (strlen (optarg + 1));
                                strcpy (server_name, optarg);
                                break;
                        case 'd':
                                dest_dir = (char*) malloc (strlen (optarg + 1));
                                strcpy (dest_dir, optarg);
                                break;
                        case -1:
                                break;
                        default:
                                abort ();
                }
        }while (next_option != -1);

        /*Client request for the particular named file*/
        ret = request (port, server_name);
        
        ret = 0;
        out:
                return ret;
}
