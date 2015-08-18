#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int spawn (char* program, char** arg_list)
{
        pid_t child_pid;
        
        /* Creating child. */
        child_pid = fork ();
        if (child_pid != 0)
        {
                /* This is the parent process. */
                return child_pid;
        }
        else
        {
                /* Now execute the program searching it the path. */
                execvp (program, arg_list);
                /* The execvp function returns only if an error occurs. */
                fprintf (stderr, "an error occurred in execvp\n");
                abort ();
        }
}

int main ()
{
        /* The list of arguments to pass to ls command. */
        char* arg_list[] = {"ls","-l","/",NULL};

        /* Spawn a child process running the “ls” command. Ignore
         * returned child PID. */
         spawn ("ls", arg_list);
         
         printf ("Done!");
         return 0;
}