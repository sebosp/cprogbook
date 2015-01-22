#include <stdio.h>
#define MAXLINECHARS 1000000
#define TABSTOP 8

int getentabline(char line[]);

int main(int argc, char *argv[]){
	char line[MAXLINECHARS];
	while(getentabline(line) > 0){
		printf("%s",line);
	}
	return 0;
}

int getentabline(char line[]){
	int lastchar,idx;
	for(idx=0;idx < (MAXLINECHARS - 1) && (lastchar = getchar()) != EOF && lastchar != '\n'; idx++){
		line[idx]=lastchar;
		if(lastchar == ' '){
			if(idx % TABSTOP == 0){
				for(;line[idx] == ' ';idx--);
				line[++idx]='\t';
			}
		}
	}
	if(lastchar == EOF)
		return 0;
	if(lastchar == '\n') // We might have reached our max...
		line[idx++]='\n';
	line[idx++]='\0';
	return idx;
}
