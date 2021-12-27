#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>
#include<limits.h>

#define MAXDEPTH 25
#define EXITONFIRST 0

int depth=0;
/*
#############
#...........#
###D#A#C#C###
  #D#C#B#A#
  #D#B#A#C#
  #D#A#B#B#
  #########
*/

long totalChecks=0;
long solutions=0;

int minpoints=INT_MAX;

typedef char Tcache[7];
typedef char Tburrow[4][4];

Tcache INITcache={0,0,  0,  0,  0,  0,0};
Tburrow INITburrow={{'D','D','D','D'},
		 {'A','C','B','A'},
		 {'C','B','A','B'},
		 {'C','A','C','B'}};/**/

Tcache *cacheHistory;
Tburrow *burrowHistory;

/*
char INITcache[]={0,0,  0,  0,  0,  0,0};
char INITburrow[][4]={
		{'A','A','A','A'},
		{'C','B','B','B'},
		{'B','C','C','C'},
		{'D','D','D','D'}}; /**/
int distance[7][4][4]={
/*0*/		{{3,5,7,9},
		 {4,6,8,10},
		 {5,7,9,11},
		 {6,8,10,12}},

/*1*/		{{2,4,6,8},
		 {3,5,7,9},
		 {4,6,8,10},
		 {5,7,9,11}},

/*2*/		{{2,2,4,6},
		 {3,3,5,7},
		 {4,4,6,8},
		 {5,5,7,9}},

/*3*/		{{4,2,2,4},
		 {5,3,3,5},
		 {6,4,4,6},
		 {7,5,5,7}},

/*4*/		{{6,4,2,2},
		 {7,5,3,3},
		 {8,6,4,4},
		 {9,7,5,5}},

/*5*/		{{8,6,4,2},
		 {9,7,5,3},
		 {10,8,6,4},
		 {11,9,7,5}},

/*6*/		{{9,7,5,3},
		 {10,8,6,4},
		 {11,9,7,5},
		 {12,10,8,6}}};

int printMap(Tcache cache, Tburrow burrow);

int checkVictory(Tcache cache, Tburrow burrow) {
	int retVal=1;
	int i;
	if (retVal) for(i=0;i<3;i++) if(burrow[1][i]!='B') { retVal=0; break; }
	if (retVal) for(i=0;i<3;i++) if(burrow[0][i]!='A') { retVal=0; break; }
	if (retVal) for(i=0;i<3;i++) if(burrow[3][i]!='D') { retVal=0; break; }
	if (retVal) for(i=0;i<3;i++) if(burrow[2][i]!='C') { retVal=0; break; }

//	if(retVal) printMap(cache,burrow);
//	printMap(cache,burrow);

	totalChecks++;
	return retVal;
}

int inCacheCost(int step, char Ch) {
	int price;

	switch(Ch) {
	case 'A':
		price=1; break;
	case 'B':
		price=10; break;
	case 'C':
		price=100; break;
	case 'D':
		price=1000; break;
	}

	return(step*price);
}

int cost(int cache, int burrowDepth, int burrowNo, char Ch) {
	int price;

	switch(Ch) {
	case 'A':
		price=1; break;
	case 'B':
		price=10; break;
	case 'C':
		price=100; break;
	case 'D':
		price=1000; break;
	}

	return(distance[cache][burrowDepth][burrowNo]*price);
}


int b2bcost(int b, int d, int bt, int dt, char Ch) {
	int price;

	switch(Ch) {
	case 'A':
		price=1; break;
	case 'B':
		price=10; break;
	case 'C':
		price=100; break;
	case 'D':
		price=1000; break;
	}

	return((d+1+dt+1+2*(abs(bt-b)))*price);
}

char ch2ch(char c) {
	return(c?c:'.');
}

int printMap(Tcache cache, Tburrow burrow) {

	printf("\n############# (%d)\n", depth);
	printf("#%c%c.%c.%c.%c.%c%c#\n",ch2ch(cache[0]),ch2ch(cache[1]),ch2ch(cache[2]),ch2ch(cache[3]),ch2ch(cache[4]),ch2ch(cache[5]),ch2ch(cache[6]));
	printf("###%c#%c#%c#%c###\n",ch2ch(burrow[0][0]),ch2ch(burrow[1][0]),ch2ch(burrow[2][0]),ch2ch(burrow[3][0]));
	printf("  #%c#%c#%c#%c#  \n",ch2ch(burrow[0][1]),ch2ch(burrow[1][1]),ch2ch(burrow[2][1]),ch2ch(burrow[3][1]));
	printf("  #%c#%c#%c#%c#  \n",ch2ch(burrow[0][2]),ch2ch(burrow[1][2]),ch2ch(burrow[2][2]),ch2ch(burrow[3][2]));
	printf("  #%c#%c#%c#%c#  \n",ch2ch(burrow[0][3]),ch2ch(burrow[1][3]),ch2ch(burrow[2][3]),ch2ch(burrow[3][3]));
	printf("  #########\n");

}

