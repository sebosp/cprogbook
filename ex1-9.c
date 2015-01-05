#include <stdio.h>
// Is getchar() != EOF is 0 or 1.
int main(int argc, char *argv[]){
	int c,spaceseen;
	while((c = getchar()) != EOF){
		if(c == ' '){
			if(spaceseen == 1){
				continue;
			}else{
				spaceseen = 1;
			}
		}else{
			spaceseen = 0;
		}
		putchar(c);
	}
	return 0;
}
