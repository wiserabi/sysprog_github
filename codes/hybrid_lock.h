#include<stdlib.h>

typedef struct hybrid_lock{
	pthread_spinlock_t slock;
	pthread_mutex_t mlock;
	pthread_mutex_t conlock;
	pthread_cond_t cond;
}hybrid_lock;
int hybrid_lock_init(hybrid_lock* hlock);
int hybrid_lock_destroy(hybrid_lock* hlock);
int hybrid_lock_lock(hybrid_lock* hlock);
int hybrid_lock_unlock(hybrid_lock* hlock);


