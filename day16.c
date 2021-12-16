
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>

int inputlength;
int versiontotal=0;

long powi(int base, int power) {
	long i,retVal=1;

	for(i=0; i<power; i++) {
		retVal*=base;
	}
	return retVal;
}


char* getBinInput() {
	char *x0input, *input;
	int i;
// OK	asprintf(&x0input,"D2FE28");
// OK	asprintf(&x0input,"38006F45291200");

// OK	asprintf(&x0input,"8A004A801A8002F478");
// OK	asprintf(&x0input,"620080001611562C8802118E34");
// OK	asprintf(&x0input,"C0015000016115A2E0802F182340");
// OK	asprintf(&x0input,"EE00D40C823060");
// OK	asprintf(&x0input,"A0016C880162017C3686B18A3D4780");


	asprintf(&x0input,"0052E4A00905271049796FB8872A0D25B9FB746893847236200B4F0BCE5194401C9B9E3F9C63992C8931A65A1CCC0D222100511A00BCBA647D98BE29A397005E55064A9DFEEC86600BD002AF2343A91A1CCE773C26600D126B69D15A6793BFCE2775D9E4A9002AB86339B5F9AB411A15CCAF10055B3EFFC00BCCE730112FA6620076268CE5CDA1FCEB69005A3800D24F4DB66E53F074F811802729733E0040E5C5E5C5C8015F9613937B83F23B278724068018014A00588014005519801EC04B220116CC0402000EAEC03519801A402B30801A802138801400170A0046A800C10001AB37FD8EB805D1C266963E95A4D1A5FF9719FEF7FDB4FB2DB29008CD2BAFA3D005CD31EB4EF2EBE4F4235DF78C66009E80293AE9310D3FCBFBCA440144580273BAEE17E55B66508803C2E0087E630F72BCD5E71B32CCFBBE2800017A2C2803D272BCBCD12BD599BC874B939004B5400964AE84A6C1E7538004CD300623AC6C882600E4328F710CC01C82D1B228980292ECD600B48E0526E506F700760CCC468012E68402324F9668028200C41E8A30E00010D8B11E62F98029801AB88039116344340004323EC48873233E72A36402504CB75006EA00084C7B895198001098D91AE2190065933AA6EB41AD0042626A93135681A400804CB54C0318032200E47B8F71C0001098810D61D8002111B228468000E5269324AD1ECF7C519B86309F35A46200A1660A280150968A4CB45365A03F3DDBAE980233407E00A80021719A1B4181006E1547D87C6008E0043337EC434C32BDE487A4AE08800D34BC3DEA974F35C20100BE723F1197F59E662FDB45824AA1D2DDCDFA2D29EBB69005072E5F2EDF3C0B244F30E0600AE00203229D229B342CC007EC95F5D6E200202615D000FB92CE7A7A402354EE0DAC0141007E20C5E87A200F4318EB0C");/**/
	input=calloc(4*strlen(x0input),sizeof(char));


	fprintf(stderr,"%s\n",x0input);
	inputlength=4*strlen(x0input);

	for(i=0;i<strlen(x0input);i++) {
		if(x0input[i]>=(char)'8') input[i*4]=1;
		if(x0input[i]=='4' || x0input[i]=='5'  || x0input[i]=='6' || x0input[i]=='7' || x0input[i]=='C' || x0input[i]=='D' || x0input[i]=='E' || x0input[i]=='F') input[i*4+1]=1;
		if(x0input[i]=='2' || x0input[i]=='3'  || x0input[i]=='6' || x0input[i]=='7' || x0input[i]=='A' || x0input[i]=='B' || x0input[i]=='E' || x0input[i]=='F') input[i*4+2]=1;
		if(x0input[i]=='1' || x0input[i]=='3'  || x0input[i]=='5' || x0input[i]=='7' || x0input[i]=='9' || x0input[i]=='B' || x0input[i]=='D' || x0input[i]=='F') input[i*4+3]=1; }
	return input;
}

void printBin(char* string, int length) {
	int i;
	for(i=0;i<length;i++) fprintf(stderr,"%c",string[i]+'0');
	fprintf(stderr,"\n");
}


long bin2int(char *input, int length) {
	int i;
	long retVal = 0;

	for(i=0; i<length; i++) retVal=retVal+(input[length-1-i])*powi(2,i);

	return retVal;
}


int parsePacket(char* input,int length){
	static int depth=0;
	int i,j;
	char type[4]={0,0,0,0};	
	char ver[4]={0,0,0,0};
	char lentype;
	int dtype,dver,insidelen,donelen;
	int processedlen=0;
	char *literal=NULL;
	
	memcpy(ver,input,3);
	memcpy(type,input+3,3);

	fprintf(stderr,"\nPacket depth %d, the head is something like ", depth++); printBin(input,20);

	fprintf(stderr,"Version: %d ",dver=bin2int(ver,3)); printBin(ver,3);
	fprintf(stderr,"Type ID: %d ",dtype=bin2int(type,3)); printBin(type,3);
	processedlen+=6;
	versiontotal+=dver;

	if(dtype!=4) {
		lentype=input[6]; processedlen++;
		fprintf(stderr,"LenType: %d\n",lentype);

		if(lentype) {
			insidelen=bin2int(input+7,11);
			fprintf(stderr,"Content length: %d PACKETS ",insidelen); printBin(input+7,11);
			processedlen+=11;

			for(i=0; i<insidelen; i++) {
				fprintf(stderr,"Stepping into No. %d at position %d, length %d\n", i, processedlen, insidelen);
				donelen=parsePacket(input+processedlen,length);
				processedlen+=donelen;
				fprintf(stderr,"\nStepping out at position %d\n", processedlen, insidelen);
			}
			
	
	
		} else {
			insidelen=bin2int(input+7,15);
			donelen=0;
			fprintf(stderr,"Content length: %d BYTES ",insidelen); printBin(input+7,15);
			processedlen+=15;

			while(1) {
				fprintf(stderr,"Stepping in at position %d, length %d\n", processedlen, insidelen);
				donelen=parsePacket(input+processedlen,insidelen);
				processedlen+=donelen; insidelen-=donelen;
				fprintf(stderr,"\nStepping out at position %d\n", processedlen, insidelen);
				if(insidelen<=6) break;
			}
			
		}
	} else {
		i=0;
		j=0;
		literal=calloc(length+1,sizeof(char));
		while(1) {
			fprintf(stderr,"(%d) %d -- ", processedlen+i, input[processedlen+i]); printBin(input+processedlen+i,5);
			memcpy(literal+j, input+processedlen+i+1,4);
			j=j+4;
			if(!input[processedlen+i]) break;
			i=i+5;
		}
		processedlen+=i+5;
		fprintf(stderr,"Literal: %d \n", bin2int(literal,j)); printBin(literal,j);
		printf("%d ", bin2int(literal,j));
		free(literal);
	}

	depth--;
	fprintf(stderr,"Total %d bytes processed in this step\n",processedlen);
//	processedlen=processedlen+(8-processedlen%8);
//	fprintf(stderr,", %d after rounding",processedlen);
	return processedlen;
}

int main () {
	char* input;
	long i;

	input=getBinInput();

	for(i=0;i<inputlength;i++) fprintf(stderr,"%d",input[i]);
	fprintf(stderr,"\n");/**/

	parsePacket(input,inputlength);

	fprintf(stderr,"Version total: %d\n",versiontotal);

	return 0;
}
