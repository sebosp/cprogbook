#include <stdio.h>
// Celsius to fahrenheit
float fahr_to_celsius(float fahr){
	return (5.0/9.0) * (fahr - 32.0);
}
int main(int argc, char *argv[]){
	float fahr;
	int lower, upper, step;
	lower = 0;
	upper = 300;
	step = 20;
	fahr = lower;
	printf("CELSIUS\tFAHR\n"); // This is the header of the convertion table
	while(fahr <= upper){
		printf("%3.0f\t%6.1f\n",fahr_to_celsius(fahr),fahr);
		fahr=fahr+step;
	}
	return 0;
}
