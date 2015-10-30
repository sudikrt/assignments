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

struct lfu_var
{
        int frequency;
        int reference;
        time_t a_time;
};

typedef struct lfu_var LFU_VAR;

int main (int argc, char *argv [])
{
        struct timeval s;
        LFU_VAR lfu_cache [100];
        int ref [20];
        int miss                =       0;
        int fault               =       0;
        int index               =       0;
        int f_size              =       0;
        int pos                 =       0;
        int hit                 =       0;
        int count               =       0;
        int min_frequency       =       0;
        int new_pos             =       0;
        int j_index             =       0;
        int key                 =       0;
        int k_index             =       0;
        int ret                 =       -1;
        int num                 =       0;
        int n                   =       0;
        int fd                  =       0;
        double min_access_t     =       0.0;
        struct timespec acc_time;
        
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

        for (index = 0; index < f_size; index ++)
        {
                lfu_cache [index].reference     =       -1;
                lfu_cache [index].frequency     =       0;
                lfu_cache [index].a_time        =       0.0;
        }

        while ((n = read (fd, &num, sizeof (int))) != 0)
        {
                for (j_index = 0; j_index < pos; j_index++)
                {
                        if (num == lfu_cache [j_index].reference)
                        {
                                hit++;
                                count   =       1;
                                key     =       j_index;
                                break;
                        }
                }

                //if hit occures
                if (count == 1)
                {
                        lfu_cache [key].frequency += 1;
                        clock_gettime (CLOCK_REALTIME, &acc_time);
                        lfu_cache [key].a_time = acc_time.tv_nsec;
                        
                        printf ("\tHit in the Frame value of %d page is \t",
                                        num);
                        for (k_index = 0; k_index < f_size ; k_index++)
                        {
                                printf ("\t%d",lfu_cache [k_index].reference);
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
                                        lfu_cache [pos].reference = num;
                                        lfu_cache [pos].frequency = 1;
                                        clock_gettime (CLOCK_REALTIME, &acc_time);
                                        lfu_cache [pos].a_time = acc_time.tv_nsec;
                                        pos++;
                                }
                                else
                                {
                                        min_frequency   =       lfu_cache [0].frequency;
                                        min_access_t    =       lfu_cache [0].a_time;

                                        for (j_index = 0; j_index < pos; j_index++)
                                        {
                                                if (lfu_cache [j_index].frequency <
                                                                min_frequency)
                                                {
                                                        min_frequency =
                                                        lfu_cache [j_index].frequency;

                                                min_access_t =
                                                lfu_cache [j_index].a_time;
                                                new_pos = j_index;
                                                }
                                                else if (lfu_cache [j_index].frequency ==
                                                        min_frequency)
                                                {
                                                        if (lfu_cache [j_index].a_time <
                                                        lfu_cache [new_pos].a_time)
                                                        {
                                                        min_frequency =
                                                        lfu_cache [j_index].frequency;
                                                        min_access_t =
                                                        lfu_cache [j_index].a_time;
                                                        new_pos = j_index;
                                                        }
                                                }
                                        }
                                        lfu_cache [new_pos].reference = num;
                                        lfu_cache [new_pos].frequency = 1;
                                        clock_gettime (CLOCK_REALTIME, &acc_time);
                                        lfu_cache [new_pos].a_time = acc_time.tv_nsec;
                                }

                                printf ("\tFault Frame value of %d page is \t", num);
                                for (k_index = 0; k_index < f_size ; k_index++)
                                {
                                        printf ("\t%d",lfu_cache [k_index].reference);
                                }
                                printf ("\n\n");
                        }
                        else
                        {
                                //if not exists on the disk create the file
                                //load the page to cache
                                

                                if(create_file_on_disk(num, argv[3]) > 0)
                                {
                                        if (pos != f_size)
                                {
                                        lfu_cache [pos].reference = num;
                                        lfu_cache [pos].frequency = 1;
                                        clock_gettime (CLOCK_REALTIME, &acc_time);
                                        lfu_cache [pos].a_time = acc_time.tv_nsec;
                                        pos++;
                                }
                                else
                                {
                                        min_frequency   =       lfu_cache [0].frequency;
                                        min_access_t    =       lfu_cache [0].a_time;

                                        for (j_index = 0; j_index < pos; j_index++)
                                        {
                                                if (lfu_cache [j_index].frequency <
                                                                min_frequency)
                                                {
                                                        min_frequency =
                                                        lfu_cache [j_index].frequency;

                                                min_access_t =
                                                lfu_cache [j_index].a_time;
                                                new_pos = j_index;
                                                }
                                                else if (lfu_cache [j_index].frequency ==
                                                        min_frequency)
                                                {
                                                        if (lfu_cache [j_index].a_time <
                                                        lfu_cache [new_pos].a_time)
                                                        {
                                                        min_frequency =
                                                        lfu_cache [j_index].frequency;
                                                        min_access_t =
                                                        lfu_cache [j_index].a_time;
                                                        new_pos = j_index;
                                                        }
                                                }
                                        }
                                        lfu_cache [new_pos].reference = num;
                                        lfu_cache [new_pos].frequency = 1;
                                        clock_gettime (CLOCK_REALTIME, &acc_time);
                                        lfu_cache [new_pos].a_time = acc_time.tv_nsec;
                                }

                                printf ("\tFault Frame value of %d page is \t", num);
                                for (k_index = 0; k_index < f_size ; k_index++)
                                {
                                        printf ("\t%d",lfu_cache [k_index].reference);
                                }
                                printf ("\n\n");
                                }
                        }
                }
        }

        printf ("\t Page hit %d\n", hit);
        printf ("\t Page miss %d\n", miss);
        printf ("\t Page Fault %d\n", fault);
        ret = 0;

        out :
                return ret;
}
