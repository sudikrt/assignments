#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define COUNT_1 3
#define COUNT_2 6
#define COUNT_END 10
int count = 0;

pthread_mutex_t c_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c_var = PTHREAD_COND_INITIALIZER;



void* funct1 ()
{
        for(;;)
        {
                pthread_mutex_lock (&c_mutex);
        
                pthread_cond_wait (&c_var, &c_mutex);
        
                count++;
                printf ("Cvalue Funct1 %d:\n",count);
        
                pthread_mutex_unlock (&c_mutex);
                
                if(count >= COUNT_END) return(NULL);
        }
        
}

void* funct2 ()
{
        for(;;)
        {
                pthread_mutex_lock (&c_mutex);
        
                if( count < COUNT_1 || count > COUNT_2)
                {
                        pthread_cond_signal ( &c_var);
                }
                else
                {
                        count++;
                        printf ("Cvalue Funct2 %d:\n", count);
                }
        
                pthread_mutex_unlock (&c_mutex);
        
                if(count >= COUNT_END) return(NULL);
        }
}
int main ()
{
        pthread_t thread_1, thread_2;
        
        pthread_create (&thread_1, NULL, &funct1, NULL);
        pthread_create (&thread_2, NULL, &funct2, NULL);
        
        pthread_join (thread_1, NULL);
        pthread_join (thread_2, NULL);
        
        return 0;

}