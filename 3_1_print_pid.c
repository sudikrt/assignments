#include <stdio.h>
#include <unistd.h>

int main ()
{
  printf ("Process id is : %d\n", (int) getpid());
  printf ("Parent process id : %d\n", (int) getppid());
  return 0;
}
