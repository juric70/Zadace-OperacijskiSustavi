#include <math.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int zavrsi = 0;
int statusBr;

void sigint_funkcija ( int sig ){
	printf("\nOdaberite radnju:\n1) Prekid programa (bez dovrsavanja radnje)\n2) Zavrsi program\n3) Ispisi trenutni status\n4) Nastavi\n");
	int input;
	scanf("%d", &input);
	switch(input){
		case 1:
			printf("\nPrekid programa\n");
			exit(0);
			break;
		case 2:
			kill(getpid(), SIGTERM);
			break;
		case 3:
			kill(getpid(), SIGUSR1);
			break;
		default:
			break;
	}
}

void sigterm_funkcija ( int sig ){
	printf("\nSIGTERM primljen, zavrsavanje programa\n");
	zavrsi = 1;
	exit(0);
}

void sigusr1_funkcija ( int sig ){
	printf("\nTrenutni status: %d\n", statusBr);
}

void proces(FILE *status, FILE *obrada){

	//printf("\n%d\n", statusBr);

	if(statusBr == 0){
		int obradaBr;
		while(!feof(obrada)){
			fscanf(obrada, "%d", &obradaBr);
		}
		statusBr = round(sqrt(obradaBr));
		//printf("\n%d\n", statusBr);
	}

	fseek(status, 0, SEEK_SET);
	fprintf(status, "%d", 0 );
	fflush(status);
	fflush(obrada);
	//printf("Upisana 0\n");
	sleep(3);
	fseek(status, 0, SEEK_SET);
	fseek(obrada, 0, SEEK_END);
	statusBr++;
	fprintf(status, "%d", statusBr);
	fprintf(obrada, "%d\n", statusBr*statusBr);
	//printf("Upisan broj\n");

	fflush(status);
        fflush(obrada);
	sleep(2);
}

void main(){

	FILE  *status, *obrada;
	status = fopen("status.txt", "r");
	obrada = fopen("obrada.txt", "r+");

	fscanf(status, "%d", &statusBr);
	signal(SIGTERM, sigterm_funkcija);
	signal(SIGINT, sigint_funkcija);
	signal(SIGUSR1, sigusr1_funkcija);

	while(zavrsi == 0){
		status = freopen("status.txt", "w+", status);
		proces(status, obrada);
	}
	fclose(status);
	fclose(obrada);
}
