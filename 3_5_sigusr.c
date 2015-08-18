#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

sig_atomic_t sigusr_count = 0;
void handler (int signal_number)
{
        printf ("Hello\n");
        ++sigusr_count;
}

int main ()
{
        struct sigaction sa;
        int i;
        memset (&sa, 0, sizeof (sa));
        sa.sa_handler = &handler;
        sigaction (SIGUSR1, &sa, NULL);
        
        /* Doing some lengthy stuffs. */
        for (i = 0; i < 10000000; i++){
                printf ("%d\t", i);
               
        }
        
        printf ("SIGUSR1 was raised %d times\n", sigusr_count);
        return 0;
}