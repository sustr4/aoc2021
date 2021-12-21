#define _GNU_SOURCE
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
	long stddev;
	int thickness;
	int thinness; } TCharacteristic;

typedef struct {
	TCharacteristic ch;
	char scn;
	char bec[12];
} TCandidate;

int equalsCh(TCharacteristic f, TCharacteristic s) {
	if((f.stddev!=s.stddev)||
		(f.thickness!=s.thickness)||
		(f.thinness!=s.thinness)) return 0;
	return 1;
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
    if (f.stddev<s.stddev) return -1;
    if (f.stddev>s.stddev) return  1;
    if (f.thickness<s.thickness) return -1;
    if (f.thickness>s.thickness) return  1;
    if (f.thinness<s.thinness) return -1;
    if (f.thinness>s.thinness) return  1;
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

	point=malloc(SCANNERS*sizeof(*point));
	for(i=0;i<=SCANNERS;i++) point[i]=calloc(MAXBEACONS,sizeof(TPoint));

	input = fopen("day19-input.txt", "r");
	if (input == NULL) exit(EXIT_FAILURE);

	while ((read = getline(&line, &len, input)) != -1) {
		if(strlen(line)>6) {
			if(!strncmp("---",line,3)) { // next scanner
				sscanf(line,"--- scanner %d ---",&j);
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

	fclose(input);
	if (line)
        free(line);

	return point;
}


TCharacteristic stddev(int scn, char *iArr, TPoint **points) {
	int i;
	TPoint cog={0,0,0};
	TPoint max={0,0,0};
	TPoint min={0,0,0};
	TDblPoint dcog={0.0,0.0,0.0};
	double dev;
	TCharacteristic retVal;

	for(i=0;i<12;i++) {
		cog.x+=points[scn][iArr[i]].x;
		cog.y+=points[scn][iArr[i]].y;
		cog.z+=points[scn][iArr[i]].z; }

	dcog.x=cog.x/12.0;
	dcog.y=cog.y/12.0;
	dcog.z=cog.z/12.0;

	dev=0.0;
	for(i=0;i<12;i++) {
		dev+=(pow(points[scn][iArr[i]].x-dcog.x,2)+
		pow(points[scn][iArr[i]].y-dcog.y,2)+
		pow(points[scn][iArr[i]].z-dcog.z,2));

//fprintf(stderr,"post -> %f, %ld\n", dev, (long)trunc(dev));
		// Extreme dimensions
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

	retVal.thinness=max.x-min.x;
	if(max.y-min.y < retVal.thinness) retVal.thinness=max.y-min.y;
	if(max.z-min.z < retVal.thinness) retVal.thinness=max.z-min.z;

	retVal.thickness=max.x-min.x;
	if(max.y-min.y > retVal.thickness) retVal.thickness=max.y-min.y;
	if(max.z-min.z > retVal.thickness) retVal.thickness=max.z-min.z;

//	retVal.stddev=1;
	retVal.stddev=(long)dev;

	return (retVal);
} 

TCandidate * candidates(TPoint** points) {
	char iArr[12];
	long int i,j,jj;
	int *beaconsmax = calloc(SCANNERS,sizeof(int));
	int scn;
	long long count=0;
	TCandidate *candidates,*goodCandidates;

	for(j=0;j<SCANNERS;j++)
		for(i=0;i<MAXBEACONS;i++) {
//			printf("Scanner %d, beacon %d, [%d,%d,%d]\n",j,i,
//				points[j][i].x, points[j][i].y, points[j][i].z);
			if((points[j][i].x!=0) &&
			    (points[j][i].y!=0) &&
			    (points[j][i].z!=0)) beaconsmax[j]=i;
		}

//	for(j=0;j<SCANNERS;j++) printf("Scanner %d sees %d beacons\n",j,beaconsmax[j]);
	fprintf(stderr,"Counting candidates ... ");

	count=0;

	for(scn=0;scn<SCANNERS;scn++)
	 for(iArr[0]=0; iArr[0]<beaconsmax[scn]; iArr[0]++)
	  for(iArr[1]=iArr[0]+1; iArr[1]<beaconsmax[scn]; iArr[1]++)
	   for(iArr[2]=iArr[1]+1; iArr[2]<beaconsmax[scn]; iArr[2]++)
	    for(iArr[3]=iArr[2]+1; iArr[3]<beaconsmax[scn]; iArr[3]++)
	     for(iArr[4]=iArr[3]+1; iArr[4]<beaconsmax[scn]; iArr[4]++)
	      for(iArr[5]=iArr[4]+1; iArr[5]<beaconsmax[scn]; iArr[5]++)
	       for(iArr[6]=iArr[5]+1; iArr[6]<beaconsmax[scn]; iArr[6]++)
	        for(iArr[7]=iArr[6]+1; iArr[7]<beaconsmax[scn]; iArr[7]++)
	 	for(iArr[8]=iArr[7]+1; iArr[8]<beaconsmax[scn]; iArr[8]++)
	 	 for(iArr[9]=iArr[8]+1; iArr[9]<beaconsmax[scn]; iArr[9]++)
	 	  for(iArr[10]=iArr[9]+1; iArr[10]<beaconsmax[scn]; iArr[10]++)
	 	   for(iArr[11]=iArr[10]+1; iArr[11]<beaconsmax[scn]; iArr[11]++) count ++;

	fprintf(stderr,"%d\n", count);

	candidates=malloc(count*sizeof(TCandidate));

	count=0;
	for(scn=0;scn<SCANNERS;scn++) {

	fprintf(stderr, "Candidates at scanner %d (startting at No. %d)\n", scn, count);

	for(iArr[0]=0; iArr[0]<beaconsmax[scn]; iArr[0]++)
	 for(iArr[1]=iArr[0]+1; iArr[1]<beaconsmax[scn]; iArr[1]++)
	  for(iArr[2]=iArr[1]+1; iArr[2]<beaconsmax[scn]; iArr[2]++)
	   for(iArr[3]=iArr[2]+1; iArr[3]<beaconsmax[scn]; iArr[3]++)
	    for(iArr[4]=iArr[3]+1; iArr[4]<beaconsmax[scn]; iArr[4]++)
	     for(iArr[5]=iArr[4]+1; iArr[5]<beaconsmax[scn]; iArr[5]++)
	      for(iArr[6]=iArr[5]+1; iArr[6]<beaconsmax[scn]; iArr[6]++)
	       for(iArr[7]=iArr[6]+1; iArr[7]<beaconsmax[scn]; iArr[7]++)
		for(iArr[8]=iArr[7]+1; iArr[8]<beaconsmax[scn]; iArr[8]++)
		 for(iArr[9]=iArr[8]+1; iArr[9]<beaconsmax[scn]; iArr[9]++)
		  for(iArr[10]=iArr[9]+1; iArr[10]<beaconsmax[scn]; iArr[10]++)
		   for(iArr[11]=iArr[10]+1; iArr[11]<beaconsmax[scn]; iArr[11]++) {
			candidates[count].ch=stddev(scn,iArr,points);
			memcpy(&(candidates[count].bec), iArr, 12*sizeof(char));
			candidates[count].scn=scn; //printf("%d, %d\n", count, candidates[count].scn);
			
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
		} else listed=0;
	}
	memset(&(goodCandidates[fitCount].ch),0,sizeof(TCharacteristic));

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

	if(retVal) {
		fprintf(stderr,"Found match ");
		for(i=0; i<12; i++) fprintf(stderr,"[%d,%d,%d],", a[i].x, a[i].y, a[i].z);
		fprintf(stderr,"\n     with ");
		for(i=0; i<12; i++) fprintf(stderr,"[%d,%d,%d],", b[i].x, b[i].y, b[i].z);
		fprintf(stderr,"\n");
	}	

	return retVal;

}


int compareAll(TCandidate first, TCandidate second, TPoint **points) {
	int i,j,type;
	TPoint shift;
	TPoint a[12], b[12];
	

	// Fill set a once
	for(i=0; i<12; i++) a[i]=points[first.scn][first.bec[i]];
	qsort (a, 12, sizeof(TPoint), compPoint);

	for(type=0; type<24; type ++) {
		// Fill set b every time
		for(i=0; i<12; i++) transPoint(type, b[i]=points[second.scn][second.bec[i]]);
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
			fprintf(stderr,"Found match scanners %d and %d with rotation %d\n", first.scn, second.scn, type);
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

/*
[404, -588, -901],
[901, -588, 404],
[-404, -588, 901],
[-901, -588, -404],
[-901, 588, 404],
[-404, 588, -901],
[901, 588, -404],
[404, 588, 901],
[404, -901, 588],
[901, 404, 588],
[-404, 901, 588],
[-901, -404, 588],
[-901, 404, -588],
[-404, -901, -588],
[901, -404, -588],
[404, 901, -588],
[588, 901, 404],
[588, -404, 901],
[588, -901, -404],
[588, 404, -901],
[-588, 404, 901],
[-588, -901, 404],
[-588, -404, -901],
[-588, 901, -404]*/


}


int main () {
	TPoint ** points;
	int count,k,j,i,root;
	TCandidate *investigate;
	int lookalikes;

//	testRotations();
//	return 0;

	fprintf(stderr,"Reading input\n");
	points=readInput();

	fprintf(stderr,"Enumerating Candidates\n");
	investigate=candidates(points);

	for(count=0;investigate[count].ch.stddev; count++);

//	for(i=0; i<count; i++) fprintf(stderr,"%d ", investigate[i].scn);
	
	i=0;
	while(i<count) { 
		for(lookalikes=0; equalsCh(investigate[i].ch,investigate[i+lookalikes].ch);lookalikes++);

//		fprintf(stderr,"%d lookalikes\n",lookalikes); 		
		for(j=i;j<i+lookalikes-1;j++) 
			for(k=j+1;k<i+lookalikes;k++) {
//				fprintf(stderr,"%d lookalikes: Comparing %d,%d\n",lookalikes,j,k);
/* Only neighbors of 0?	*///	if(investigate[j].scn==0||investigate[k].scn==0) {
					if(investigate[j].scn!=investigate[k].scn) {
//						fprintf(stderr,"Comparing set in scanner %d to %d\n",investigate[j].scn,investigate[k].scn);
						compareAll(investigate[j],investigate[k],points);
					}
	//			}
			}
		i+=lookalikes;	
	}

	fprintf(stderr,"Comparisons made: %d\n",comparisonsMade);

	return 0;
}
