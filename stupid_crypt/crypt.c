/***********************************************************************************
 * 		Title: 		crypt.c
 * 		Author:		Matteo "BrainD3v" Cirinnà
 * 		Date:		25/02/2024
 *
 * 		Desciption:	The program will encrypt files using a normal
 * 				caesar encrypt method, maybe I'll thing something 
 * 				about the permutations block
 **********************************************************************************/

// Libraries

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>

// Defines
#define MAXFNM 50
#define MAXEXT 5
#define BUFLEN 100
#define KEYLEN 10
#define DATALN 15

// Global Variables


// Functions

int checkChar(char* str, int limit){
	int len = strlen(str);
	if( len > limit ){
		fprintf(stderr, "Error: String is too long\n");
		return EXIT_FAILURE;
	}
	for(int i = 0; i < len; i++){
		if( str[i] > 0xFF){
			fprintf(stderr, "Error: %c is not a valid character, use ASCII-only char\n");
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

void keygen(int* key){
	int k = *key;
	char* username = (char*) malloc(DATALN * sizeof(char));
	char* password = (char*) malloc(DATALN * sizeof(char));
	char* keystrng = (char*) malloc(DATALN * sizeof(char));

	printf("Input username: ");
	scanf("%s", username);
	while( checkChar(username, DATALN) != EXIT_SUCCESS ){
		printf("Input username: ");
		scanf("%s", username);
	}

	printf("Input password: ");
	scanf("%s", password);
	while( checkChar(password, DATALN) != EXIT_SUCCESS){
		printf("Input password: ");
		scanf("%s", password);
	} 
	
	sprintf(keystrng, "%d", k);

	// Pan the strings to the same length
	int diff = 0;
	if( strlen(username) > strlen(password)){
		diff = strlen(username) - strlen(password);
		for(int i = 0; i < diff; i++) strcat(password, "0");
	} else if( strlen(password) > strlen(username) ){
		diff = strlen(password) - strlen(username);
		for(int i = 0; i < diff; i++) strcat(username, "0");
	}
	
	if( strlen(username) > strlen(keystrng) ){
		
		diff = strlen(username) - strlen(keystrng);
		for(int i = 0; i < diff; i++) strcat(keystrng, "0");
	
	} else if ( strlen(keystrng) > strlen(username) ){
		
		diff = strlen(keystrng) - strlen(username);
		for(int i = 0; i < diff; i++){
			strcat(username, "0");
			strcat(password, "0");
		}
	}
	
	for(int i = 0; i < strlen(username); i++){
		keystrng[i] ^= username[i] ^ password[i];   
	}
	printf("Checking values\n\tusername: %s\n\tpassword: %s\n\tkeystring: %s\n", username, password, keystrng);
	
	int tmp = 0;
       	for(int i = 0; i < strlen(keystrng); i++){
		tmp += keystrng[i];
	}
	*key = tmp%0xFF;

	printf("The key is: %d\n", *key);
}

void caesar(char* in, char* out, int key){
	FILE* IN;
	FILE* OUT;
	char* buffer = (char*) malloc(BUFLEN * sizeof(char));

	if( (IN = fopen(in, "rb")) == NULL){
		fprintf(stderr, "Errore (in): %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if( (OUT = fopen(out, "wb+" )) == NULL  ){
		fprintf(stderr, "Errore (out): %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	struct stat in_stat;
	stat(in, &in_stat);
	size_t in_size = in_stat.st_size;
	size_t to_read = in_size;
	size_t len = BUFLEN;
	while( fread(buffer, (size_t) len, (size_t) 1, IN) != 0  ){
		for(int i = 0; i < len; i++) buffer[i] += key*i;
		if( fwrite(buffer, (size_t) len, (size_t) 1, OUT) == 0 ){
			fprintf(stderr, "Error: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		to_read -= BUFLEN;
		len = (to_read < BUFLEN)? to_read : BUFLEN;
	}
	printf("End of the encryption\n");
	fclose(IN);
	fclose(OUT);
	
}

void daesar(char* in, char* out, int key){
	FILE* IN;		//file to decrypt
	FILE* OUT;
       	char* buffer = (char*) malloc(BUFLEN * sizeof(char));
	
	if( (IN = fopen(in, "rb")) == NULL){
		fprintf(stderr, "Errore (in): %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if( (OUT = fopen(out, "wb+" )) == NULL  ){
		fprintf(stderr, "Errore (out): %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	struct stat in_stat;
	stat(in, &in_stat);
	size_t in_size = in_stat.st_size;
	size_t to_read = in_size;
	size_t len = BUFLEN;
	while( fread(buffer, (size_t) len, (size_t) 1, IN) != 0  ){
		for(int i = 0; i < len; i++) buffer[i] -= key*i;
		if( fwrite(buffer, (size_t) len, (size_t) 1, OUT) == 0 ){
			fprintf(stderr, "Error: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		to_read -= BUFLEN;
		len = (to_read < BUFLEN)? to_read : BUFLEN;
	}
	printf("End of Decrypt\n");
	fclose(IN);
	fclose(OUT);
}

// Main

int main(int argc, char* argv[]){
	char* in_filename;
       	char* out_filename;
	int key = 1;
	int mode = -1;
	in_filename  = (char*) malloc(MAXFNM * sizeof(char));
	out_filename = (char*) malloc(MAXFNM * sizeof(char));
	
	if(argc == 1){
		printf("Non hai immesso nessun file da cifrare, per favore, mettine uno: ");
		scanf("%s", in_filename);
	} else  {
		if( strlen(argv[1]) > MAXFNM ){
			fprintf(stderr, "Errore: Nome del file troppo lungo\n");
			exit(EXIT_FAILURE);
		}
		sprintf(in_filename, "%s", argv[1]);
		if( argc == 2  ){
			key = atoi(argv[1]);
			printf("key set: %d\n", key);
		}
	}

	printf("Il nome del file inserito è: %s\n", in_filename);
	keygen(&key);
	char* extention = (char*) malloc(MAXEXT * sizeof(char));
	sscanf(in_filename, "%[^.].%s", out_filename, extention);

	printf("Choose:\n [0] Crypt\n [1] Decrypt\nChoice: ");
	scanf("%d", &mode);
	switch(mode){
		case 0:
			printf("Crypt\n");
			strcat(out_filename, "_enc.");
			strcat(out_filename, extention);
			caesar(in_filename, out_filename, key);
			break;

		case 1:
			printf("Decrypt\n");
			strcat(out_filename, "_dec.");
			strcat(out_filename, extention);
			daesar(in_filename, out_filename, key);
			break;
		default:
			printf("Error: Mode not defined\n");
			break;
	}

}