int move(char *oldcache, char oldburrow[][4], int currentPrice) {
	Tcache cache;
	Tburrow burrow;
	int b,c,d,bt,dt; // b .. burrow, c .. cache, d .. depth
	int m,n,i;
	int freePath;

	if(checkVictory(oldcache,oldburrow)) {
		if(currentPrice<minpoints) minpoints=currentPrice;
//		printMap(oldcache,oldburrow);
//		if(!EXITONFIRST)
//			printf("Solution worth %d in %d steps, Current minimum price %d\n",currentPrice, depth, minpoints);

		if((depth<=26) && (currentPrice<=minpoints)) {
			solutions++;
//			for(i=0; i<depth; i++) printMap(cacheHistory[i],burrowHistory[i]);
		}
		return 1;
	}

//	printf("Currently %d points at this:\n", currentPrice);


	depth++;
	if(depth>MAXDEPTH) { //safety cutoff
//		printf("Safety cutoff\n");
		depth--;
		return 1; 
	}
	if(currentPrice>minpoints) {
//		printf("Price cutoff (%d>%d)\n", currentPrice, minpoints);
		depth--;
		return 1; //points cutoff (this path too costly)
	}

	memcpy(cache,oldcache,7*sizeof(char));
	memcpy(burrow,oldburrow,16*sizeof(char));

//	printMap(cache,burrow);

	// From burrow to burrow
	for(b=0; b<4; b++) { // Iterate through burrows
//		printMap(cache, burrow);
		for(d=0; d<4; d++) {
			//printf("b=%d, d=%d, burrow[d][b]=%c\n",b, d, burrow[b][d]);
			if(burrow[b][d]) break; }
		if(d>=4) continue;
//		printf("first movable in burrow %d is depth %d\n",b,d);

		// do not leave home unnecessarily
		int same=1;
		for(i=d; i<4; i++) {
			if(burrow[b][i]!=(char)b+'A') { same=0; break; }
		}
		if(same) continue;

		//Target burrow
		bt=burrow[b][d]-'A';
		for(dt=3; burrow[bt][dt]; dt--) {
			if(dt<0) break;
			if(burrow[bt][dt]!=burrow[b][d]) { // There is stille a different-coloured piece
				dt=-1;
				break;
			}
		}
		if(dt<0) continue;

		//Is path free?
		b2b:
		freePath=1;

		if(bt<b) {// This pos. is left of burrow
			for(i=bt+2; i<(b+2); i++) 
				if(cache[i])
					freePath=0; }
		else {// This pos. is right of burrow
			for(i=b+2; i<(bt+2); i++)
				if(cache[i])
					freePath=0;}

//			printf("path to cache %d from burrow %d is %sfree\n", c, b, freePath?"":"not ");

		if(freePath) {
			burrow[bt][dt]=burrow[b][d];
			burrow[b][d]=0;
//				printf("d%02d: Moving %c(%d) from burrow %d, depth %d, to cache %d\n",depth,cache[c],cache[c],b,d,c);
//				printMap(cache, burrow);
//			cacheHistory[depth]=(Tcache*)cache;
//			burrowHistory[depth]=(Tburrow*)burrow;
			move(cache, burrow, currentPrice+b2bcost(b,d,bt,dt,burrow[bt][dt]));
			if(solutions&&EXITONFIRST) {
				printf("\n%c from burrow %d to burrow %d:", burrow[bt][dt], b, bt);
				printMap(cache,burrow);
				return 0;
			}
			// Reset board
			memcpy(cache,oldcache,7*sizeof(char));
			memcpy(burrow,oldburrow,16*sizeof(char));
		}
	}


//	printMap(cache, burrow);
	// From cache to burrow
	for(c=0; c<7; c++) {
		if(!cache[c]) continue;
		b=(int)cache[c]-'A';
		for(d=3; burrow[b][d]; d--) {
			if(d<0) break;
			if(burrow[b][d]!=cache[c]) { // There is stille a different-coloured piece
				d=-1;
				break;
			}
		}
		if(d<0) continue;
//		printf("%c from cache %d can move. It should go to burrow %d, depth %d\n", cache[c], c, b, d);
//		printMap(cache, burrow);

		//Is path free?
		freePath=1;

		if(c<(b+2)) // This pos. is left of burrow
			for(i=c+1; i<(b+2); i++) if(cache[i]) freePath=0;
		if(c>=(b+2)) // This pos. is right of burrow
			for(i=(b+2); i<=c-1; i++) if(cache[i]) freePath=0;

//		printf("path from cache %d to burrow %d is %sfree\n", c, b, freePath?"":"not ");

		if(freePath) {
			burrow[b][d]=cache[c];
			cache[c]=0;
//				printf("d%02d: Moving %c(%d) from burrow %d, depth %d, to cache %d\n",depth,cache[c],cache[c],b,d,c);
//				printMap(cache, burrow);
			move(cache, burrow, currentPrice+cost(c, d, b, burrow[b][d]));
			if(solutions&&EXITONFIRST) {
				printf("\n%c from cache %d to burrow %d:", burrow[b][d], c, b);
				printMap(cache,burrow);
				return 0;
			}
			// Reset board
			memcpy(cache,oldcache,7*sizeof(char));
			memcpy(burrow,oldburrow,16*sizeof(char));
		}

	}


	// From burrow to cache
	for(b=0; b<4; b++) { // Iterate through burrows
//		printMap(cache, burrow);
		for(d=0; d<4; d++) {
			//printf("b=%d, d=%d, burrow[d][b]=%c\n",b, d, burrow[b][d]);
			if(burrow[b][d]) break; }
		if(d==4) continue;
//		if (depth==1) printf("first movable in burrow %d is depth %d\n",b,d);

		// do not leave home unnecessarily
		int same=1;
		for(i=d; i<4; i++) {
			if(burrow[b][i]!=(char)b+'A') { same=0; break; }
		}
		if(same) continue;

		//Choose cache
		for(c=0;c<7;c++) {
			//Is path free?
			freePath=1;

			if(c<(b+2)) // This pos. is left of burrow
				for(i=c; i<(b+2); i++) if(cache[i]) freePath=0;
			if(c>=(b+2)) // This pos. is right of burrow
				for(i=(b+2); i<=c; i++) if(cache[i]) freePath=0;

//			printf("path to cache %d from burrow %d is %sfree\n", c, b, freePath?"":"not ");

			if(freePath) {
				cache[c]=burrow[b][d];
				burrow[b][d]=0;
//				printf("d%02d: Moving %c(%d) from burrow %d, depth %d, to cache %d\n",depth,cache[c],cache[c],b,d,c);
//				printMap(cache, burrow);
				move(cache, burrow, currentPrice+cost(c, d, b, cache[c]));
				if(solutions&&EXITONFIRST) {
					printf("\n%c from burrow %d to cache %d:", cache[c], b, c);
					printMap(cache,burrow);
					return 0;
				}
				// Reset board
				memcpy(cache,oldcache,7*sizeof(char));
				memcpy(burrow,oldburrow,16*sizeof(char));
			}
		}
	}

/*
	// From cache to cache
	for(c=0; c<7; c++) {
		if(!cache[c]) continue;
//		printf("%02d: %c from cache %d can move elsewhere in the cache\n", depth, cache[c], c);
	
//		printMap(cache, burrow);
		// Try left
		for(i=c-1; (!cache[i])&&(i>=0); i--) {
			cache[i]=cache[c];
			cache[c]=0;
//			printMap(cache, burrow);
			move(cache, burrow, currentPrice+inCacheCost(c-i,cache[i]));
			cache[c]=cache[i];
			cache[i]=0;
		}	
		// Try right
		for(i=c+1; (!cache[i])&&(i<7); i++) {
			cache[i]=cache[c];
			cache[c]=0;
//			printMap(cache, burrow);
			move(cache, burrow, currentPrice+inCacheCost(i-c,cache[i]));
			cache[c]=cache[i];
			cache[i]=0;
		}
	}*/

	depth--;
//	printf("move() exit\n");
	return 0;
}

int main () {

//	cacheHistory=malloc(MAXDEPTH*sizeof(Tcache*));
//	burrowHistory=malloc(MAXDEPTH*sizeof(Tburrow*));

//	cacheHistory[0]=(Tcache*)INITcache;
//	burrowHistory[0]=(Tburrow*)INITburrow;

	printMap(INITcache, INITburrow);

	move(INITcache, INITburrow, 0);

	printf("%ld positions checked\n",totalChecks);
	printf("Minimum price found %d.\n",minpoints);

	return 0;

}

