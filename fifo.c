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
        int ret         =       -1;
        int ref [20];
        int frame_array [10];

        printf ("\tEnter the no. of elements :");
        scanf ("%d", &limit);

        if (limit <= 0)
                goto out;

        printf ("\tEnter the page no. :");
        for (index = 0; index < limit; index++)
        {
                scanf ("%d", &ref [index]);
        }

        printf ("\tEnter the frame size:");
        scanf ("%d", &f_size);

        if (limit <= 0)
                goto out;

        for (index = 0; index < limit; index++)
        {
                count = 0;
                for ( j_index = 0; j_index < f_size; j_index++)
                {
                        if (ref [index] == frame_array [j_index])
                        {
                                hit++;
                                count = 1;
                                printf ("\tHit in the Frame value of %d page is \t", ref [index]);
                                for (k_index = 0; k_index < f_size ; k_index++)
                                {
                                        printf ("\t%d",frame_array [k_index]);
                                }
                                printf ("\n\n");
                                break;
                        }
                }

                if (count == 0){
                        if (pos >= f_size )
                        {
                                pos = 0;
                        }
                        frame_array [pos++] = ref [index];
                       
                        printf ("\tFrame value is of %d page is \t", ref [index]);
                                for (k_index = 0; k_index < f_size ; k_index++)
                        {
                                printf ("\t%d",frame_array [k_index]);
                        }
                        printf ("\n\n");
                }
        }

        printf ("Hit %d\n", hit);
        printf ("Total no of page fault is %d\n", (limit-hit));
        ret = 0;

        out :
                return ret;
}