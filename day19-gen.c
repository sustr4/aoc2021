#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define SCANNERS 36
#define MAXBEACONS 28

#define EXPECTED 168668016

long comparisonsMade=0;

const int trans[24][3][3] = {
{{ 1, 0, 0}, { 0, 1, 0}, { 0, 0, 1 }},
{{ 0, -1, 0}, { 1, 0, 0}, { 0, 0, 1 }},
{{ -1, -0, 0}, { 0, -1, 0}, { 0, 0, 1 }},
{{ -0, 1, 0}, { -1, -0, 0}, { 0, 0, 1 }},
{{ 0, 0, 1}, { 0, 1, 0}, { -1, 0, 0 }},
{{ 0, 1, 0}, { 0, 0, -1}, { -1, 0, 0 }},
{{ 0, 0, -1}, { 0, -1, -0}, { -1, 0, -0 }},
{{ 0, -1, -0}, { 0, -0, 1}, { -1, -0, 0 }},
{{ 0, 0, -1}, { 0, 1, 0}, { 1, 0, 0 }},
{{ 0, -1, -0}, { 0, 0, -1}, { 1, 0, 0 }},
{{ 0, -0, 1}, { 0, -1, -0}, { 1, 0, -0 }},
{{ 0, 1, 0}, { 0, -0, 1}, { 1, -0, 0 }},
{{ 1, 0, 0}, { 0, 0, -1}, { 0, 1, 0 }},
{{ 0, 0, 1}, { 1, 0, -0}, { -0, 1, 0 }},
{{ -1, 0, 0}, { 0, 0, 1}, { -0, 1, -0 }},
{{ -0, 0, -1}, { -1, 0, 0}, { 0, 1, 0 }},
{{ 1, 0, 0}, { 0, 0, 1}, { 0, -1, 0 }},
{{ 0, 0, 1}, { -1, 0, 0}, { -0, -1, 0 }},
{{ -1, 0, 0}, { -0, 0, -1}, { -0, -1, -0 }},
{{ -0, 0, -1}, { 1, 0, -0}, { 0, -1, 0 }},
{{ 1, 0, 0}, { 0, -1, -0}, { 0, 0, -1 }},
{{ 0, -1, 0}, { -1, -0, -0}, { 0, 0, -1 }},
{{ -1, -0, 0}, { -0, 1, -0}, { 0, -0, -1 }},
{{ -0, 1, 0}, { 1, 0, -0}, { -0, -0, -1 }}};

typedef struct {
	double x;
	double y;
	double z; } TDblPoint;

typedef struct {
	int x;
	int y;
	int z; } TPoint;

typedef struct {
	int stddev;
	int middle;
	int thickness;
	int thinness; } TCharacteristic;

typedef struct {
	TCharacteristic ch;
	char scn;
	char bec[12];
} TCandidate;

int equalsCh(TCharacteristic f, TCharacteristic s) {
	if((f.stddev==s.stddev)&&
		(f.middle==s.middle)&&
		(f.thickness==s.thickness)&&
		(f.thinness==s.thinness)) return 1;
	return 0;
}

TPoint transPoint(int t, TPoint a) {
	int i,j;
	TPoint retVal;

	retVal.x=trans[t][0][0]*a.x+trans[t][0][1]*a.y+trans[t][0][2]*a.z;
	retVal.y=trans[t][1][0]*a.x+trans[t][1][1]*a.y+trans[t][1][2]*a.z;
	retVal.z=trans[t][2][0]*a.x+trans[t][2][1]*a.y+trans[t][2][2]*a.z;

	return retVal;
}

int compPoint (const void * first, const void * second) {
    TPoint f = *((TPoint*)first);
    TPoint s = *((TPoint*)second);
    if ((f.x==s.x)&&(f.y==s.y)&&(f.z==s.z)) return 0;
    if (f.x < s.x) return -1;
    if ((f.x==s.x)&&(f.y<s.y)) return -1;
    if ((f.x==s.x)&&(f.y==s.y)&&(f.z<s.z)) return -1;
    return 1;
}

int compChar (const void * first, const void * second) {
    TCharacteristic f = *((TCharacteristic*)first);
    TCharacteristic s = *((TCharacteristic*)second);
    if (f.stddev<(s.stddev)) return -1;
    if (f.stddev>(s.stddev)) return  1;
    if (f.thickness<s.thickness) return -1;
    if (f.thickness>s.thickness) return  1;
    if (f.thinness<s.thinness) return -1;
    if (f.thinness>s.thinness) return  1;
    if (f.middle<s.middle) return -1;
    if (f.middle>s.middle) return  1;
    return 0;
}

