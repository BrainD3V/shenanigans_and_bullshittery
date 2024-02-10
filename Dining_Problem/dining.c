#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>


#define 	N	5

#define		PNS	0
#define		AFF	1
#define		MNG	2

#define		LBR	0
#define		PRS	1

#define		EXT	3
#define		stanco	100000



#define    	RST 	"\033[1;0m"
#define    	RED 	"\033[1;31m"
#define    	GRN 	"\033[1;32m"
#define    	YLW 	"\033[1;33m"

struct arg{
	int fil;
};

int max = N;
//variabili della soluzione di Dijkstra
int stato[N];			//Stato del filosofo i
int shw_st[N];			//Array d'appoggio
int shw_frk[N];
sem_t mutex;			//mutex per regione critica
sem_t forch[N];			//semaforo della forchetta


//variabili dei thread (filosofi)
pthread_t filosofi[N+1];
pthread_attr_t f_attr;
int err; 			//? ← Devo stare attento, questo è copiato da un pdf su internet

//----------------------------
//dichiarazioni
void pensa(int i);
void prendi(int i);
void mangia(int i);
void lascia(int i);
void test(int i);
void alzati(int i);
//---------------------------
void* filosofo(void* arg){
	int i = ((struct arg*) arg)->fil;
	printf("Il filosofo %d si è seduto\n",i);
	while(1){
		pensa(i);
		prendi(i);
		mangia(i);
		lascia(i);
	}
	sem_wait(&mutex);
	stato[i] = EXT;
	shw_st[i] = EXT;
	sem_post(&forch[i]);
	shw_frk[i] = LBR;
	sem_post(&mutex);
	printf("Il filosofo %d si è alzato\n", i);
}

void prendi(int i){
	int dx = (i == N - 1)? 0: i+1;
	sem_wait(&mutex);	//blocco la regione critica → stato del filosofo
	stato[i] = AFF;		//modifico la regione critica
	shw_st[i] = AFF;
	test(i);		//controllo se entrambe le forchette sono libere
	sem_post(&mutex);	//rilascio la regione critica ← stato del filosofo
	sem_wait(&forch[i]);	//blocco la regione critica → stato della forchetta
	shw_frk[i] = PRS;
        shw_frk[dx] = PRS;	
}

void lascia(int i){
	int sx = (i == 0)? N - 1: i-1;
       	int dx = (i == N - 1)? 0: i+1;

	sem_wait(&mutex);	//blocco la regione critica → stato del filosofo
	stato[i] = PNS;		//modifico la regione critica
	shw_st[i] = PNS;
	test(sx);
	test(dx);
	sem_post(&mutex);	//rilascio la regione critica → stato del filosofo
}

void test(int i){
	int sx = (i == 0)? max -1 : i-1;
       	int dx = (i == max - 1)? 0: i+1;
	if(stato[i] == AFF && stato[sx] != MNG && stato[dx] != MNG){
		stato[i] = MNG;
		shw_st[i] = MNG;
		sem_post(&forch[i]);
		shw_frk[i] = LBR;
		shw_frk[dx] = LBR;
	}
}


//funzioni per l'esecuzione del programma
void init(){
	for(int i = 0; i < max; i++){
		stato[i] = PNS;
	}
}

void pensa(int i){
	if(stato[N] == PNS){
	}
}

void mangia(int i){
	if(stato[i] == MNG){
	}
}

// Utilities

void* show(){
	char fstatus[4][30] = {"è affamato\0", "sta pensando\0", "sta mangiando\0", "si è alzato\0"};
	char cstatus[4][10] = {YLW, RED, GRN, RST};
	char forchetta[2][20] = {"è presa\0", "è libera\0"};
	char ext = 0x0;
	char cnt;
	for(int i = 0; i < max; i++){
		cnt |= 0b1 << i;
	}
	printf("Checking for cnt: %b", cnt);

	while(ext != cnt){
	system("clear");
		for(int i = 0; i < max; i++){
			if(stato[i] == EXT){ 
				ext |= 0b1 << i;
				continue;
			}
			printf("Filosofo %d %s%s%s\n", i, cstatus[shw_st[i]], fstatus[shw_st[i]], RST);
		}
	}
	printf("La cena è finita \n");	
}

int main(int argc, char** argv){
	init();
	struct arg* id[N];
	for(int i = 0; i < N; i++){
		id[i] = (struct arg*) malloc(sizeof(struct arg));
	}
	sem_init(&mutex, 			//sem_t* sem		puntatore al semaforo 
		 0,				//int pshared		È condivisibile tra i processi?
	       	 1	);			//unsigned val		Valore di inizializzazione
	for(int i = 0; i < N; i++){
		sem_init(&forch[i],
			 0,
			 LBR	);
	}
	
	pthread_attr_init(&f_attr);
	for(int i = 0; i < N; i++){
		id[i]->fil = i;
		if( (err = pthread_create(&filosofi[i], &f_attr, filosofo, (void *)id[i])) != 0){
			fprintf(stderr, "Errore: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	if( (err = pthread_create(&filosofi[N], &f_attr, show, NULL)) != 0 ){
		fprintf(stderr, "Errore: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	pthread_attr_destroy(&f_attr);
	for(int i = 0; i < N; i++){
		pthread_join(filosofi[i], NULL);
	}
	printf("\n");
	printf("Sono stati mangiati: %d kg\n");
	return EXIT_SUCCESS;
}

