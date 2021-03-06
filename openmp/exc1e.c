#include <omp.h>
#include <stdio.h>

static long num_steps = 100000;
double step;

int
main(int argc, char *argv[])
{
	double pi = 0.0;
	double sum[omp_get_num_threads()];
	int i,nthreads = 0;
	for(i = 0; i < omp_get_num_threads(); i++){
		sum[i] = 0.0;
	}
	step = 1.0/(double) num_steps;
	double ini_time = omp_get_wtime();
	# pragma omp parallel
	{
		double x;
		int i,id,nthrds;
		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		if (id == 0) nthreads = nthrds; 
		printf("{%d}: [%ld,%ld]\n",
			id,
			id * (num_steps/nthrds),
			(id + 1) * (num_steps/nthrds)
			);
		for(i = id * (num_steps/nthrds), sum[id] = 0.0;
		    i < (id + 1) *(num_steps/nthrds);
		    i++){

//		for(i = 0; i < num_steps; i++){
			x = (i + 0.5) * step;
			sum[id] += 4.0/(1.0 + x * x);
		}
	}
	for(i = 0; i < nthreads; i++){
		pi += step * sum[i];
	}
	printf("Duration: [%f] result: [%f]\n",omp_get_wtime() - ini_time,pi);
	return 0;
}
