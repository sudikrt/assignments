#include <stdio.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>

int main ()
{
        int seg_id;
        char* shared_mem;
        struct shmid_ds buffer;
        int seg_size;
        const int sh_size = 0X6400;
        pid_t pid;
        
        /* Allocating Memory */
        seg_id = shmget (IPC_PRIVATE, sh_size, 
                        IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
        /* Attaching Shared memory */
        shared_mem = (char*) shmat (seg_id, 0, 0);
        
        printf ("Shared memory is attached to the adress is %p\n", shared_mem);
        
        shmctl (seg_id, IPC_STAT, &buffer);
        
        seg_size = buffer.shm_segsz;
        printf ("Segment size is %d\n", sh_size);
        
        sprintf (shared_mem, "Hello WWWWorld\n");
        printf ("Contents %s\n",shared_mem);
        shmdt (shared_mem);
        
        pid = fork ();
        shared_mem = (char*) shmat (seg_id, (void*) 0x5000000, 0);
        
        printf ("New address is %p\n", shared_mem);
        
        sprintf (shared_mem, "Hello RRRRedHat\n");
        printf ("Contents %s\n",shared_mem);
        
        shmdt (shared_mem);
        
        shmctl (seg_id, IPC_RMID, 0);
        return 0;
}