#include <stdio.h>
#define MAXLINECHARS 1000000
#define MAXPRINTCHARS 80

int getlittleline(char line[]);

int main(int argc, char *argv[]){
	int len;
	char line[MAXLINECHARS];
	while((len = getlittleline(line)) > 0){
		if(len > MAXPRINTCHARS - 1){
			printf("%s",line);
		}
	}
	return 0;
}

int getlittleline(char line[]){
	int cinput,idx;
	for(idx=0;idx < MAXLINECHARS - 1 && (cinput = getchar()) != EOF && cinput != '\n'; idx++)
		line[idx]=cinput;
	if(cinput == '\n'){
		line[idx++]='\n';
	}
	line[idx] = '\0';
	return idx;
}
