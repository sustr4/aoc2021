#include<stdio.h>
#include<stdlib.h>
#include"day24-gen.h"

int main(int argc, char *argv[]) {
	int i[14];

	for(i[0]=9; i[0]>=0; i[0]--)
	for(i[1]=i[0]; i[1]>=0; i[1]--)
	for(i[2]=i[1]; i[2]>=0; i[2]--)
	for(i[3]=i[2]; i[3]>=0; i[3]--)
	for(i[4]=i[3]; i[4]>=0; i[4]--)
	for(i[5]=i[4]; i[5]>=0; i[5]--)
	for(i[6]=i[5]; i[6]>=0; i[6]--)
	for(i[7]=i[6]; i[7]>=0; i[7]--)
	for(i[8]=i[7]; i[8]>=0; i[8]--)
	for(i[9]=i[8]; i[9]>=0; i[9]--)
	for(i[10]=i[9]; i[10]>=0; i[10]--)
	for(i[11]=i[10]; i[11]>=0; i[11]--)
	for(i[12]=i[11]; i[12]>=0; i[12]--)
	for(i[13]=i[12]; i[13]>=0; i[13]--)
	for(i[14]=i[13]; i[14]>=0; i[14]--)
		if(!check(i)) {
			printf("%d%d%d%d%d%d%d%d%d%d%d%d%d%d\n",i[0],i[1],i[2],i[3],i[4],i[5],i[6],i[7],i[8],i[9],i[10],i[11],i[12],i[13],i[14]);
		}
}
