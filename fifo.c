#include <stdio.h>
#include <stdlib.h>

int main ()
{
        int n, k, i, count = 0, ref [20 ], f_size, frame_array [10] = {0}, j, hit = 0, pos = 0;
        printf ("\tEnter the no. of elements :");
        scanf ("%d", &n);
        printf ("\tEnter the frame size:");
        scanf ("%d", &f_size);
        printf ("\tEnter the page no. :");
        for (i = 0; i < n; i++)
        {
                scanf ("%d", &ref [i]);
        }
        for (i = 0; i < n; i++)
        {
                count = 0;
                for ( j = 0; j < f_size; j++)
                {
                        if (ref [i] == frame_array [j])
                        {
                                hit++;
                                count = 1;
                                printf ("\tHit in the Frame value of %d page is \t", ref [i]);
                                for (k = 0; k < f_size ; k++)
                                {
                                        printf ("\t%d",frame_array [k]);
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
                        frame_array [pos++] = ref [i];
                       
                        printf ("\tFrame value is of %d page is \t", ref [i]);
                                for (k = 0; k < f_size ; k++)
                        {
                                printf ("\t%d",frame_array [k]);
                        }
                        printf ("\n\n");
                }
        }
        printf ("Hit %d\n", hit);
        printf ("Total no of page fault is %d\n", (n-hit));
}