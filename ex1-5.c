#include <stdio.h>
// Reverse order of conversion table
main(){
	float fahr, celsius;
	int lower, upper, step;
	lower = 0;
	upper = 300;
	step = 20;
	fahr = upper;
	printf("CELSIUS\tFAHR\n",fahr,celsius);
	while(fahr >= lower){
		celsius = (5.0/9.0) * (fahr - 32.0);
		printf("%3.0f\t%6.1f\n",celsius,fahr);
		fahr=fahr-step;
	}
}
