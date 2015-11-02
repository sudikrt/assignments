#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock_fun = PTHREAD_MUTEX_INITIALIZER;
void* fun (void* a)
{
        int x = *((int*) a);
        int i;
        pthread_mutex_lock (&lock_fun);
        for (i = 0; i < 100; i++)
        {
                printf ("Value :%d\n", i);
        }
        pthread_mutex_unlock (&lock_fun);
}
int main ()
{
        pthread_t id1;
        pthread_t id2;
        int a = 10, b = 20;
        pthread_create (&id1, NULL, &fun, &a);
        pthread_create (&id2, NULL, &fun, &b);
        
        pthread_join (id1, NULL);
        pthread_join (id2, NULL);
        return 0;
}