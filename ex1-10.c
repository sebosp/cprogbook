#include <stdio.h>
// Is getchar() != EOF is 0 or 1.
int main(int argc, char *argv[]){
	int c;
	while((c = getchar()) != EOF){
		switch(c){
			case '\t': putchar('\\');putchar('t');
				   break;
			case  ' ': putchar('\\');putchar('b');
				   break;
			case '\\': putchar('\\');putchar('\\');
				   break;
			default: putchar(c);
		}
	}
	return 0;
}
