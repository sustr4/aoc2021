#include<stdio.h>

long long p0wins=0, p1wins=0;

int thrWeight[10]={0,0,0,1,3,6,7,6,3,1};

int throw(int p0pos, int p1pos, int p0points, int p1points, int player, int throwVal, long weight) {
	int i;

	// Award points
	if(player) {
		p1pos=p1pos+throwVal;
		if(p1pos>10) p1pos=p1pos-10;
		p1points+=p1pos;
	} else {
		p0pos=p0pos+throwVal;
		if(p0pos>10) p0pos=p0pos-10;
		p0points+=p0pos;
	}

	// Win or continue
	if(p0points>20) p0wins+=weight;
	else if(p1points>20) p1wins+=weight;
	else for(i=3;i<10;i++)
		throw(p0pos, p1pos, p0points, p1points, !player, i, weight*thrWeight[i]);
}

int main () {
	int i;

	for(i=3;i<10;i++)
		throw(10, 8, 0, 0, 0, i, thrWeight[i]);

	printf("  P1 won %15ld times, P2 won %15ld times\n",p0wins, p1wins);
	printf("Winner: %ld\n",p0wins>p1wins?p0wins:p1wins);

	return 0;
}
