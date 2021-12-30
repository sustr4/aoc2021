#!/bin/bash

rm -f day24-gen.c
rm -f day24-gen.h


cat <<EOF >> day24-gen.c
#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>

int test(int *i) {
	int j=0,w=0,x=0,y=0,z=0;

`cat day24-input | sed -f day24.sed`

	return z;
}



int main(int argc, char *argv[]) {
//	int i[14]={9,9,9,9,9,9,9,9,9,9,9,9,9,9};
//	int i[14]={5,5,5,5,5,5,5,5,5,5,5,5,5,5};
//	int i[14]={9,9,7,9,5,8,6,7,8,1,5,9,6,9};
	int i[14];
	int j,inp,p;
	long int k, zmin, zmax;

	for(i[0]=1; i[0]<=9; i[0]++)
	for(i[1]=1; i[1]<=9; i[1]++) 
	for(i[2]=1; i[2]<=9; i[2]++)
	for(i[3]=1; i[3]<=9; i[3]++)
	for(i[4]=1; i[4]<=9; i[4]++)
	for(i[5]=1; i[5]<=9; i[5]++)
	for(i[6]=1; i[6]<=9; i[6]++)
	for(i[7]=1; i[7]<=9; i[7]++)
	for(i[8]=1; i[8]<=9; i[8]++)
	for(i[9]=1; i[9]<=9; i[9]++)
	for(i[10]=1; i[10]<=9; i[10]++)
	for(i[11]=1; i[11]<=9; i[11]++)
	for(i[12]=1; i[12]<=9; i[12]++)
	for(i[13]=1; i[13]<=9; i[13]++)
	{
		if(!test(i)) {
			printf("%d%d%d%d%d%d%d%d%d%d%d%d%d%d\\\\n",i[0],i[1],i[2],i[3],i[4],i[5],i[6],i[7],i[8],i[9],i[10],i[11],i[12],i[13]);
			exit(0);
		}
	}/**/
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



cc day24-gen.c -ggdb -O3 -o day24-gen

echo Running ...
time ./day24-gen

