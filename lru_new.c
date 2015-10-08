#include <stdio.h>

int main ()
{
        int n, k, i, count = 0, ref [20], f_size, frame_array [20], j, 
        hit = 0, pos = 0, point = 0;
        printf ("\tEnter the no. of elements :");
        scanf ("%d", &n);
        printf ("\tEnter the frame size:");
        scanf ("%d", &f_size);
        printf ("\tEnter the page no. :");
        for (i = 0; i < n; i++)
        {
                scanf ("%d", &ref [i]);
                frame_array [i] = -1;
        }
        for (i = 0; i < n; i++)
        {
                for ( j = 0; j < f_size; j++)
                {
                        if (frame_array [j] == ref [i])
                        {
                                point = j;
                                count = 1;
                                hit++;
                                break;
                        }
                }
                if (count == 1)
                {
                        if (point <= pos)
                        {
                                for (j = point; j < pos-1; j++)
                                {
                                        frame_array [j] = frame_array [j+1];
                                }
                                frame_array [j] = ref [i];
                        }
                        else
                        {
                                if ( point == f_size -1)
                                {
                                        frame_array [point] = ref [i];
                                }
                                else
                                {
                                        for (j = point ; j < f_size - 1 ; j++)
                                        {
                                                frame_array [j] = frame_array [j+1];
                                        }
                                        frame_array [j] = ref [i];
                                }
                        }
                        printf ("\tHit in the Frame value of %d page is \t", ref [i]);
                        for (k = 0; k < f_size ; k++)
                        {
                                printf ("\t%d",frame_array [k]);
                        }
                        printf ("\n\n");
                        count = 0;
                }
                else
                {
                        if (pos != f_size)
                        {
                                frame_array [pos++] = ref [i];
                        }
                        else
                        {
                                for (j = 0; j < f_size - 1; j++)
                                {
                                        frame_array [j] = frame_array [j+1];
                                }
                                frame_array [j] = ref [i];
                        }
                        printf ("\tFrame value of %d page is \t", ref [i]);
                        for (k = 0; k < f_size ; k++)
                        {
                                printf ("\t%d",frame_array [k]);
                        }
                        printf ("\n\n");
                }
        }
        printf ("No of page faults is %d\n", (n-hit));

}