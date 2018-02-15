#include "hybrid_lock.h"
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#define MILLION 1000000L

int hybrid_lock_init(hybrid_lock* hlock){
	if((hlock=malloc(sizeof(hybrid_lock)))!=NULL){
		if(pthread_spin_init(&hlock->slock,PTHREAD_PROCESS_SHARED)!=0){
			free(hlock);
			return -1;
		}
		if(pthread_mutex_init(&hlock->mlock,NULL)!=0){
			free(hlock);
			return -1;
		}
		if(pthread_mutex_init(&hlock->conlock,NULL)!=0){
			free(hlock);
			return -1;
		}
		if(pthread_cond_init(&hlock->cond,NULL)!=0){
			free(hlock);
			return -1;
		}
		return 0;
	}
	return -1;
}
int hybrid_lock_destroy(hybrid_lock* hlock){
	pthread_spin_destroy(&hlock->slock); 
	pthread_mutex_destroy(&hlock->mlock);
	//free(hlock);
	return 0;
}
int hybrid_lock_lock(hybrid_lock* hlock){
	struct timeval start,finish;
	int i;
	gettimeofday(&start,NULL);
	while(1){
		gettimeofday(&finish,NULL);
		if((MILLION*(finish.tv_sec-start.tv_sec)+finish.tv_usec-start.tv_usec)>MILLION){
			pthread_mutex_lock(&hlock->conlock);
			pthread_cond_wait(&hlock->cond,&hlock->conlock);
			gettimeofday(&start,NULL);
			pthread_mutex_unlock(&hlock->conlock);
		}
		else if(pthread_spin_trylock(&hlock->slock)){
			pthread_mutex_lock(&hlock->mlock);
			break;
		}
	}
	return 0;
}
int hybrid_lock_unlock(hybrid_lock* hlock){
	pthread_mutex_lock(&hlock->conlock);
	pthread_spin_unlock(&hlock->slock);
	pthread_mutex_unlock(&hlock->mlock);
	pthread_mutex_unlock(&hlock->conlock);
	pthread_cond_signal(&hlock->cond);
	return 0;
}


