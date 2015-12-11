#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int make_server (int client_socket)
{
    int length;
    char* text;
    char buffer[1000];
    char* file_name;
    int number_of_bytes;
    int ret = -1;
    int fd;
    struct stat st;
    
    if (read (client_socket, &length, sizeof (length)) == 0)
                        return 0;
    text = (char*) malloc (length);
    
    read (client_socket, text, length);
    strcpy (file_name, text);
    printf ("%s", text);
    
    ret = chdir("/home/sudarshan/");
    if(ret == -1)
    {
        printf ("Error in changing directory\n");
        //goto out;   
    }

    
    ret = stat(text, &st);
    //check for the existance
    if(ret == -1)
    {
        //send the file to client
        strcpy (text, "Failure");
        write(client_socket, text, sizeof(text));
        return 1;
    }
        strcpy (text, "Succes");
        write(client_socket, text, sizeof(text));
        fd = open(file_name, O_RDWR);
        if(fd == -1)
        {
            printf ("Error in creating/reading file\n");
            return ;
        }
        while ((number_of_bytes = read (fd, buffer, 1000)) != 0)
        {
                write(client_socket, buffer, 1000);
        }
        free (text);
    return 0;
    
} 
int main (int argc, char* argv [])
{
    const char* const socket_name = argv[1];
    int socket_fd;
    struct sockaddr_un name;
    int succes;
    
    socket_fd = socket (PF_LOCAL, SOCK_STREAM, 0);
    
    name.sun_family = AF_LOCAL;
    strcpy (name.sun_path, socket_name);
    bind (socket_fd, (struct sockaddr*) &name, SUN_LEN (&name));
    
    listen (socket_fd, 5);
    
        struct sockaddr_un client_name;
        socklen_t client_name_len;
        int client_sock_fd;
        
        client_sock_fd = accept (socket_fd, (struct sockaddr*) &client_name, &client_name_len);
        
        succes = make_server (client_sock_fd);
        
        close (client_sock_fd);
        
    close (socket_fd);
    unlink (socket_name);
    
    return 0;
}