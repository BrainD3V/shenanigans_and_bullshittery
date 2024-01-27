#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#define NUMTHREADS 2
#define SPD        125
//structs
struct vect2{
	int 	x;
        int	y;
};

struct point{
	char symbol;
	struct vect2 pos;
	struct vect2 lpos;
	struct vect2 vel;
	struct vect2 acc;
};

struct display{
	int w, h;
	char** screen;
	char*  cmd;
};

struct t_args{
	struct display* 	d;
	struct point* 		pnt;
};

// globals
const int CMDLEN = 100;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct display disp;
struct point pnt;

// // point const
const int acc_decay = 0;
struct vect2 grav = {.x = 0, .y = -1};

// Vextor functions
void mod_vect(struct vect2* v, int new_x, int new_y){
	v->x = new_x;
	v->y = new_y;
}

void add_vect(struct vect2* v, int add_x, int add_y){
	v->x += add_x;
	v->y += add_y;
}

void cap_vect(struct vect2* v, int max_x, int max_y, int inv){
	if(inv == 0){
		v->x = (v->x >= max_x)? max_x: v->x;
		v->y = (v->y >= max_y)? max_y: v->y;
	} else {
		v->x = (v->x <= max_x)? max_x: v->x;
		v->y = (v->y <= max_y)? max_y: v->y;
	}
}


// Point functions
void point_create(struct point* p, int x, int y){
	p->symbol = '*';
	mod_vect(&(p->pos), x, y);
	mod_vect(&(p->lpos), x, y);
	mod_vect(&(p->acc), 0, grav.y);
}


void new_state(struct point* p, struct display* d){
	p->lpos.x = p->pos.x;
	p->lpos.y = p->pos.y;

	if(p->pos.x + p->vel.x >= d->w || p->pos.x + p->vel.x <= 0) {
		p->vel.x *= -1;

	//	p->vel.x -= p->pos.x;
		if(p->pos.x <= 0){
			p->pos.x = 1;
		} else if (p->pos.x + p->vel.x >= d->w) {
			p->pos.x = d->w - 1;
		}
	}

	if(p->pos.y + p->vel.y >= d->h || p->pos.y + p->vel.y <= 0){ 
		p->vel.y *= -1;
		p->vel.y -= p->pos.y;
		if(p->pos.y + p->vel.y <= 0){
			p->pos.y = 1;
		} else if(p->pos.y + p->vel.y >= d->h){
			p->pos.y = d->h - 2;
		}
	}
		
	add_vect(&(p->pos), p->vel.x, p->vel.y);
	add_vect(&(p->vel), p->acc.x, p->acc.y);
	add_vect(&(p->acc), acc_decay, acc_decay);
	add_vect(&(p->acc), 0, grav.y);
       	cap_vect(&(p->vel), -5, -5, 1);
	cap_vect(&(p->vel),  5,  5, 0);
	cap_vect(&(p->acc), -3, -3, 1);
	cap_vect(&(p->acc),  3,  3, 0);
}


// Screen Functions

void screen_init(struct display* d, int wid, int hei){
	d->w = wid;
	d->h = hei;
	d->screen = (char**) malloc(hei*sizeof(char*));
	d->cmd    = (char*) malloc(CMDLEN*sizeof(char));
	strcpy(d->cmd, "Input> ");
	for(int i = 0; i < hei; i++){
		d->screen[i] = (char*) malloc((wid+1)*sizeof(char));
		d->screen[i][0] = '|';
		for(int j = 1; j < wid-1; j++){
			d->screen[i][j] = ' ';
		}
		d->screen[i][(d->w)-1] = '|';
		d->screen[i][(d->w)] = '\n';
	}

}

void screen_show(struct display* d, struct point* p){
	int lpx = p->lpos.x;
	int lpy = p->lpos.y;

	int pox = p->pos.x;
	int poy = p->pos.y;
	
	int wid = d->w;
	int hei = d->h;
	
	
	d->screen[hei-lpy][wid-lpx] = ' ';
	d->screen[hei-poy][wid-pox] = p->symbol;
	
	for(int y = 0; y < d->h; y++){
		printf("%s", d->screen[y]);
	}
	for(int x = 0; x < d->w; x++) printf("=");
	printf("\n%s", d->cmd);
	printf("\n");
}

//Threads 
void* anim_thread(void *args){
	struct t_args *dispnt = args;
	while(1){
		
		pthread_mutex_lock(&mutex);
		
		screen_show(&disp, &pnt);
		new_state(&pnt, &disp);
		
		pthread_mutex_unlock(&mutex);
		
		usleep(SPD*1000);
		system("clear");
		printf("point state::> x= %d\t y= %d\t vx=%d\t vy=%d\t ax= %d\t ay= %d\n", pnt.pos.x, pnt.pos.y, pnt.vel.x, pnt.vel.y);
	}
	free(dispnt);
}

void* input_thread(void* args){
	struct t_args *dispnt = args; 
	char cmd[CMDLEN];
	char fun[4];
	int  x;
	int  y;
	
	while(1){
		strcpy(disp.cmd, "Input> ");
		if( scanf("%s", fun) > 0){
			pthread_mutex_lock(&mutex);
			sscanf(cmd, "%s", fun);
			strcat(disp.cmd, fun);
			pthread_mutex_unlock(&mutex);
		} else {
			continue;
		}

		if(strcmp(fun, "app") == 0){
			pthread_mutex_lock(&mutex);
			strcpy(disp.cmd, "App listened.\n x: ");
			scanf("%d", &x);
			strcpy(disp.cmd, "y: ");
			scanf("%d", &y);
			add_vect(&pnt.acc, x, y);
			pthread_mutex_unlock(&mutex);
		} else if (strcmp(fun, "kll") == 0){
			exit(EXIT_FAILURE);
		} else {
			printf("%s\n", cmd);
			printf("Comando non riconosciuto\n");
		}	
	}
}

int main(int argc, char** argv){
	if(argc != 5){
		fprintf(stderr, "Numero argomenti errato, inserire prima la dimensione dello schermo, poi la posizione del punto!\n");
		exit(EXIT_FAILURE);
	}
	
	int dwid = atoi(argv[1]);
	int dhei = atoi(argv[2]);

	int ppx = atoi(argv[3]);
	int ppy = atoi(argv[4]);
	
	printf("Creo il display\n");	
	screen_init(&disp, dwid, dhei);
	printf("Creo il punto\n");	
	point_create(&pnt, ppx, ppy);
	
	printf("Inizializzo gli argomenti per i thread\n");
	struct t_args *args = malloc(sizeof(struct t_args*));
	args->d = &disp;
	args->pnt = &pnt;

	pthread_t threads[NUMTHREADS];
	
	if( pthread_create(&threads[0], NULL, anim_thread, args) != 0){
		fprintf(stderr, "Errore nella creazione del thread animato: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("Creato thread per l'animazione\n");
	if( pthread_create(&threads[1], NULL, input_thread, args) != 0){
		fprintf(stderr, "Errore nella creazione del thread input: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("Creato thread per l'input.\n");
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
	
	free(args);
	exit(EXIT_SUCCESS);
}
