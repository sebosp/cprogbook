#include <stdio.h>
// Is getchar() != EOF is 0 or 1.
main(){
	int res;
	while(res = (getchar() != EOF)){
		printf("res is %i\n",res);
	}
	printf("Last response is: %i, (getchar() != EOF) is: %s\n",EOF,(res?"YES":"NO"));
}