int comp (const void * first, const void * second) {
    int f = *((int*)first);
    int s = *((int*)second);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}


int compCandidate (const void * first, const void * second) {
	TCandidate f = *((TCandidate*)first);
	TCandidate s = *((TCandidate*)second);
	int ch;
	ch=compChar(&(f.ch),&(s.ch));
	if(ch) return(ch);
	return comp(&(f.scn),&(s.scn));
}



TPoint **readInput() {
	FILE * input;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int i,j;
	TPoint **point;

	point=calloc(SCANNERS+1,sizeof(*point));

	input = fopen("day19-input.txt", "r");
	if (input == NULL) exit(EXIT_FAILURE);

	while ((read = getline(&line, &len, input)) != -1) {
		if(strlen(line)>6) {
			if(!strncmp("---",line,3)) { // next scanner
				sscanf(line,"--- scanner %d ---",&j);
				point[j]=calloc(MAXBEACONS,sizeof(TPoint));
				i=0;
			} else {
				sscanf(line,"%d,%d,%d",
					&(point[j][i].x),
					&(point[j][i].y),
					&(point[j][i].z));
				i++;
			}
		}
	}

	for(j=0; point[j]; j++)
		for(i=0; point[j][i].x || point[j][i].y || point[j][i].z ; i++)
			fprintf(stderr,"Scn %2d, point #%02d: [%d, %d, %d]\n", j, i, point[j][i].x, point[j][i].y, point[j][i].z);


	fclose(input);
	if (line)
        free(line);

	return point;
}


TCharacteristic stddev(int scn, char *iArr, TPoint **points) {
	int i,j;
	int dev=0;
	TPoint max={0,0,0};
	TPoint min={0,0,0};
	TCharacteristic retVal;
	int dim[3];

	for(i=0;i<12;i++) {
		for(j=0;j<12;j++) {
			dev+=abs(points[scn][iArr[i]].x-points[scn][iArr[j]].x)+
			     abs(points[scn][iArr[i]].y-points[scn][iArr[j]].y)+
			     abs(points[scn][iArr[i]].z-points[scn][iArr[j]].z);
		}
	       if(points[scn][iArr[i]].x>max.x)
		       max.x=points[scn][iArr[i]].x;
	       if(points[scn][iArr[i]].y>max.y)
		       max.y=points[scn][iArr[i]].y;
	       if(points[scn][iArr[i]].z>max.z)
		       max.z=points[scn][iArr[i]].z;
	       if(points[scn][iArr[i]].x<min.x)
		       min.x=points[scn][iArr[i]].x;
	       if(points[scn][iArr[i]].y<min.y)
		       min.y=points[scn][iArr[i]].y;
	       if(points[scn][iArr[i]].z<min.z)
		       min.z=points[scn][iArr[i]].z;
	}

	dim[0]=max.x-min.x;
	dim[1]=max.y-min.y;
	dim[2]=max.z-min.z;

	qsort(dim, 3, sizeof(int), comp);

	retVal.stddev=dev;
	retVal.thinness=dim[0];
	retVal.middle=dim[1];
	retVal.thickness=dim[2];
//	retVal.thinness=0;
//	retVal.middle=0;
//	retVal.thickness=0;

	fprintf(stderr,"stddev=%d\tthin=%5d mid=%5d thick=%d5\n",retVal.stddev, retVal.thinness, retVal.middle, retVal.thickness);
	return (retVal);
} 

