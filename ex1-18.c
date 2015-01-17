#include <stdio.h>
#define MAXLINECHARS 1000000
#define MAXPRINTCHARS 80

int getlittleline(char line[]);

int main(int argc, char *argv[]){
	int len;
	char line[MAXLINECHARS];
	while((len = getlittleline(line)) > 0){
		printf("%s",line);
	}
	return 0;
}

int getlittleline(char line[]){
	int lastchar,idx;
	for(idx=0;idx < MAXLINECHARS - 1 && (lastchar = getchar()) != EOF && lastchar != '\n'; idx++)
		line[idx]=lastchar;
	if(idx == 0){ // EOF found.
		return 0;
	}
	if(lastchar == '\n'){
		idx--; // Remove the \n...
	}
	for(;idx > 0 && (line[idx] == ' ' || line[idx] == '\t');--idx);
	if(idx > 0){
		line[++idx]='\n';
	}
	line[++idx] = '\0';
	return idx;
}
