#include <stdio.h>
#include <fcntl.h>

#define MAX_SIZE 20000
#define FILE_NAME "random_file.dat"

int main ()
{
        int index 	=	 0;
	int number	=	 0;
	int ret 	=	 -1;
        int fd;
        
        fd=open(FILE_NAME, O_WRONLY | O_CREAT);
	if (fd < 0)
		goto out;

        for (index = 0; index < MAX_SIZE ; index++)
        {
                number = rand () % 1000;
                ret = write(fd, &number, sizeof(int));
		if (ret <= 0){
			ret = -1;
			goto out;
		}
        }
        close (fd);
	ret = 0;
	out :
		return ret; 
}

