#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

char* read_from_file (const char* file_name, size_t length)
{
        char* buffer;
        int fd;
        ssize_t bytes_read;
        
        /* Allocate the buffer. */
        buffer = (char*) malloc (length);
        if (buffer == NULL)
                return NULL;
        
        /* Open the file. */
        fd = open (file_name, O_RDONLY);
        /* Check for the failure of opening file. */
        if (fd == -1)
        {
                free (buffer);
                return NULL;
        }
        /* Read the data */
        bytes_read = read (fd, buffer, length);
        /* Again check for the bytes read */
        if (bytes_read != length)
        {
                free (buffer);
                close (fd);
                return NULL;
        }
        /* if Everything goes fine then. */
        close (fd);
        return buffer;
}
int main ()
{
        const char* filename = "2_1_arglist.c";
        int fd,len=100;
        fd = open (filename, O_RDONLY);
        
        printf ("Read from file is :\n ");
        printf ("%s", read_from_file (filename, len));
        return 0;

}