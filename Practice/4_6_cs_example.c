#include <pthread.h>
#include <stdio.h>
#include <string.h>

void* print_data (void* i)
{
        int old_state;
        int s = *((int*) i);
        
        /* Critical Section starts */
        pthread_setcancelstate (PTHREAD_CANCEL_DISABLE, &old_state);
        
        printf ("{");
        
        printf ("%d", s);
       
        printf ("}");
        
        
        /* Critical section ends */
        pthread_setcancelstate (old_state, NULL);
}

int main ()
{
        pthread_t id1;
        pthread_t id2;
        pthread_t id3;
        
        int a = 10;
        pthread_create (&id1, NULL, &print_data, &a);
        a = 20;
        pthread_create (&id2, NULL, &print_data, &a);
        a = 30;
        pthread_create (&id3, NULL, &print_data, &a);
        
        pthread_join (id1, NULL);
        pthread_join (id2, NULL);
        pthread_join (id3, NULL);
}
