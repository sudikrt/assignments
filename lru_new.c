#include "cache_header.h"

int check_file_on_disk (int fd, char *path)
{
        struct stat st;
        char    p_no[10];
        int ret         =       -1;
        
        sprintf (p_no, "%d", fd);
        ret = chdir(path);
        if(ret == -1)
        {
                printf ("Error in changing directory\n");
                goto out;
        }
        ret = stat(p_no, &st);
        if(ret == -1)
        {
                printf ("Error file doesnot exists\n");
                goto out;
        }
        ret = 0;
        out:
                return ret;
}

int create_file_on_disk (int fd, char *path)
{
        char    p_no[10];
        int ret         =       -1;
        
        sprintf (p_no, "%d", fd);
        ret = chdir(path);
        if(ret == -1)
        {
                printf ("Error in changing directory\n");
                goto out;
        }
        ret=open(p_no,O_RDWR | O_CREAT);
        if(ret == -1)
        {
                printf ("Error in creating/reading file\n");
                goto out;
        }
        close (ret);
        out:
        return ret;
}

int main (int argc, char *argv [])
{
        int miss        =       0;
        int k_index     =       0;
        int index       =       0;
        int count       =       0;
        int f_size      =       0;
        int j_index     =       0;
        int hit         =       0;
        int pos         =       0;
        int point       =       0;
        int ret         =       -1;
        int num         =       0;
        int fd          =       0;
        int n           =       0;
        int fault       =       0;
        int ref [20];
        int frame_array [20]; 
        
        if(argc < 4)
        {
                printf ("Error please enter the proper number of arguments\n");
                goto out;
        }
        f_size = atoi(argv [1]);
        
        fd=open(argv[2],O_RDONLY);
        
        if(f_size < 1){
                printf ("Error enter the proper cache size\n");
                goto out;
        }

        while ((n = read (fd, &num, sizeof (int))) != 0)
        {
                for ( j_index = 0; j_index < f_size; j_index++)
                {
                        if (frame_array [j_index] == num)
                        {
                                point = j_index;
                                count = 1;
                                hit++;
                                break;
                        }
                }

                //if hit occures
                if (count == 1)
                {
                        if (point <= pos)
                        {
                                for (j_index = point; j_index < pos-1;
                                        j_index++)
                                {
                                        frame_array [j_index] =
                                        frame_array [j_index+1];
                                }
                                frame_array [j_index] = num;
                        }
                        else
                        {
                                if ( point == f_size -1)
                                {
                                        frame_array [point] = num;
                                }
                                else
                                {
                                        for (j_index = point ; j_index < f_size - 1 ;
                                                j_index++)
                                        {
                                                frame_array [j_index] =
                                                        frame_array [j_index+1];
                                        }
                                        frame_array [j_index] = num;
                                }
                        }
                        printf ("\tHit in the Frame value of %d page is \t",
                                        num);
                        for (k_index = 0; k_index < f_size ; k_index++)
                        {
                                printf ("\t%d",frame_array [k_index]);
                        }
                        printf ("\n\n");

                        count = 0;
                }
                else
                {
                        //Page miss occures
                        //check file exists on disk
                        fault++;
                        if(check_file_on_disk(num, argv [3]) == 0)
                        {       
                                //if exists on the disk
                                //laod the page to cache directly
                                miss++;
                                if (pos != f_size)
                                {
                                        frame_array [pos++] = num;
                                }
                                else
                                {
                                        for (j_index = 0; j_index < f_size - 1; j_index++)
                                        {
                                                frame_array [j_index] =
                                                frame_array [j_index+1];
                                        }
                                        frame_array [j_index] = num;
                                }
                                printf ("\tFrame value of %d page is \t", num);
                                for (k_index = 0; k_index < f_size ; k_index++)
                                {
                                        printf ("\t%d",frame_array [k_index]);
                                }
                                printf ("\n\n");
                        }
                        else
                        {
                                //if not exists on the disk create the file
                                //load the page to cache

                                if(create_file_on_disk(num, argv [3]) > 0){
                                        if (pos != f_size)
                                        {
                                                frame_array [pos++] = num;
                                        }
                                        else
                                        {
                                                for (j_index = 0; j_index < f_size - 1; j_index++)
                                                {
                                                        frame_array [j_index] =
                                                        frame_array [j_index+1];
                                                }
                                                frame_array [j_index] = num;
                                        }
                                        printf ("\tFrame value of %d page is \t", num);
                                        for (k_index = 0; k_index < f_size ; k_index++)
                                        {
                                                printf ("\t%d",frame_array [k_index]);
                                        }
                                        printf ("\n\n");
                                }
                        }
                }
        }
        printf ("\tPage hit %d\n", hit);
        printf ("\tPage mis %d\n", miss);
        printf ("\tPage fault %d\n", fault);
        ret = 0;
        out :
                return ret;
}