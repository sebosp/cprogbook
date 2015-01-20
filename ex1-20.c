#include <stdio.h>
#define MAXLINECHARS 1000000
#define TABSTOP 8

int getdetabline(char line[]);

int main(int argc, char *argv[]){
	char line[MAXLINECHARS];
	while(getdetabline(line) > 0){
		printf("%s",line);
	}
	return 0;
}

int getdetabline(char line[]){
	int lastchar,idx;
	for(idx=0;idx < (MAXLINECHARS - 1) && (lastchar = getchar()) != EOF && lastchar != '\n'; idx++){
		if(lastchar == '\t'){
			for(;idx % TABSTOP != 0; idx++)
				line[idx] = ' ';
			idx--;
		}else
			line[idx]=lastchar;
	}
	if(lastchar == EOF)
		return 0;
	if(lastchar == '\n') // We might have reached our max...
		line[idx++]='\n';
	line[idx++]='\0';
	return idx;
}
