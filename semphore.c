#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <error.h>
#include <sys/types.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>

sem_t sem;
int count = 0;
void* fun (void* dat)
{
        int c = *((int *) dat);
        printf ("%d variable is waiting\n", c);
        sem_wait (&sem);
        printf ("%d is entered\n", c);
        printf ("%d thread is with counter value %d\n", c, count);
        count++;
        printf ("%d thread is with new counter value %d\n", c, count);
        printf ("Exiting\n");
        sem_post (&sem);
}

int main ()
{
        pthread_t id1, id2;
        int a = 1, b = 2;
        sem_init(&sem, 0, 1);
        pthread_create (&id1, NULL, &fun, (void *) &a);
        pthread_create (&id2, NULL, &fun, (void *) &b);
        
        pthread_join (id1, NULL);
        pthread_join (id2, NULL);
        sem_destroy (&sem);
        return 0;
}