#include "cache_header.h"

struct lfu_var
{
        int frequency;
        int reference;
        time_t a_time;
};

typedef struct lfu_var LFU_VAR;

int main ()
{
        struct timeval s;
        LFU_VAR lfu_cache [10];
        int ref [20];
        int limit               =       0;
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
        double min_access_t     =       0.0;
        struct timespec acc_time;


        printf ("\tEnter the number of pages: ");
        scanf ("%d", &limit);

        if (limit <= 0)
                goto out;

        printf ("\tEnter the page references:");
        for (index = 0; index < limit; index++)
        {
                scanf ("%d", &ref [index]);
        }

        printf ("\tEnter the frame size:");
        scanf ("%d", &f_size);

        for (index = 0; index < f_size; index ++)
        {
                lfu_cache [index].reference     =       -1;
                lfu_cache [index].frequency     =       0;
                lfu_cache [index].a_time        =       0.0;
        }

        for (index = 0; index < limit; index++)
        {
                for (j_index = 0; j_index < pos; j_index++)
                {
                        if (ref [index] == lfu_cache [j_index].reference)
                        {
                                hit++;
                                count   =       1;
                                key     =       j_index;
                                break;
                        }
                }

                if (count == 1)
                {
                        lfu_cache [key].frequency += 1;
                        clock_gettime (CLOCK_REALTIME, &acc_time);
                        lfu_cache [key].a_time = acc_time.tv_nsec;
                        
                        printf ("\tHit in the Frame value of %d page is \t",
                                        ref [index]);
                        for (k_index = 0; k_index < f_size ; k_index++)
                        {
                                printf ("\t%d",lfu_cache [k_index].reference);
                        }
                        printf ("\n\n");
                        count = 0;
                }
                else
                {
                        if (pos != f_size)
                        {
                                lfu_cache [pos].reference = ref [index];
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
                                lfu_cache [new_pos].reference = ref [index];
                                lfu_cache [new_pos].frequency = 1;
                                clock_gettime (CLOCK_REALTIME, &acc_time);
                                lfu_cache [new_pos].a_time = acc_time.tv_nsec;
                        }

                        printf ("\tFault Frame value of %d page is \t", ref [index]);
                        for (k_index = 0; k_index < f_size ; k_index++)
                        {
                                printf ("\t%d",lfu_cache [k_index].reference);
                        }
                        printf ("\n\n");
                }
        }

        printf ("\tTotal no of page fault is %d\n", (limit - hit));
        ret = 0;

        out :
                return ret;
}
//      2 5 6 4 2 4 2 6 7 8
