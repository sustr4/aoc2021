#include<stdio.h>
#include<stdlib.h>

void readInput() {

}

int main () {
	FILE * input;
	int bounds[450][6];
	size_t len = 0;
	ssize_t read;
	char *line = NULL;
	char status[450];
	int rulecount=0;
	int max[6];
	ssize_t shift;
	int i;
	int x,y,z,xsize,ysize;
	char *stick;
	long totalCount=0;


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

/*		fprintf(stderr,"[%d,%d],[%d,%d],[%d,%d]\n",
			bounds[rulecount][0],
			bounds[rulecount][1],
			bounds[rulecount][2],
			bounds[rulecount][3],
			bounds[rulecount][4],
			bounds[rulecount][5]);/**/

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
	if (line)
        free(line);

	for(z=max[4]; z<=max[5]; z++) {
		fprintf(stderr,"Counting sticks in stick %d\n",z);
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

		
				if((z==10)&&(y==10)){	
/*					for(x=0;x<xsize;x++) {
						printf("%s",stick[x]?"X":".");
					}
					printf("\n");/**/
				}
			}

			//Count lights
			int stickCount=0;
			for(x=0;x<xsize;x++)
				if(stick[x]) stickCount++;

	//		printf("%d on in this stick\n", stickCount);
			totalCount+=(long)stickCount;

			//free stick
			free(stick);
		}
	}	

	printf("Total Count %ld\n",totalCount);

	return 0;
}