TCandidate * candidates(TPoint** points) {
	char iArr[12];
	int i,j,jj;
	int *beaconsmax = calloc(SCANNERS,sizeof(int));
	int scn;
	long count=0;
	TCandidate *candidates,*goodCandidates;

	for(j=0; points[j]; j++)
		for(i=0;i<MAXBEACONS;i++) {
//			printf("Scanner %d, beacon %d, [%d,%d,%d]\n",j,i,
//				points[j][i].x, points[j][i].y, points[j][i].z);
			if((points[j][i].x!=0) &&
			    (points[j][i].y!=0) &&
			    (points[j][i].z!=0)) beaconsmax[j]=i;
		}

	for(j=0; points[j];j++) printf("Scanner %d sees %d(+1) beacons\n",j,beaconsmax[j]);
	fprintf(stderr,"Counting candidates ... \n");

	count=0;

	for(scn=0;points[scn];scn++) {
	 for(iArr[0]=0; iArr[0]<=beaconsmax[scn]; iArr[0]++)
	  for(iArr[1]=iArr[0]+1; iArr[1]<=beaconsmax[scn]; iArr[1]++)
	   for(iArr[2]=iArr[1]+1; iArr[2]<=beaconsmax[scn]; iArr[2]++)
	    for(iArr[3]=iArr[2]+1; iArr[3]<=beaconsmax[scn]; iArr[3]++)
	     for(iArr[4]=iArr[3]+1; iArr[4]<=beaconsmax[scn]; iArr[4]++)
	      for(iArr[5]=iArr[4]+1; iArr[5]<=beaconsmax[scn]; iArr[5]++)
	       for(iArr[6]=iArr[5]+1; iArr[6]<=beaconsmax[scn]; iArr[6]++)
	        for(iArr[7]=iArr[6]+1; iArr[7]<=beaconsmax[scn]; iArr[7]++)
	 	for(iArr[8]=iArr[7]+1; iArr[8]<=beaconsmax[scn]; iArr[8]++)
	 	 for(iArr[9]=iArr[8]+1; iArr[9]<=beaconsmax[scn]; iArr[9]++)
	 	  for(iArr[10]=iArr[9]+1; iArr[10]<=beaconsmax[scn]; iArr[10]++)
	 	   for(iArr[11]=iArr[10]+1; iArr[11]<=beaconsmax[scn]; iArr[11]++) {
			fprintf(stderr,"Scn %d, Set %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
				scn,iArr[0],iArr[1],iArr[2],iArr[3],iArr[4],iArr[5],iArr[6],iArr[7],iArr[8],iArr[9],iArr[10],iArr[11]);/**/
			count ++;
			}
		fprintf(stderr, "%d candidates with scanner %d included\n", count, scn);
	}

	candidates=malloc(count*sizeof(TCandidate));

	count=0;
	for(scn=0;points[scn];scn++) {

		fprintf(stderr,"Computing characteristics for sets in scanner %d\n",scn);

	for(iArr[0]=0; iArr[0]<=beaconsmax[scn]; iArr[0]++)
	 for(iArr[1]=iArr[0]+1; iArr[1]<=beaconsmax[scn]; iArr[1]++)
	  for(iArr[2]=iArr[1]+1; iArr[2]<=beaconsmax[scn]; iArr[2]++)
	   for(iArr[3]=iArr[2]+1; iArr[3]<=beaconsmax[scn]; iArr[3]++)
	    for(iArr[4]=iArr[3]+1; iArr[4]<=beaconsmax[scn]; iArr[4]++)
	     for(iArr[5]=iArr[4]+1; iArr[5]<=beaconsmax[scn]; iArr[5]++)
	      for(iArr[6]=iArr[5]+1; iArr[6]<=beaconsmax[scn]; iArr[6]++)
	       for(iArr[7]=iArr[6]+1; iArr[7]<=beaconsmax[scn]; iArr[7]++)
		for(iArr[8]=iArr[7]+1; iArr[8]<=beaconsmax[scn]; iArr[8]++)
		 for(iArr[9]=iArr[8]+1; iArr[9]<=beaconsmax[scn]; iArr[9]++)
		  for(iArr[10]=iArr[9]+1; iArr[10]<=beaconsmax[scn]; iArr[10]++)
		   for(iArr[11]=iArr[10]+1; iArr[11]<=beaconsmax[scn]; iArr[11]++) {
			candidates[count].ch=stddev(scn,iArr,points);
			for(jj=0;jj<12;jj++) candidates[count].bec[jj]=iArr[jj];
//			memcpy(&(candidates[count].bec), iArr, 12*sizeof(char));
			candidates[count].scn=scn;
			fprintf(stderr,"Candidate %2d (scn %2d):\tstddev=%d\tthin=%5d mid=%5d thick=%d5 Set: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d \n", count, candidates[count].scn, candidates[count].ch.stddev, candidates[count].ch.thinness, candidates[count].ch.thickness,
candidates[count].bec[0],
candidates[count].bec[1],
candidates[count].bec[2],
candidates[count].bec[3],
candidates[count].bec[4],
candidates[count].bec[5],
candidates[count].bec[6],
candidates[count].bec[7],
candidates[count].bec[8],
candidates[count].bec[9],
candidates[count].bec[10],
candidates[count].bec[11]);/**/
			
			count++;
		   }
	}

	fprintf (stderr,"Sorting ... ");

	qsort (candidates, count, sizeof(TCandidate), compCandidate);

	fprintf (stderr,"done\n");



	int fitCount=0;
	int listed=0;
	for(i=1; i<count; i++) {
		if(equalsCh(candidates[i-1].ch,candidates[i].ch)) {
			fitCount++;
			if (!listed) {
				fitCount++;	
			}
			listed=1;
		} else listed=0;
	}

	fprintf(stderr,"Total good candidates: %d\n",fitCount);

	goodCandidates=malloc((fitCount+1)*sizeof(TCandidate));

	int combinations=0;

	fitCount=0;	
	listed=0;
	for(i=1; i<count; i++) {
		if(equalsCh(candidates[i-1].ch,candidates[i].ch)) {
			if (!listed) {
				
/*				printf("%7d. %d, %d, %d Scanner %d, Beacons ", fitCount,
					candidates[i-1].ch.stddev, candidates[i-1].ch.thinness, candidates[i-1].ch.thickness, candidates[i-1].scn);
				for(j=0; j<12; j++) printf("%d,",candidates[i-1].bec[j]);
				printf("\n");/**/
				goodCandidates[fitCount].ch=candidates[i-1].ch;
				goodCandidates[fitCount].scn=candidates[i-1].scn;
				for(j=0; j<12; j++) goodCandidates[fitCount].bec[j]=candidates[i-1].bec[j];
				combinations++;
				fitCount++;
			}
				goodCandidates[fitCount].ch=candidates[i].ch;
				goodCandidates[fitCount].scn=candidates[i].scn;
				for(j=0; j<12; j++) goodCandidates[fitCount].bec[j]=candidates[i].bec[j];
/*			printf("%7d. %d, %d, %d Scanner %d, Beacons ", fitCount,
				candidates[i].ch.stddev, candidates[i].ch.thinness, candidates[i].ch.thickness, candidates[i].scn);
			for(j=0; j<12; j++) printf("%d,",candidates[i].bec[j]);
			printf("\n");/**/
			fitCount++;
			listed=1;
		} else {
			listed=0;
		}
	}
	memset(&(goodCandidates[fitCount].ch),0,sizeof(TCharacteristic));

	fprintf(stderr,"Total %d unique combinations\n",combinations);

	free(candidates);

	return goodCandidates;
}

