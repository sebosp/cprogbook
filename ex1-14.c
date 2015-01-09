#include <stdio.h>
// character frequency "bars"
int main(int argc, char *argv[]){
	int c;
	int cfreq['~'-'!'];
	int i,j;
    unsigned short int range;
    range = '~'-'!';
	for(i = 0; i < range; ++i)
		cfreq[i]=0;
	while((c = getchar()) != EOF){
		if(c >= '!' && c <= '~'){
			++cfreq[c-'!'];
		}
	}
	printf("character frequency:");
	for(i = 0; i < range; ++i)
		if(cfreq[i]){
			printf("%c:",i+'!');
			for(j=0;j<cfreq[i];j++)
				printf(".");
			printf("\n");
		}
	return 0;
}
