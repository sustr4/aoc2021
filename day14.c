#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

int maxdepth=10;

int mode=0; // 0 .. count, 1 .. output

int skip_last=0;

long *counter;

char **lookup;

const char * pattern[] = {
"HB", "KO", "KK", "PF", "VB", "KC", "BP", "SS", "BS", "PB", "VH", "BK", "BO", "HN", "NN", "PV", "NH", "KP", "NB", "NF", "PP", "PN", "VN", "SC", "NS", "SV", "BV", "FV", "OK", "HF", "CV", "KB", "OB", "NO", "OF", "HP", "BB", "FB", "OC", "NV", "OV", "OP", "SP", "FK", "VV", "VK", "OS", "CO", "CH", "HV", "FN", "CS", "PS", "HS", "VO", "NP", "FP", "KF", "CC", "BF", "VP", "HO", "FC", "BH", "NK", "BN", "SH", "CP", "VS", "ON", "FS", "HK", "PC", "KN", "CK", "HH", "CN", "BC", "PH", "OO", "FO", "SK", "FF", "VC", "SF", "KH", "SO", "KS", "SB", "VF", "PK", "OH", "HC", "PO", "NC", "FH", "KV", "CB", "CF", "SN", NULL };


char insert[] = {
'C', 'S', 'N', 'F', 'F', 'S', 'H', 'H', 'B', 'O', 'C', 'S', 'F', 'V', 'K', 'C', 'P', 'N', 'V', 'V', 'O', 'B', 'K', 'O', 'N', 'B', 'P', 'F', 'H', 'F', 'K', 'C', 'B', 'V', 'B', 'C', 'F', 'H', 'K', 'H', 'S', 'N', 'N', 'F', 'B', 'H', 'F', 'F', 'V', 'V', 'B', 'F', 'F', 'F', 'K', 'F', 'B', 'P', 'N', 'S', 'F', 'H', 'F', 'K', 'S', 'V', 'K', 'B', 'K', 'S', 'P', 'F', 'O', 'H', 'N', 'N', 'S', 'K', 'N', 'B', 'O', 'B', 'V', 'N', 'N', 'V', 'F', 'H', 'K', 'V', 'O', 'N', 'F', 'O', 'F', 'V', 'V', 'C', 'O', 'H' };

int expandPair(char *pair) {
	static int depth=0;
	char newpair[]="  ";
	int i;
	static int progress;


	depth++;


//	for(i=0; pattern[i]; i++) {
//		if(!strncmp(pattern[i],pair,2)) break;
//	}
	if (lookup[pair[0]][pair[1]]) {

//		printf("Pair %s, going with pattern %s, insert is %c\n", pair, pattern[i], insert[i]);
		if(depth>maxdepth-1) {
			if(mode) printf("%c%c",pair[0],lookup[pair[0]][pair[1]]);
			counter[(char)pair[0]]++;
			counter[(char)lookup[pair[0]][pair[1]]]++;
		} else {
			newpair[0]=pair[0];
			newpair[1]=lookup[pair[0]][pair[1]];
			expandPair(newpair);
			newpair[0]=lookup[pair[0]][pair[1]];
			newpair[1]=pair[1];
			expandPair(newpair);
		}	
	}

	depth--;
}

int main(int argc, char *argv[]) {
	char *input;
	int i;
	char testch[]="  ";
	char j;


	asprintf(&input,"SFBBNKKOHHHPFOFFSPFV");

	fprintf(stderr,"No. of args: %d\nusage:\n%s <input-string> <depth> <mode> <skip_last>\n", argc, argv[0]);

	if(argc>1) {
		free(input);
		input=argv[1];
	}

	if(argc>2) {
		maxdepth=atoi(argv[2]); 
		fprintf(stderr,"Max depth will be %d\n", maxdepth);
	}

	if(argc>3) {
		mode=atoi(argv[3]); 
		fprintf(stderr,"Mode will be %s\n", mode?"'output'":"'count'");
	}

	if(argc>4) {
		skip_last=atoi(argv[4]); 
		fprintf(stderr,"Last character %s count\n", skip_last?"won't":"will");
	}

	counter=calloc((char)'Z',sizeof(long));
	
	lookup=calloc((char)'Z',sizeof(long));
	for(j=(char)'A'; j<=(char)'Z'; j++) lookup[j]=calloc((char)'Z',sizeof(long));

	for(i=0; pattern[i]; i++)
		lookup[pattern[i][0]][pattern[i][1]]=insert[i];

	for(i=0; i<strlen(input)-1; i++) {
		testch[0]=input[i];
		testch[1]=input[i+1];
		fprintf(stderr,"expanding pair %d: %s\n", i, testch);
		expandPair(testch);
	}

	if(!skip_last) {
		counter[(char)input[strlen(input)-1]]++;
		if (mode) printf("%c",input[strlen(input)-1]);
	}

	long min,max;

	max=counter[(char)'A'];
	for(j=(char)'B'; j<=(char)'Z'; j++) if(counter[j]>max) max=counter[j];
	min=max;
	for(j=(char)'A'; j<=(char)'Z'; j++) {
		if( counter[j] && counter[j]<min) min=counter[j];
		if(!mode) printf("%c\t%d\n",j,counter[j]);
	}

	fprintf(stderr,"Difference: %d\n", max-min);

	return 0;
}
