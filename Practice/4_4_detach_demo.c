#include <stdio.h>
#include <pthread.h>

void* thread_fun (void* arg)
{
        
}

int main ()
{
        pthread_attr_t attr;
        pthread_t id;
        
        pthread_attr_init (&attr);
        pthread_attr_setdetachstate (&attr,PTHREAD_CREATE_DETACHED);
        pthread_create (&thread, &attr, &thread_fun, NULL);
        pthread_attr_destroy (&attr);
        
        
        return 0;

}