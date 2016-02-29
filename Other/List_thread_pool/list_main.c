#include "list.h"
#include "pool.h"

int main () {        
	
        int choice 	= 0;
        int i 		= 0;
        int ret 	= -1;
        char ch [20];
	pthread_t tid;
		
	scanf ("%s", ch);
	thread_pool_t *pool = create_thread_pool(3);
	for (i = 0; i < 20; i++) {

		pool -> list -> first_node = insert_into_list (pool -> list, ch);

		pthread_mutex_lock (& (pool -> q_lock));
		pthread_cond_broadcast (& pool -> cond_t);
		pthread_mutex_unlock (& (pool -> q_lock));
	}

	sleep (7);

	for (i = 0; i < 20; i++) {

		pool -> list -> first_node = insert_into_list (pool -> list, ch);

		pthread_mutex_lock (& (pool -> q_lock));
		pthread_cond_broadcast (& pool -> cond_t);
		pthread_mutex_unlock (& (pool -> q_lock));
	}
	
	sleep (30);
	
	destroy_threadpool (pool);
        ret = 0;

out:
        return ret;
}
