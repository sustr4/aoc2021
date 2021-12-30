#!/bin/bash

rm -f day24-gen.c
rm -f day24-gen.h


cat <<EOF >> day24-gen.c
#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>

long int depthvisited[14]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char *current;

typedef struct {
	long int w;
	long int x;
	long int y;
	long int z; } TVars;

TVars (*step[14])(TVars v, int i);

TVars dummy() {
	long int w=0,x=0,y=0,z=0; TVars v;
`cat day24-input | sed -f day24.sed`
v.w=w; v.x=x; v.y=y; v.z=z;v.w=w; v.x=x; v.y=y; v.z=z;
return v;
}

int vEq(TVars f, TVars s) {

	if(f.w!=s.w) return 0;
	if(f.x!=s.x) return 0;
	if(f.y!=s.y) return 0;
	if(f.z!=s.z) return 0;

	return 1;
}

int check(int i[14]) {
	int j=0;
	long int w=0,x=0,y=0,z=0;

	return z;
}

void printVars(TVars v) {
	printf("%d, %d, %d, %d\\\\n", v.w, v.x, v.y, v.z);
}

long int powli(long int base, int power) {
	long int retVal=1;
	int i;

	if(power<0) return 0;
	
	for(i=0;i<power;i++) retVal*=base;
	return retVal;
}

int test(TVars expectedResult, int depth) {
	TVars candid,res;
	int retVal=0;
	int j,k;
	long int zmin,zmax;

	depthvisited[depth]++;

	if(depth<11)
		printVars(expectedResult);
//	printf("ExpectedResult at depth %d: ", depth);
//	printVars(expectedResult);
//	printf("\\\\n");
//	for(candid.w=9; candid.w>0; candid.w--)
//	  for(candid.x=60; candid.x>0; candid.x--)
//	    for(candid.y=60; candid.y>0; candid.y--)
	      for(k=0; k<=7; k++) {
		zmin=powli(26,k-1);
		zmax=powli(26,k)+26;
	      for(candid.z=zmin; candid.z<=zmax; candid.z++)
		for(j=9; j>0; j--) {
			res=step[depth](candid,j);
	//		printf("Got     Result at depth %d: ", res);
	//printf("\\\\n");
			if(vEq(res,expectedResult)) {
				current[depth]=(char)j+'0';
				if(!depth) {
					retVal=1;
					fprintf(stderr,"%s\\\\n",current);
				}
				else test(candid,depth-1);
			}
		}
		}/**/
	return retVal;
}

int main(int argc, char *argv[]) {
//	int i[14]={9,9,9,9,9,9,9,9,9,9,9,9,9,9};
//	int i[14]={5,5,5,5,5,5,5,5,5,5,5,5,5,5};
//	int i[14]={9,9,7,9,5,8,6,7,8,1,5,9,6,9};
	int i[14];
	TVars v,res;
	int j,inp,p;
	long int k, zmin, zmax;

	asprintf(&current,"              ");

	step[0]=&step0; step[1]=&step1; step[2]=&step2; step[3]=&step3;
	step[4]=&step4; step[5]=&step5; step[6]=&step6; step[7]=&step7;
	step[8]=&step8; step[9]=&step9; step[10]=&step10; step[11]=&step11;
	step[12]=&step12; step[13]=&step13;


	i[0]=9;	
	i[1]=9;	
	i[2]=9;	

//	for(i[0]=9; i[0]>0; i[0]--)
//	for(i[1]=9; i[1]>0; i[1]--) 
//	for(i[2]=9; i[2]>0; i[2]--)
	for(i[3]=9; i[3]>0; i[3]--)
	for(i[4]=9; i[4]>0; i[4]--)
	for(i[5]=9; i[5]>0; i[5]--)
	for(i[6]=9; i[6]>0; i[6]--)
	for(i[7]=9; i[7]>0; i[7]--)
	for(i[8]=9; i[8]>0; i[8]--)
	for(i[9]=9; i[9]>0; i[9]--)
	for(i[10]=9; i[10]>0; i[10]--)
	for(i[11]=9; i[11]>0; i[11]--)
	for(i[12]=9; i[12]>0; i[12]--)
	for(i[13]=9; i[13]>0; i[13]--)
	{

		v.w=0; v.x=0; v.y=0; v.z=0;
		for(j=0; j<14; j++) {
			v=step[j](v, i[j]);
		}
		if(!v.z) {
			printf("%d%d%d%d%d%d%d%d%d%d%d%d%d%d\\\\n",i[0],i[1],i[2],i[3],i[4],i[5],i[6],i[7],i[8],i[9],i[10],i[11],i[12],i[13]);
			exit(0);
		}
	}/**/

/*
// Skip: 3, 13,  ?


//	for(v.w=0; v.w<10; v.w++) {
//	  for(v.x=0; v.x<60; v.x++) {
//	    for(v.y=0; v.y<60; v.y++) {

	      for(k=0; k<=7; k++) {
		zmin=powli(26,k-1);
		zmax=powli(26,k)+26;

	      for(v.z=zmin; v.z<=zmax; v.z++)
		for(j=9; j>0; j--) {
			depthvisited[13]++;
			res=step[13](v, j);
			if(!res.z) {
				current[13]=(char)j+'0';
				test(res,12);
			}
		}
//	    printf(".");
	    }
//	  printf(",");
//	  }
//	}

	printf("\n");
	for(j=0;j<14;j++) printf("Depth %2d visited %ld times\\\\n", j, depthvisited[j]);/**/
}

EOF

ITER=0;
while read line; do
	echo $line | grep 'step(' >/dev/null
	if [ $? == 0 ]; then
		NEWLINE=`echo "$line" | sed "s/step/step${ITER}/"`
		echo "$NEWLINE" >> day24-gen.c.tmp
		ITER=$(($ITER+1))
	else echo "$line" >> day24-gen.c.tmp
	fi

done < day24-gen.c

mv day24-gen.c.tmp day24-gen.c



cc day24-gen.c -ggdb -o3 -o day24-gen

echo Running ...
time ./day24-gen

