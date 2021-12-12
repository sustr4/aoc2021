#include <stdio.h>
#include <malloc.h>

int sumArray(int arr[12][12]) {
	int i,j,sum=0;
	for(j=1;j<=10;j++) for(i=1;i<=10;i++) sum+=arr[j][i];
	return sum;
}

int main() {

	int round,i,j,o,p,lastsum=0,expl=0;
	int octos[12][12]={
		{0,0,0,0,0,0,0,0,0,0,0,0},
		{0,5,4,3,3,5,6,6,2,7,6,0},
		{0,6,3,7,6,2,5,3,4,3,8,0},
		{0,8,4,5,8,6,3,6,3,1,6,0},
		{0,6,2,5,3,2,5,4,5,2,5,0},
		{0,7,2,1,1,1,3,7,1,3,8,0},
		{0,1,4,1,1,5,2,6,5,3,2,0},
		{0,5,7,8,8,7,6,1,4,2,4,0},
		{0,8,6,7,7,8,4,1,5,1,4,0},
		{0,1,6,2,2,3,3,1,6,3,1,0},
		{0,5,8,7,6,7,1,2,2,2,7,0},
		{0,0,0,0,0,0,0,0,0,0,0,0}};

	round=1;
	while(sumArray(octos)) {

		// 1st, Increment all
		for(j=1;j<=10;j++) for(i=1;i<=10;i++) octos[j][i]+=1;


		// 2nd Explode all
		int octonotes[12][12]={
			{0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0}};
		

		while (lastsum != sumArray(octos)) {
			lastsum=sumArray(octos);
			for(j=1;j<=10;j++)
				for(i=1;i<=10;i++)
					if ( !octonotes[j][i] && octos[j][i]>=10) {
						for (o=j-1; o<=j+1; o++)
							for (p=i-1; p<=i+1; p++)
								octos[o][p]++;
						octonotes[j][i]++;
					}
		}

		// 3rd Reset and count explosions
		for(j=1;j<=10;j++)
			for(i=1;i<=10;i++)
				if(octos[j][i]>=10) {
					octos[j][i]=0;
					expl++; }

		if (round==100) printf("Total explosions after %d rounds: %d\n",round,expl);
		round++;	
	}
	printf("Exiting after round %d\n", round-1);

	return 0;
}
