#include<stdio.h>
#include <stdlib.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>
#define N 5


int fil_br[5] = {0,1,2,3,4}; 
sem_t sem[N];
sem_t mutex;
int state[N];

void test(int br){
	//1 == gladan, 2 == jede, 3 == misli
	if(state[br] == 1 && state[(br+4)%N] != 2 && state[(br+1)%N] != 2 ){
		
		state[br] = 2;
		printf("filozof %d uzima vilice i jede\n", br +1);
		fflush(stdin);
		sleep(1);
		sem_post(&sem[br]);
	}
}

void uzima_vilicu(int br){
	sem_wait(&mutex);
	state[br] = 1;
	printf("filozof %d je gladan\n", br +1 );
	test(br);
	sem_post(&mutex);
	sem_wait(&sem[br]);
	fflush(stdin);
	sleep(1);
}

void vrati_vilice(int br){
	sem_wait(&mutex);
	
	printf("filozof %d ostavlja vilice i ide misliti\n", br +1);
	fflush(stdin);
	sleep(1);
	state[br] = 3;
	test((br+4)%N);
	test((br+1)%N);
	sem_post(&mutex);	
}

void* filozof(void* pdr){
	int temp =*((int *)pdr);
	while(1){
		int temp =*((int *)pdr);
		sleep(2);
		uzima_vilicu(temp);
		sleep(0);
		vrati_vilice(temp);
	}
}

void main(){
	pthread_t thread_id[N];
	sem_init(&mutex, 0, 1);
	int br;
	for(int i=0; i<N; i++){
		sem_init(&sem[i],0,0);
		state[i] = 3;
	}
	for(int i=0; i<N; i++){
		int *br = (int *) malloc(sizeof(int));
		*br = i;
		pthread_create(&thread_id[i], NULL, filozof,  br);
		printf("Filozof %d misli\n", i+1);
	}
	for(int i=0; i<N; i++){
		pthread_join(thread_id[i], NULL);
	}
	
}
