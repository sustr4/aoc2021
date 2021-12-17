#include<stdio.h>
#include<limits.h>

#define YMAX -225
#define YMIN -267
#define XMIN 14
#define XMAX 50 /**/

/*#define YMAX -5
#define YMIN -10
#define XMIN 20
#define XMAX 30 /**/

int main () {
	long long y,sdy,dy,max,x,sdx,dx,i,j=0;

	for(sdy=YMIN;sdy<LLONG_MAX;sdy++) { // Try all possible vertical speeds
		for(sdx=0;sdx<=XMAX;sdx++) { // Try all horizontal speeds that do not overshoot at first step
			x=0; y=0; i=1; max=0;
			dy=sdy; dx=sdx;
			while(y>=YMIN) {
				if(!dy) max=y;
				if(y<=YMAX && x>=XMIN && x<=XMAX) {
					printf("%ld: Starting speed [%ld,%ld], %ld steps, current position [%ld,%ld], max height %ld\n",++j,sdx,sdy,i,x,y,max); break; }
				y+=dy--;	// step height, fall faster
				x+=dx; if(dx>0) dx--; // progress slower
				i++;
			}
		}
	}
	return 0;
}
