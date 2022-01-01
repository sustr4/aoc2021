#include<stdio.h>
#include<stdlib.h>

int comp(const void *a, const void *b)
{
  const int *da = (const int *) a;
  const int *db = (const int *) b;
  return (*da > *db) - (*da < *db);
}

int main (int argc, char *argv[]) {
	FILE * input;
	int bounds[450][6];
	size_t len = 0;
	ssize_t read;
	char *line = NULL;
	char status[450];
	int rulecount=0;
	int max[6]={0,0,0,0,0,0};
	ssize_t shift;
	int i,j;
	int x,y,z,xsize,ysize;
	char *stick;
	long totalCount=0;
	int *zees;

	input = fopen("day22-input", "r");
	if (input == NULL) exit(EXIT_FAILURE);

	while ((read = getline(&line, &len, input)) != -1) {
		if(line[1]=='n') {
			status[rulecount]=1;
			shift=3;
		}
		if(line[1]=='f') {
			status[rulecount]=0;
			shift=4;
		}

		sscanf(line+shift,"x=%d..%d,y=%d..%d,z=%d..%d",
			&bounds[rulecount][0],
			&bounds[rulecount][1],
			&bounds[rulecount][2],
			&bounds[rulecount][3],
			&bounds[rulecount][4],
			&bounds[rulecount][5]);

		for(i=0; i<6; i+=2) {
			if(bounds[rulecount][i]<max[i]) max[i]=bounds[rulecount][i];
			if(bounds[rulecount][i+1]>max[i+1]) max[i+1]=bounds[rulecount][i+1];
		}		
		rulecount++;


	}

	fprintf(stderr,"Extremes: [%d,%d],[%d,%d],[%d,%d]\n",
		max[0], max[1], max[2], max[3], max[4], max[5]);

	xsize=max[1]-max[0]+1;
	ysize=max[3]-max[2]+1;

	fclose(input);
	if (line) free(line);

	fprintf(stderr,"%d significant slices: ", rulecount*3);
	zees=malloc(rulecount*3*sizeof(int));
	for(j=0;j<rulecount;j++) {
		zees[j*3]=bounds[j][4];
		zees[j*3+1]=bounds[j][5];
		zees[j*3+2]=bounds[j][5]+1;
	}

	qsort(zees,rulecount*3,sizeof(int),comp);

	for(j=0;j<(rulecount*3)-1;j++) {
		if(zees[j]==zees[j+1]) {
			fprintf(stderr,"Layer %d mentioned twice. Skipping first occurrence\n",zees[j]);
			continue; }
		z=zees[j];
		fprintf(stderr,"Counting sticks in slice %d (<=%d)\n",z,max[5]);
		for(y=max[2]; y<=max[3]; y++) {

			// Allocate stick
			stick=calloc(xsize+2,sizeof(char));

			//Apply rules
			for(i=0; i<rulecount; i++) {
				if((z>=bounds[i][4])&&
					(z<=bounds[i][5])&&
					(y>=bounds[i][2])&& 
					(y<=bounds[i][3])) // Does this rule even apply
					
					for(x=bounds[i][0]; x<=bounds[i][1]; x++)
						stick[x-max[0]]=status[i];
		
			}

			//Count lights
			int stickCount=0;
			for(x=0;x<xsize;x++)
				if(stick[x]) stickCount++;

	//		printf("%d on in this stick\n", stickCount);
			totalCount+=(long)stickCount*(zees[j+1]-zees[j]);

			//free stick
			free(stick);
		}
	}	

	printf("%ld\n",totalCount);

	return 0;
}
