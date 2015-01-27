#include <stdio.h>
#include <stdbool.h>
// This is a single-line comment;
void uncommentLine();
/* This is a multi-line
 * Comment */
int main(int argc, char *argv[]){
	uncommentLine();
	return 0;
}

void uncommentLine(){
	int curchar,prevchar;
	prevchar = ' ';
	bool oneLineComment,multiLineComment;
	oneLineComment = multiLineComment = false;
	while((curchar = getchar()) != EOF){
//		printf("Reading: %c\n",curchar);
		if(prevchar == '/'){// Comments start with /...
			if(curchar == '/'){
				oneLineComment = true;
			}else{
				if(curchar == '*')
					multiLineComment = true;
			}
		}
		if(prevchar == '*' && curchar == '/' && multiLineComment)
			multiLineComment = false;
		if(curchar == '\n'){
			oneLineComment = false;
			prevchar = curchar;
		}
		if(!oneLineComment && !multiLineComment && prevchar == '/')
			putchar(prevchar);
		if(!oneLineComment && !multiLineComment && curchar != '/')
			putchar(curchar);
		prevchar = curchar;
	}
}
