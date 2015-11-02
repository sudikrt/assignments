/*Program to demonstrate the asynchronous write
 * initially created a buffer of 1MB and writing
 * the contents to the file on the disk*/
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>

int main (int argc, char *argv [])
{
        int ret         =       -1;
        int index       =       0;
        int no          =       0;
        double start    =       0.0;
        double stop     =       0.0;
        double tot      =       0.0;
        int fd;
        struct timeval s;
        struct stat stat_var;
        int *buf;
        
        fd = open(argv [1], O_RDWR | O_CREAT);
        if (fd < 0){
                printf("Error in creating or opening file\n");
                goto out;
        }
        if (buf == NULL){
                printf("Error in allocating memory\n");
                goto out;
        }
        buf =(int*)calloc(1024*1024, sizeof(int));
        gettimeofday(&s,NULL);
        start=s.tv_sec+(s.tv_usec/1000000.0);
        while (1)
        {
                for (index = 0; index < 1024*1024; index++)
                {
                        no = rand ();
                        *(buf+index)=no;
                }

                ret = stat(argv [1], &stat_var);

                if(stat_var.st_size >= 1073741824)
                {
                        ret = 0;
                        break;
                }

                ret = write(fd, buf, sizeof(buf)*(1024*1024));
                if(ret < 0)
                {
                        printf ("Error in wriring\n");
                                ret = -1;
                                goto out;
                }
        }
        gettimeofday(&s,NULL);
        stop=s.tv_sec+(s.tv_usec/1000000.0);
        free (buf);
        close (fd);

        tot=stop-start;
        printf("\nTime taken %f\t ",tot);

        out:
                return ret;
}