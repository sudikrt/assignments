#include <stdio.h>
#include <pthread.h>

struct pass_data
{
        int count;
};

void* fib_cal (void* param)
{
        struct pass_data* p = (struct pass_data*) param;
        int i;
        int f1 = 0, f2 = 1, fib = 0;
        printf ("Fibonocci series :\n");
        if (p->count == 1)
        {
                printf ("%d", f1);
        }
        else if (p->count == 2)
        {
                printf ("%d\t%d", f1, f2);
        }
        else
        {
                printf ("%d\t%d", f1, f2);
                for (i = 3; i <= p->count; i++)
                {
                        fib = f1 + f2;
                        printf ("%d\t", fib);
                        f1 = f2;
                        f2 = fib;
                }
        }
        return NULL;
}

int main ()
{
        pthread_t id1;
        pthread_t id2;
        
        struct pass_data arg1;
        struct pass_data arg2;
        
        arg1.count = 5;
        arg2.count = 10;
        
        pthread_create (&id1, NULL, &fib_cal, &arg1);
        printf ("\n");
        pthread_create (&id2, NULL, &fib_cal, &arg2);
        
        pthread_join (id1, NULL);
        pthread_join (id2, NULL);
        
        return 0;
}