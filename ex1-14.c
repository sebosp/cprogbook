#include <stdio.h>
// character frequency "bars"
int main(int argc, char *argv[]){
	int c;
	unsigned short int range = '~'-'!';
	int cfreq[range];
	int i,j;
	for(i = 0; i < range; ++i)
		cfreq[i]=0;
	while((c = getchar()) != EOF){
		c-='!';
		if(c <= range){
			++cfreq[c];
		}
	}
	printf("character frequency:");
	for(i = 0; i < range; ++i)
		if(cfreq[i]){
			putchar(i+'!');
			for(j=0;j<cfreq[i];j++)
				putchar('.');
			putchar('\n');
		}
	return 0;
}
