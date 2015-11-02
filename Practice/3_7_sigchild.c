#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

sig_atomic_t child_exit_status;

void clean_up_child_process (int signal_number)
{
        /* Cleaning child process. */
        int status;
        wait (&status);
        /* Store its exit status in a global variable. */
        child_exit_status = status;
}

int main ()
{
        pid_t child_pid;
        /* Handle SIGCHILD by calling clean_up_child_process. */
        struct sigaction sigchld_action;
        memset (&sigchld_action, 0, sizeof (sigchld_action));
        sigchld_action.sa_handler = &clean_up_child_process;
        sigaction (SIGCHLD, &sigchld_action, NULL);
        
        /* Forking Child process. */
        child_pid = fork ();
        if (child_pid > 0)
        {
                printf ("hello\n");
                sleep(60);
        }
        else
        {
                printf ("hello333333333\n");
                /* exit (0); */
        }
        printf ("Status %d\n", child_exit_status);
        return 0;
}