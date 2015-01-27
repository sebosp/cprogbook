#include <stdio.h>
#include <stdbool.h>
#define MAXLINESIZE 1000000
#define MAXLINECHARS 8

int getsplitline(char line[]);

int main(int argc, char *argv[]){
	char line[MAXLINESIZE];
	while(getsplitline(line) > 0){
		printf("%s",line);
	}
	return 0;
}

int getsplitline(char line[]){
	int lastchar,idx,idxFold;
	bool hasNonPrintable = false;
	for(idx=0;idx < (MAXLINESIZE - 1) && (lastchar = getchar()) != EOF; idx++){
		line[idx] = lastchar;
		if(lastchar >= '!' && lastchar <= '~')
			hasNonPrintable = true;
		if(idx > MAXLINECHARS && !(lastchar >= '!' && lastchar <= '~') && hasNonPrintable){
			for(idxFold = idx-1;line[idxFold] >= '!' && line[idxFold] <= '~';idxFold--); // Locate a nonprintable slot to split.
			line[idxFold]='\n'; // Split
			line[++idx]='\n';
			line[++idx]='\0';
			return idx;
		}
		if(lastchar == '\n'){
			break;
		}
	}
	if(lastchar == EOF)
		return 0;
	line[++idx]='\0';
	return idx;
}
