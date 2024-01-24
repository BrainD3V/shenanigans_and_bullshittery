#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#define NUMTHREADS 2
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
};

struct t_args{
	struct display* 	d;
	struct point* 		pnt;
};

// constants
const int CMDLEN = 25;
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

		p->vel.x -= p->pos.x;
		if(p->pos.x <= 0){
			p->pos.x = 0;
		} else if (p->pos.x + p->vel.x >= d->w) {
			p->pos.x = d->w;
		}
	}

	if(p->pos.y + p->vel.y >= d->h || p->pos.y + p->vel.y <= 0){ 
		p->vel.y *= -1;
		p->vel.y -= p->pos.y;
		if(p->pos.y + p->vel.y <= 0){
			p->pos.y = 0;
		} else if(p->pos.y + p->vel.y >= d->h){
			p->pos.y = d->h;
		}
	}
		

	add_vect(&(p->pos), p->vel.x, p->vel.y);
	add_vect(&(p->vel), p->acc.x, p->acc.y);
	add_vect(&(p->acc), acc_decay, acc_decay);
	mod_vect(&(p->acc), 0, grav.y);
       	cap_vect(&(p->vel), -5, -5, 1);
	cap_vect(&(p->acc), -3, -3, 1);	
}


// Screen Functions

void screen_init(struct display* d, int wid, int hei){
	d->w = wid;
	d->h = hei;
	d->screen = (char**) malloc(hei*sizeof(char*));
	for(int i = 0; i < hei; i++){
		d->screen[i] = (char*) malloc((wid+1)*sizeof(char));
		for(int j = 0; j < wid; j++){
			d->screen[i][j] = ' ';
		}
		d->screen[i][wid] = '\n';
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
}

//Threads 
void* anim_thread(void *args){
	struct t_args *dispnt = args;
	struct display disp = *dispnt->d;
	struct point   pnt  = *dispnt->pnt;
	while(1){
		screen_show(&disp, &pnt);
		new_state(&pnt, &disp);
		usleep(250*1000);
		printf("point state::> x= %d\t y= %d\t vx=%d\t vy=%d\n", pnt.pos.x, pnt.pos.y, pnt.vel.x, pnt.vel.y);	
	}
	free(dispnt);
}

void* input_thread(void* args){
	struct t_args *dispnt = args; 
	struct point pnt = *dispnt->pnt;
	char cmd[CMDLEN];
	char fun[3];
	int  x;
	int  y;
	while(1){
		system("clear");
		printf("Input>");
		scanf("%s", cmd);
		sscanf(cmd, "%s %d %d", fun, x, y);
		if(strcmp(fun, "app") == 0){
			add_vect(&pnt.acc, x, y);
		} else if (strcmp(fun, "kll") == 0){
			break;
		} else {
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
	struct display disp;
	screen_init(&disp, dwid, dhei);
	printf("Creo il punto\n");
	struct point pnt;
	point_create(&pnt, ppx, ppy);

	struct t_args *args = malloc(sizeof(struct t_args*));
	args->d = &disp;
	args->pnt = &pnt;

	pthread_t threads[NUMTHREADS];
	
	pthread_create(&threads[0], NULL, anim_thread, args);
	pthread_create(&threads[1], NULL, input_thread, args);

	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
	
	free(args);
	exit(EXIT_SUCCESS);
}
