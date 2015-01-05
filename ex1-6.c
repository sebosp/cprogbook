#include <stdio.h>
// Is getchar() != EOF is 0 or 1.
int main(int argc, char *argv[]){
	int res;
	while(res = (getchar() != EOF)){
		printf("res is %i\n",res);
	}
	printf("Last response is: %i, (getchar() != EOF) is: %s\n",EOF,(res?"YES":"NO"));
	return 0;
}
