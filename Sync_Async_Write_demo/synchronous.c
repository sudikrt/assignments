/*This contains the demonstrations of synchronous write to a file*/ 
#include <stdio.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>

int main (int argc, char *argv [])
{
        int index       =       0;
        int ret         =       -1;
        int no          =       0;
        double start    =       0.0;
        double stop     =       0.0;
        double tot      =       0.0;
        int fd;
        struct timeval s;
        struct stat stat_var;
        
        fd=open(argv [1], O_RDWR | O_CREAT);
        if (fd < 0){
                printf("Error in creating or opening file\n");
                goto out;
        }
        gettimeofday(&s,NULL);
        start=s.tv_sec+(s.tv_usec/1000000.0);
        while(1)
        {
                no = rand ();
                ret = write(fd, &no, sizeof(no));
                if(ret < 0)
                {
                        printf ("Error in wriring\n");
                        ret = -1;
                        goto out;
                }
                ret = stat(argv [1], &stat_var);
                if(stat_var.st_size >= 1073741824)
                {
                        ret = 0;
                        break;
                }
                
        }
        gettimeofday(&s,NULL);
        stop=s.tv_sec+(s.tv_usec/1000000.0);
        
        close (fd);
        
        tot=stop-start;
        printf("\nTime taken %f\t ",tot);
        out:
                return ret;
}