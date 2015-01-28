#include <stdio.h>
#include <stdbool.h>
#define MAXMATCHES 1000
// This is a single-line comment;
void uncommentLine();
/* This is a multi-line
 * Comment */
int main(int argc, char *argv[]){
	uncommentLine();
	return 0;
}

void uncommentLine(){
	int curchar,idx,matchChar;
	idx = -1;
	char missingCloses[MAXMATCHES];
	bool insideSingleQuote, insideDoubleQuote;
	insideSingleQuote = insideDoubleQuote = false;
	while((curchar = getchar()) != EOF){
		if(insideSingleQuote){
			if(curchar == '\x27')
				insideSingleQuote = false;
			continue;
		}
		if(insideDoubleQuote){
			if(curchar == '"')
				insideDoubleQuote = false;
			continue;
		}
		switch(curchar){
			case '"':
				insideDoubleQuote = true;
				continue;
			case '\x27':
				insideSingleQuote = true;
				continue;
			case '[':
			case '(':
			case '{':
				missingCloses[++idx]=curchar;
				continue;
			default:
				break;
		}
		matchChar = ' ';
		switch(curchar){
			case ']': matchChar = '['; break;
			case ')': matchChar = '('; break;
			case '}': matchChar = '{'; break;
			default:
				continue;
		}
		if(missingCloses[idx--] != matchChar){
			printf("Unmatched '%c' at character %i\n",matchChar,idx);
			return;
		}
	}
	if(idx > -1){
		missingCloses[++idx] = '\0';
		printf("List of unmatched entries: %s\n",missingCloses);
	}
	if(insideSingleQuote)
		printf("Could not locate end of single quote\n");
	if(insideDoubleQuote)
		printf("Could not locate end of double quote\n");
}
