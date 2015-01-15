#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define LM (1)		//memory unit
#define L1 (LM+2)	//layer 1 unit
#define L2 (10)		//layer 2 unti
#define L3 (LM+3)	//layer 3 unit
#define LL (L1 * L2 + L2 * L3)
#define PARENT (20)	//best parent
#define SON (10)	//son per unit
#define STEP (1000)	// test step
#define MG (1000)	//goal unit

#define VR (1)		// speed range
#define WR (0.5)	// degree speed range

#define PI (3.1415926)

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
	float *wall[4];
	int n;
};

void init_car(car* c){
	c->point = 0;
	int i;
	for(i = 0;i < LL;i++)
		c->gene[i] = (random() - RAND_MAX / 2) * 1.0 / RAND_MAX;
}

void init_goal(goal* g){
	g->n = 0;
	g->x = random() * 5.0 / RAND_MAX;
	g->y = random() * 5.0 / RAND_MAX;
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
			t.gene[random()%LL] = (random()-RAND_MAX/2) / 1.0 / RAND_MAX;
			t.point = 0;
			l = STEP;
			for(j = 0;j < LM;j++)
				memory_t[0][j] = random() * 0.01 / RAND_MAX;
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
					if(false)
						hit = 1;
				//go
				x = dx;
				y = dy;
#ifdef DEBUG
					printf("\n\t-><%f,%f>[%f,%f](%f,%f)",dv,dw,v,w,x,y);
#endif
				//if finish
				if(abs(x)+abs(y) < 1){
					t.point++;
					x = g[t.point].x;
					y = g[t.point].y;
#ifdef DEBUG
					printf("\n(%f,%f)",x,y);
#endif
				}
			}
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
