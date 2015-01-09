#include <stdio.h>
#define MAXLENGTH 100
// word length frequency "bars"
int main(int argc, char *argv[]){
	int c;
	int wlengths[MAXLENGTH];
	int i,j;
	int curlength = 0;
	for(i = 0; i < MAXLENGTH; ++i)
		wlengths[i]=0;
	while((c = getchar()) != EOF){
		if(c < '!' || c > '~'){
			if(curlength && curlength < MAXLENGTH)
				++wlengths[curlength];
			curlength=0;
		}else{
			++curlength;
		}
	}
	printf("Word length frequency:");
	for(i = 0; i < MAXLENGTH; ++i)
		if(wlengths[i]){
			printf("\n%03d:",i);
			for(j=0;j<wlengths[i];j++)
				printf(".");
		}
	printf("\n");
	return 0;
}
