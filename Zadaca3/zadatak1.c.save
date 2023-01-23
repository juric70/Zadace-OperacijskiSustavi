#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
int *ULAZ, *BROJ, brSt, brDr, *REZ;
int max(){
	int najv = BROJ[0];
	for(int i = 0; i<brSt; i++){
		if(BROJ[i] > najv){
			najv = BROJ[i];
		}
	}
	return najv;
}
int sve_zauzeto(){
	for(int i = 0; i<brSt; i++){
		if(REZ[i] == -1){
			return 0;
		}
	}
	return 1;
}

void udji_u_kriticni_odsjecak(int i){
	ULAZ[i] = 1;
	BROJ[i] = max() + 1;
	ULAZ[i] = 0;

	for(int j = 0;j< brSt-1;j++){
		while(ULAZ[j]!=0){}
		while(BROJ[j] != 0 && (BROJ[j] < BROJ[i] || (BROJ[j] == BROJ[i] && j<i))){};
	}
}

void izadji_iz_kriticnog_odsjecka(int i){
	BROJ[i] = 0;
}
void *provjeri_stol(void *pdr){
	if(sve_zauzeto()){
	pthread_exit("svi su stolovi zauzeti");
	}
	int dr =*((int *)pdr);

	int rand_tbl = rand() % brSt;
	printf("Dretva %d: pokusavam rez stol %d\n", dr + 1, rand_tbl + 1);
	udji_u_kriticni_odsjecak(rand_tbl);
	char stanje[brSt ];
        for(int i = 0; i<brSt; i++){
                stanje[i] = (REZ[i] == -1) ? '-' : REZ[i] + 1 + '0';
        }
        stanje[brSt] = '\0';
	if(REZ[rand_tbl] == -1){
		REZ[rand_tbl] = dr;
		for(int i = 0; i<brSt; i++){
                        stanje[i] = (REZ[i] == -1) ? '-' : REZ[i] + 1 + '0';
                }
                stanje[brSt] = '\0';
		printf("Dretva %d: rezerviram stol %d, stanje: %s\n", dr + 1, rand_tbl + 1, stanje);
	}
	else{
		printf("Dretva %d: neuspjela rezervacija stola %d, stanje: %s\n", dr + 1, rand_tbl + 1, stanje);
	}
	izadji_iz_kriticnog_odsjecka(rand_tbl);

}
void main(){
	printf("br dretvi: ");
	scanf("%d", &brDr);
	printf("br st: ");
	scanf("%d", &brSt);
	fflush(stdin);
	pthread_t thread_id;

	REZ = (int*)malloc(sizeof(int)*brSt);
        BROJ = (int*)malloc(sizeof(int)*brSt);
        ULAZ = (int*)malloc(sizeof(int)*brSt);
	for(int i = 0; i<brSt;i++){
		REZ[i] = -1;
		BROJ[i] = 0;
		ULAZ[i] = 0;
	}
	while(!sve_zauzeto()){
	for(int i = 0; i<brDr; i++){
		int * pdr = (int * ) malloc (sizeof(int));
		*pdr = i;
		pthread_create(&thread_id, NULL, provjeri_stol, pdr);
	};}
	for(int i = 0; i<brDr; i++){
		pthread_join(thread_id, NULL);
	};

	free(REZ);
	free(BROJ);
	free(ULAZ);
}
