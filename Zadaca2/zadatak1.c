#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/wait.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <pthread.h>
#define SHARED_MEMORY_SIZE (0x6400)
int segment_Id;
int *sh_mem;
void brisi(){
	printf("signal primljen gasi sve");
	shmdt(sh_mem);
	shmctl(segment_Id, IPC_RMID, NULL);
	exit(0);
}

void main(){
	segment_Id = shmget(IPC_PRIVATE, SHARED_MEMORY_SIZE, IPC_CREAT | 0660 );
	sh_mem = (int*) shmat(segment_Id, NULL, 0);
	while(1){
		if(fork() == 0){
			*sh_mem = 1;
			printf("%d\n", *sh_mem);
			sleep(3);
			printf("%d\n", *sh_mem);
		  	exit(0);
		}

			signal(SIGINT, brisi);
			*sh_mem = 2;
			printf("%d\n", *sh_mem);
			sleep(3);
			printf("%d\n", *sh_mem);
			wait(NULL);

	}
}
