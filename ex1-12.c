#include <stdio.h>
// Separate by printable chars, sort of "words"
int main(int argc, char *argv[]){
	int c;
	while((c = getchar()) != EOF){
		if(c < '!' || c > '~'){
			putchar('\n');
		}else{
			if(c != '\n'){
				putchar(c);
			}
		}
	}
	return 0;
}
