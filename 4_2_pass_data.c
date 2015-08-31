#include <stdio.h>
#include <pthread.h>

struct pass_arg
{
        char c;
        int times;
};
void* print_data (void* pass_param)
{
        struct pass_arg* p = (struct pass_arg*) pass_param;
        int i;
        
        for (i = 0; i < p->times; i++)
                fputc (p->c, stderr);
        return NULL;
}

int main ()
{
        pthread_t id1;
        pthread_t id2;
        struct pass_arg arg1;
        struct pass_arg arg2;
        
        arg1.c = 'a';
        arg1.times = 10;
        pthread_create (&id1, NULL, &print_data, &arg1);
        
        arg2.c = 'b';
        arg2.times = 10;
        pthread_create (&id2, NULL, &print_data, &arg2);
        
        return 0;

}