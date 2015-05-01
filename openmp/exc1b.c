#include <omp.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
	#pragma omp parallel
	{
		int ID = 0;
		printf("Hello(%d)",ID);
		printf(" world(%d)\n",ID);
	}
	return 0;
}
