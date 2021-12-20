#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include"day20.h"

int main() {
	int i,j,repetition;
	int extend=0, tgtsize=104,srcsize;
	char *src=NULL,*tgt=input;
	int index,lit;

	for(repetition=0; repetition<50; repetition++) {
		extend=+1;
		srcsize=tgtsize;
		tgtsize+=+2*extend;
		src=tgt;
		tgt=calloc(tgtsize*tgtsize,sizeof(char));
		if(filter[0]&&(!(repetition%2))) memset(tgt,1,tgtsize*tgtsize*sizeof(char));
		lit=0;

		for (j=1;j<srcsize-1;j++) {
			for (i=1;i<srcsize-1;i++) {

				index=0;
				index+=src[i-1+(j-1)*srcsize]*256;
				index+=src[i+(j-1)*srcsize]*128;
				index+=src[i+1+(j-1)*srcsize]*64;
				index+=src[i-1+j*srcsize]*32;
				index+=src[i+j*srcsize]*16;
				index+=src[i+1+j*srcsize]*8;
				index+=src[i-1+(j+1)*srcsize]*4;
				index+=src[i+(j+1)*srcsize]*2;
				index+=src[i+1+(j+1)*srcsize]*1;

				tgt[i+extend+(j+extend)*tgtsize]=filter[index];
				printf("%s",tgt[i+extend+(j+extend)*tgtsize]?"X":index?",":".");
				lit+=tgt[i+extend+(j+extend)*tgtsize];
			}
			printf(" %d\t%d\t%d\n",j,srcsize,tgtsize);
		}
		printf("Lit: %d\n",lit);

	}
}
