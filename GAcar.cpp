#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define LM (1)		//memory unit
#define L1 (LM+2)	//layer 1 unit
#define L2 (5)		//layer 2 unti
#define L3 (LM+3)	//layer 3 unit
#define LL (L1 * L2 + L2 * L3)
#define PARENT (20)	//best parent
#define SON (5)	//son per unit
#define STEP (1000)	// test step
#define MG (1000)	//goal unit

#define VR (1)		// speed range
#define WR (0.5)	// degree speed range

#define PI (3.1415926)
#define RAND(MAX) (random() * (double)(MAX) / RAND_MAX)

//#define DEBUG

double active_function(double t){
	return 1/(1+exp(-t));
}

struct car{
	float gene[LL];
	int point;
};

struct goal{
	float x,y;	//goal always at (0,0) (x,y) is the start point of car
	float *wall;
	int n = 0;
};

int direction(float x1,float y1,float x2,float y2,float x3,float y3){

}
int intersect(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4){
	return 0;
}

void init_car(car* c){
	c->point = 0;
	int i;
	for(i = 0;i < LL;i++)
		c->gene[i] = RAND(1) - 0.5;
}

void init_goal(goal* g){
	int i;
	float cx,cy;
	if(g->n)
		free(g->wall);
	g->n = random() % 5;
	g->wall = (float*)malloc(4*g->n*sizeof(float));
	for(i = 0;i < g->n ;i++){
		cx = RAND(30);
		cy = RAND(30);
		g->wall[i*4+0] = cx + RAND(5);
		g->wall[i*4+1] = cy + RAND(5);
		g->wall[i*4+2] = cx + RAND(5);
		g->wall[i*4+3] = cy + RAND(5);
	}
	g->x = RAND(50);
	g->y = RAND(50);
}

int main(){
	int i,j,k,l,I,generation;
	float memory_t[2][20];
	float v,w,x,y,dx,dy,dv,dw;
	float hit;
	car best[2][PARENT],t;
	goal g[MG];
	for(i = 0;i < PARENT;i++)
		init_car(best[0]+i);
	
	for(generation = 0; generation < 100;generation++){
		for(i = 0;i < PARENT;i++){
			best[(generation + 1) & 1][i].point = -1;
		}
		for(i = 0;i < MG;i++)
			init_goal(g+i);
		for(I = 0;I < SON;I++)
			for(i = 0;i < PARENT;i++){
			t = best[generation & 1][i];
			t.gene[random()%LL] = RAND(1) - 0.5;
			t.point = 0;
			l = STEP;
			for(j = 0;j < LM;j++)
				memory_t[0][j] = RAND(0.01);
			//inti memory
			x = g[0].x;
			y = g[0].y;
			v = 0;
			w = 0;
#ifdef DEBUG
					printf("(%f,%f)",x,y);
#endif
			while(l--){
				//init input
				memory_t[0][LM+0] = - x * cos(w) - y * sin(w);
				memory_t[0][LM+1] = x * sin(w) - y * cos(w);
				//calc output
				for(j = 0;j < L2;j++){
					memory_t[1][j] = 0;
					for(k = 0;k < L1;k++){
						memory_t[1][j] += t.gene[j*L1+k] * memory_t[0][k];
					}
					memory_t[1][j] = active_function(memory_t[1][j]);
				}
				for(j = 0;j < L3;j++){
					memory_t[0][j] = 0;
					for(k = 0;k < L2;k++){
						memory_t[0][j] += t.gene[j*L2+k] * memory_t[1][k];
					}
					memory_t[0][j] = active_function(memory_t[0][j]);
				}
				dv = VR * (memory_t[0][LM+0] - 0.5);
				dw = WR * (memory_t[0][LM+1] - memory_t[0][LM+2]);
				v = sqrt((v + dv)*(v+dv) + dw * dw);
				w += asin(dw/v);
				if(v == 0)
					w = VR;
				if(w > PI)
					w -= 2 * PI;
				if(w <= -PI)
					w += 2 * PI;
				dx = x + v * cos(w);
				dy = y + v * sin(w);
				//if hit wall
				hit = 0;
				for(j = 0;j < g[t.point].n;j++)
					if(intersect(x,y,dx,dy,g[t.point].wall[0],g[t.point].wall[1],g[t.point].wall[2],g[t.point].wall[3]))
						hit = 1;
				//go
				x = dx;
				y = dy;
#ifdef DEBUG
					printf("\n\t-><%f,%f>[%f,%f](%f,%f)",dv,dw,v,w,x,y);
#endif
				//if finish
				if(abs(x)+abs(y) < 0.1){
					t.point++;
					x = g[t.point].x;
					y = g[t.point].y;
#ifdef DEBUG
					printf("\n(%f,%f)",x,y);
#endif
				}
			}
#ifdef DEBUG
			putchar(10);
#endif
			//if better
			j = -1;k = t.point;
			for(l = 0;l < PARENT;l++){
				if(best[(generation + 1) & 1][l].point < k) {
					j = l;
					k = best[(generation + 1) & 1][l].point;
				}
			}
			if(j != -1)
				best[(generation + 1) & 1][j] = t;
		}
		//print score board
		printf("%d:",generation);
		for(i = 0;i < PARENT;i++)
			printf("%5d",best[(generation  + 1) &1][i].point);
		putchar(10);
	}
	return 0;
}
