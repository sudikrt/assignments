#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
void write_text (int socket_fd, const char* text)
{

 int len = strlen (text) + 1; 
        write (socket_fd, &len, sizeof (len));
write (socket_fd, text, len);

}

void read_text (int socket_fd, const char* text1)
{
    char* text;
    int i;
    int fd;
    int ret;
    char* buffer;
    int number_of_bytes_read;
    text = (char*) malloc (100);
    buffer = (char*) malloc (100);
    i = read (socket_fd, text, sizeof(text));
    printf ("\n\t %s", text);
    if (strcmp(text, "Success") == 0)
    {
        ret = chdir("/home/");
        if(ret == -1)
        {
            printf ("Error in changing directory\n");
        //goto out;   
        }
        fd = open(text1, O_RDWR | O_CREAT);
        if(fd == -1)
        {
            printf ("Error in creating/reading file in client \n");
            return ;
        }
        while (1) 
        {
            number_of_bytes_read = read (socket_fd, buffer, sizeof(buffer));
            if (number_of_bytes_read == 0)
                return;
            ret = write(fd, &buffer, sizeof(buffer));
            if (ret <= 0){
            //ret = -1;
            //goto out;
            }
        }
        close (fd);
    }
    else
    {
        printf ("\n\t%s\n", text);
    }
}

int main (int argc, char* const argv[])
{
const char* const socket_name = argv[1];
const char* const message = argv[2];
int socket_fd;
struct sockaddr_un name;

socket_fd = socket (PF_LOCAL, SOCK_STREAM, 0);

name.sun_family = AF_LOCAL;
strcpy (name.sun_path, socket_name);

connect (socket_fd, (struct sockaddr*) &name, SUN_LEN (&name));

write_text (socket_fd, message);

read_text (socket_fd, message);
close (socket_fd);
return 0;
}




