#include <stdio.h>
#define MAXLINECHARS 500

int getlittleline(char line[]);

int main(int argc, char *argv[]){
	int len;
	char line[MAXLINECHARS];
	int linenum=0;
	while((len = getlittleline(line)) > 0){
		++linenum;
		if(len == MAXLINECHARS - 1){
			printf("Line %d cut at %d max characters. First 100 chars: %.100s\n",linenum,MAXLINECHARS,line);
		}
	}
	return 0;
}

int getlittleline(char line[]){
	int cinput,idx;
	for(idx=0;idx < MAXLINECHARS - 1 && (cinput = getchar()) != EOF && cinput != '\n'; idx++)
		line[idx]=cinput;
	if(cinput == '\n'){
		line[idx++] = cinput;
	}
	line[idx] = '\0';
	return idx;
}
