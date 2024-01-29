#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>


#define CHARS 62
#define NCAND 10
#define FSAFE 100000 								//Failsafe, è solo una prova di concetto, non ho intenzione di fare un'implementazione seria. (anche perchè dovrei determinare dei numeri primi a 1024 bit)
#define MXLEN 25



int p, q;
int n, z;
int e, d;
int b_size;
char* message;




struct block{
	int size;
	int used;
	char* blk;
};

void init_block(struct block* b, int sz){
	b->size = sz;
	b->used = 0;
	int n_bytes = (sz / 8)+1;
	b->blk = (char*) malloc((n_bytes+1)*sizeof(char));
}

void rest_block(struct block* b){
	b->used = 0;
	b->blk = "";
}

void cat_ch_block(struct block* b, char c){
	if(b->used <= b->size){
		b->blk = 
	}
}

void look_for_d(int* c){
	int i = 0;
	int found = 0;
	
	while(found < NCAND){
		for(int j = 2; j < z; j++){
			int div = z/j;
			float fdiv = (float)z/(float)j;
			printf("j: %d", j);
			if((fdiv - div) != 0) {
				printf("\t→\tè un candidato.\n");
				*(c+i) = j;
				found++;
				i++;
			} else {
				printf("\n");
			}
			if( found == NCAND ) return;
		}
		printf("\rCandidati trovati: %d.", found);
		fflush(stdout);
	}
}

int choose_d(int* c){
	int chosen, choice;
	int valid = 0;
	printf("Ecco la lista dei candidati:\n");
	for(int i = 0; i < NCAND; i++){
		printf("%d) %d\n", i, *(c+i));
	}
	do{
		printf("\rScegli un candidato dalla lista:\t");
		scanf("%d", &choice);
		fflush(stdout);
	
		if(choice < 0 || choice >= NCAND){
			valid = -1;
			printf("Scelta non valida, riprovare.\n");
		} else {
			valid = 1;
			printf("Hai scelto: %d.\n", *(c+choice));
			chosen = *(c+choice);
		}
	}while(valid <= 0);
	return chosen;
}

int find_e(int dec, int zrime){
	int res = 0;
	while( ((res*dec)%zrime) != 1){
		printf("\rTestato: %d\t %d % %d = %d", res, res*dec, zrime, (res*dec)%zrime );
		res++;
		fflush(stdout);
		if( res >= FSAFE ){
			printf("Ci sto mettendo troppo, scegli un altro valore per d.\n");
			return -1;
		}
	} 
	return res;
}

int determine_block_size(){
	int k = 0;
	while( pow((float) 2, (float)k) <  n){
		k++;
	}
	return k;
}

void encrypt(char* m, int blk_sz, unsigned long int* out){
	int i = 0;

	char msg[MXLEN];
	for(int i = 0; i < MXLEN; i++){
		msg[i] = *(m+i);
	}
	char c = 'a';
	int broken_c = 0;

	struct block e_block;
	init_block(&e_block, blk_sz);
	rest_block(&e_block);
	printf("e_block.size: %d\n", e_block.size);	
	while( c != '\0'){
		sscanf(msg, "%c", &c);
		printf("\t\t%c\n", c);
		fflush(stdout);
		if(broken_c != 0) {
			c = c >> broken_c;
			e_block.blk[0] = c;
			i++;
			broken_c = 0;
			e_block.used += broken_c;

			out[i] = (unsigned long int) e_block.blk;
			out[i] = (unsigned long int) pow((float)out[i], (float)e);
			
			printf("%ld", out[i]);
			out[i] = out[i] % n;
			printf("%ld", out[i]);
			continue;
		}
		if(e_block.used + 8 <= e_block.size){
			printf("Scrivo in e_block\n");
			*(e_block.blk + e_block.used) = c;
			e_block.used += 8;
			printf("Finito la scrittura\n");

		} else if (e_block.used + 8 > e_block.size){
			printf("used>size\n");	
			c = c << (e_block.used - e_block.size);
			c = c >> (e_block.used - e_block.size);
			printf("scrivo il carattere\n");	
			strcat(e_block.blk, &c);
			printf("Ho scritto il carattere\n");
			broken_c = e_block.used - e_block.size;
			rest_block(&e_block);
			i--;
		} else if (e_block.used == e_block.size){
			
			out[i] = (unsigned long int) e_block.blk;
			out[i] = (unsigned long int) pow((float)out[i], (float)e);
			
			printf("%ld", out[i]);
			out[i] = out[i] % n;
			printf("%ld", out[i]);
		}
		i++;
	}

}

int main(int argc, char** argv){
	message = (char *) malloc(MXLEN*sizeof(char));
	unsigned long int cipher[MXLEN];
	if(argc >= 3){
		p = atoi(argv[1]);
		q = atoi(argv[2]);
	} else if (argc == 2){
		p = atoi(argv[1]);
		printf("Sembra che tu abbia dimenticato di inserire tutti i dati.\nInserisci q:\t");
		scanf("%d", &q);
	} else {
		printf("Sembra che tu abbia dimenticato di inserire i dati\n\rInserisci p:\t");
		scanf("%d", &p);
		fflush(stdout);
		printf("\rInserisci q:\t");
		scanf("%d", &q);
		fflush(stdout);
	}
	
	n = p*q;												//Determina anche la grandezza del mio blocco, devo determinarlo dinamicamente
	b_size = determine_block_size();
	z = (p-1)*(q-1);

	
	int candidati[NCAND];
	look_for_d(candidati);
	do{
		d = choose_d(candidati);
		e = find_e(d, z);
	}while(e <= 0);
	
	printf("\nChe messaggio vuoi criptare?\n");
	while( scanf(" %s ", message) > MXLEN){
		printf("Messaggio troppo lungo, riprova.\n MXLEN: %d, strlen: %d\n", MXLEN, strlen(message));
	}
	
	encrypt(message, b_size, cipher);

	printf("Messaggio in chiaro: %s\nChiave pubblica: (%d, %d)\nChiave privata: (%d, %d)\n", message, e, n, d, n);
	printf("Messaggio cifrato:\t");
	for(int i = 0; i < MXLEN; i++){
		printf("%ld\t", cipher[i]);
	}
	printf("\n");
}	
