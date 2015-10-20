#include "cache_header.h"

int main ()
{
        int limit       =       0;
        int k_index     =       0;
        int index       =       0;
        int count       =       0;
        int f_size      =       0;
        int j_index     =       0;
        int hit         =       0;
        int pos         =       0;
        int point       =       0;
        int ret         =       -1;
        int ref [20];
        int frame_array [20]; 

        printf ("\tEnter the no. of elements :");
        scanf ("%d", &limit);

        if (limit <= 0)
                goto out;

        printf ("\tEnter the page no. :");
        for (index = 0; index < limit; index++)
        {
                scanf ("%d", &ref [index]);
                frame_array [index] = -1;
        }

        printf ("\tEnter the frame size:");
        scanf ("%d", &f_size);

        if (f_size <= 0)
                goto out;

        for (index = 0; index < limit; index++)
        {
                for ( j_index = 0; j_index < f_size; j_index++)
                {
                        if (frame_array [j_index] == ref [index])
                        {
                                point = j_index;
                                count = 1;
                                hit++;
                                break;
                        }
                }

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
                                frame_array [j_index] = ref [index];
                        }
                        else
                        {
                                if ( point == f_size -1)
                                {
                                        frame_array [point] = ref [index];
                                }
                                else
                                {
                                        for (j_index = point ; j_index < f_size - 1 ;
                                                j_index++)
                                        {
                                                frame_array [j_index] =
                                                        frame_array [j_index+1];
                                        }
                                        frame_array [j_index] = ref [index];
                                }
                        }
                        printf ("\tHit in the Frame value of %d page is \t",
                                        ref [index]);
                        for (k_index = 0; k_index < f_size ; k_index++)
                        {
                                printf ("\t%d",frame_array [k_index]);
                        }
                        printf ("\n\n");

                        count = 0;
                }
                else
                {
                        if (pos != f_size)
                        {
                                frame_array [pos++] = ref [index];
                        }
                        else
                        {
                                for (j_index = 0; j_index < f_size - 1; j_index++)
                                {
                                        frame_array [j_index] =
                                                frame_array [j_index+1];
                                }
                                frame_array [j_index] = ref [index];
                        }
                        printf ("\tFrame value of %d page is \t", ref [index]);
                        for (k_index = 0; k_index < f_size ; k_index++)
                        {
                                printf ("\t%d",frame_array [k_index]);
                        }
                        printf ("\n\n");
                }
        }
        printf ("No of page faults is %d\n", (limit-hit));
        ret = 0;
        out :
                return ret;
}