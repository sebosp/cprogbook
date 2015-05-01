#include <omp.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
	double A[1000];
	omp_set_num_threads(4);
	#pragma omp parallel
	{
		int ID = omp_get_num_thread();
		pooh(ID,A);
		printf("Hello(%d)",ID);
	}
	printf("All done\n");
	return 0;
}