int compareSet (TPoint *a, TPoint *b) {
	int i;
	int retVal=1;

	comparisonsMade++;

	for(i=0; i<12; i++) {
/*		fprintf(stderr,"Comparing ");
		for(i=0; i<12; i++) fprintf(stderr,"[%d,%d,%d],", a[i].x, a[i].y, a[i].z);
		fprintf(stderr,"\n     with ");
		for(i=0; i<12; i++) fprintf(stderr,"[%d,%d,%d],", b[i].x, b[i].y, b[i].z);
		fprintf(stderr,"\n");/**/
		if((a[i].x!=b[i].x)||(a[i].y!=b[i].y)||(a[i].z!=b[i].z)) {
			retVal=0;
			break;
		}
	}

/*	if(retVal) {
		fprintf(stderr,"Found match ");
		for(i=0; i<12; i++) fprintf(stderr,"[%d,%d,%d],", a[i].x, a[i].y, a[i].z);
		fprintf(stderr,"\n     with ");
		for(i=0; i<12; i++) fprintf(stderr,"[%d,%d,%d],", b[i].x, b[i].y, b[i].z);
		fprintf(stderr,"\n");
	}	*/

	return retVal;

}


int compareAll(TCandidate first, TCandidate second, TPoint **points) {
	int i,j,type;
	TPoint shift;
	TPoint a[12];
	TPoint b[12];

	// Fill set a once
	for(i=0; i<12; i++) {
		a[i]=points[first.scn][first.bec[i]];
	}

	qsort (a, 12, sizeof(TPoint), compPoint);

/*	for(i=0; i<12; i++) {
		fprintf(stderr,"%s[%d, %d, %d]\n",i?"":"Set A after ordering\n", a[i].x, a[i].y, a[i].z);
	}/**/


	for(type=0; type<24; type ++) {
		// Fill set b every time
		for(i=0; i<12; i++) b[i]=transPoint(type, b[i]=points[second.scn][second.bec[i]]);
		qsort (b, 12, sizeof(TPoint), compPoint);

		shift.x=b[0].x-a[0].x;
		shift.y=b[0].y-a[0].y;
		shift.z=b[0].z-a[0].z;

/*		printf("Point A0 [%4d, %4d, %4d]\n",a[0].x,a[0].y,a[0].z);
		printf("Point B0 [%4d, %4d, %4d]\n",b[0].x,b[0].y,b[0].z);
		printf("Shift    [%4d, %4d, %4d]\n",shift.x,shift.y,shift.z);/**/

		for(i=0; i<12; i++) {
			b[i].x-=shift.x;
			b[i].y-=shift.y;
			b[i].z-=shift.z; }

		if(compareSet(a,b)) {
			fprintf(stderr,"Found match scanners %d and %d with rotation %d, shift [%d, %d, %d]\n", first.scn, second.scn, type, shift.x, shift.y, shift.z);
		}
	}

}

