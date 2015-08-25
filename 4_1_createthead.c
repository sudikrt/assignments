#include <stdio.h>
#include <pthread.h>

void* calc (void* tep)
{
        int i;
        printf ("hello\n");
        int fact = 1;
        for (i = 1; i < 10000; i++)
        {
                fact = fact * i;
                fputc (fact, stderr);
                printf ("%d\t", fact);
        }
        
        return NULL;
}

int main()
{
        pthread_t thread_id;
        /*Create the new Thread*/
        pthread_create (&thread_id, NULL, &calc, NULL);
        pthread_join (thread_id, NULL);
        
        return 0;
}
