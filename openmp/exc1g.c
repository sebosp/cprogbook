#include <omp.h>
#include <stdio.h>

static long num_steps = 100000;
double step;

int
main(int argc, char *argv[])
{
	double pi = 0.0;
	double sum = 0.0;
	int i,nthreads = 0;
	step = 1.0/(double) num_steps;
	double ini_time = omp_get_wtime();
	# pragma omp parallel
	{
		double x;
		int i,id,nthrds;
		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		if (id == 0)
			nthreads = nthrds; 
		for(i = id; i < num_steps; i+=nthrds){
			# pragma omp atomic
			{
			x = (i + 0.5) * step;
			sum += 4.0/(1.0 + x * x);
			}
		}
	}
	pi = step * sum;
	printf("Duration: [%f] result: [%f]\n",omp_get_wtime() - ini_time,pi);
	return 0;
}