int testRotations() {
	TPoint x={404, -588, -901},y;
	int type;

	for(type=0;type <24; type ++) {
		y=transPoint(type,x);
		printf("[%d, %d, %d]\n",y.x, y.y, y.z);
	}
}




int Altmain () {
	TPoint ** points;
	int count,k,j,i,root;
	TCandidate *investigate;
	int lookalikes;
	int progress=0;

//	testRotations();
//	return 0;

	fprintf(stderr,"Reading input\n");
	points=readInput();

	fprintf(stderr,"Enumerating Candidates\n");
	investigate=candidates(points);

	for(count=0;investigate[count].ch.stddev; count++);

//	for(i=0; i<count; i++) fprintf(stderr,"%d ", investigate[i].scn);
	
	fprintf(stderr,"Starting main alalysis cycle\n");
	i=0;
	while(i<count) { 
		for(lookalikes=0; equalsCh(investigate[i].ch,investigate[i+lookalikes].ch);lookalikes++);
		if(!lookalikes) i++; // This shouldn't happen but at least don't freeze

//		fprintf(stderr,"%d lookalikes\n",lookalikes); 		
		for(j=i;j<i+lookalikes-1;j++) 
			for(k=j+1;k<i+lookalikes;k++) {
//				fprintf(stderr,"%d lookalikes: Comparing %d,%d\n",lookalikes,j,k);
/* Only neighbors of 0?	*///	if(investigate[j].scn==0||investigate[k].scn==0) {
					if(investigate[j].scn!=investigate[k].scn) {
//						fprintf(stderr,"Comparing set in scanner %d to %d\n",investigate[j].scn,investigate[k].scn);
						compareAll(investigate[j],investigate[k],points);
						if(++progress>10000) {
							progress=0;
							fprintf(stderr,".");
						}
					}
	//			}
			}
		i+=lookalikes;	
	}

	fprintf(stderr,"Comparisons made: %d\n",comparisonsMade);

	return 0;
}

int main (int argc, char *argv[]) {
	TPoint ** points;
	int count,k,j,i,rot;
	int Scn1, Scn2;
	TPoint *first, *second, *TRsecond;
	TPoint shift;
	TPoint test;
	int matches;

	int Count1, Count2;	

	if(argc>2) {
		Scn1=atoi(argv[1]);
		Scn2=atoi(argv[2]);
		fprintf(stderr,"Using scanners %d and %d", Scn1, Scn2);
	} else {
		fprintf(stderr,"Two scanners as arguments!\n");
		exit(1);
	}
		

	fprintf(stderr,"Reading input\n");
	points=readInput();

	fprintf(stderr,"Comparing rotations\n");


	first=points[Scn1];
	for(rot=0; rot<24; rot++) {
		second=calloc(MAXBEACONS+1, sizeof(TPoint));
		TRsecond=calloc(MAXBEACONS+1, sizeof(TPoint));
		for(i=0;(points[Scn2][i].x)&&(points[Scn2][i].y)&&(points[Scn2][i].z);i++) {
			second[i]=transPoint(rot,points[Scn2][i]);
		}

		// Try fitting each point in second set on the first point in the first set
		for(i=0;(second[i].x)&&(second[i].y)&&(second[i].z);i++) {
			shift.x=second[i].x-first[0].x;
			shift.y=second[i].y-first[0].y;
			shift.z=second[i].z-first[0].z;

			for(k=0;(second[k].x)&&(second[k].y)&&(second[k].z);k++) {
				TRsecond[k].x=second[k].x-shift.x;
				TRsecond[k].y=second[k].y-shift.y;
				TRsecond[k].z=second[k].z-shift.z;

			}

			count=0;
			matches=0;
			for(j=0;(first[j].x)&&(first[j].y)&&(first[j].z);j++) {
				for(k=0;(TRsecond[k].x)&&(TRsecond[k].y)&&(TRsecond[k].z);k++) {
					if(!compPoint(&(first[j]),&(TRsecond[k]))) matches++;
					count++;
				}
			}

			if(matches>11) 
				fprintf(stdout,"%d matches after %d tests at Rotation %d, shift [%d, %d, %d] (scanners %d and %d)\n", matches, count, rot, shift.x, shift.y, shift.z, Scn1, Scn2);
		}

		



		free(second);
	}
}
