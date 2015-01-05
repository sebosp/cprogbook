#include <stdio.h>
// Is getchar() != EOF is 0 or 1.
int main(int argc, char *argv[]){
	int c,tabs,spaces,newlines,others;
	c=tabs=spaces=newlines=others = 0;
	while((c = getchar()) != EOF){
		switch(c){
			case '\t': ++tabs;
				   break;
			case  ' ': ++spaces;
				   break;
			case '\n': ++newlines;
				   break;
			default: ++others;
		}
	}
	printf("Tabs: %i, spaces: %i, new-lines: %i, others: %i\n",tabs,spaces,newlines,others);
	return 0;
}
