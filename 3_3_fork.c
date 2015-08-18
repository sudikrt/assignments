#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main ()
{
  pid_t child_pid;

  printf ("The pid of main program is : %d\n", (int) getpid());

  child_pid = fork ();
  if (child_pid != 0 )
  {
    printf ("This is the parent process with PID : %d\n", (int) getpid());
    printf ("The child PID is : %d\n", (int) child_pid);
  }
  else
  {
    printf ("Hello");
    printf ("The child PID is :%d\n", (int) getpid());
  }

  return 0;
}
