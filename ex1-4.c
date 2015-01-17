#include <stdio.h>
// Celsius to fahrenheit
int main(int argc, char *argv[]){
	float fahr, celsius;
	int lower, upper, step;
	lower = 0;
	upper = 300;
	step = 20;
	fahr = lower;
	celsius=0;
	printf("CELSIUS\tFAHR\n"); // This is the header of the convertion table
	while(fahr <= upper){
		celsius = (5.0/9.0) * (fahr - 32.0);
		printf("%3.0f\t%6.1f\n",celsius,fahr);
		fahr=fahr+step;
	}
	return 0;
}
