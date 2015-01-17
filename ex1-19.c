#include <stdio.h>
#define MAXLINECHARS 1000000

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
	char readline[MAXLINECHARS];
	for(idx=0;idx < MAXLINECHARS - 1 && (lastchar = getchar()) != EOF && lastchar != '\n'; idx++)
		readline[idx]=lastchar;
	if(idx == 0){ // EOF found.
		return 0;
	}
	if(lastchar == '\n'){
		idx--; // Remove the \n...
	}
	int len = idx;
	for(;idx >= 0;idx--)
		line[len-idx]=readline[idx];
	if(len > 0){
		if(lastchar == '\n'){
			line[++len]='\n';
		}
		line[++len]='\0';
	}
	return len;
}
