#include <stdio.h>
#include <pthread.h>

void* fact_cal (void* arg)
{
        int fact = 1;
        int n = *((int*) arg);
        int i;
        
        printf ("Fact %d \n",fact);
        printf ("N :%d\n", n);
        for (i = 1; i <= n; ++i){
                fact = fact * i;
                //printf("%d\n",fact);
        }
        
        printf ("Fact %d \n",fact);
        return (void*) fact;
}
int main ()
{
        pthread_t id;
        int fact_data = 5;
        int result = 0;
        
        pthread_create (&id, NULL, &fact_cal, &fact_data);
        
        pthread_join (id, (void*) &result);
        printf ("Result :%d\n", result);
        return 0;
}
