#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
/* A Handler for a temporary file created with write_temp_file.
   In this implementation it's just a file descripter */
typedef int temp_file_handle;

/* Writes length bytes from BUFFER into a temporary file. The 
   temporary file is immediately unlinked. Returns a handle to the 
   temporary file */

temp_file_handle write_temp_file (char* buffer, size_t length)
{
	/* Create the filename and file. The XXXXXX will be replaced with 
	   characters that make the filename unique*/
	char temp_filename[] = "/tmp/temp_file.XXXXXX";
	int fd = mkstemp (temp_filename);
	/* Unlink the file immediately, so that it will be removed when the
	   file descriptor is closed. */
	//unlink (temp_filename);
	/* Writes the number of bytes first. */
	write (fd, &length, sizeof(length));

	/* Now writing the data itself. */
	write (fd, buffer, length);
	
	return fd;
}

/* Reads the content of file a temporary file*/

char* read_temp_file (temp_file_handle temp_file, size_t* length)
{
	char* buffer;
	/* The TEMP_FILE handle is a file descriptor to the temporary file. */
	int fd = temp_file;
	/* Rewind to the beginning of the file. */
	lseek (fd, 0, SEEK_SET);
	/* Read the size of the data in the temporary file. */
	read (fd, length, sizeof (*length));
	/* Allocate a buffer and read the data. */
	buffer = (char*) malloc (*length);
	read (fd, buffer, *length);
	/* Close the file descriptor, which will cause the temporary file to
	   go away. */
	close (fd);
	return buffer;
}

int main ()
{
	char* buffer = "#include <stdlib.h>/* Write the number of bytes to the file";

	size_t len = 100000000;
	int i = write_temp_file (buffer, strlen(buffer)); 
	
	printf ("Contents are :\n %s", read_temp_file (i, &len));
}
	
