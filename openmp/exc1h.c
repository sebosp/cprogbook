#include <omp.h>
#include <stdio.h>

static long num_steps = 100000;
double step;

int
main(int argc, char *argv[])
{
	double pi,sum = 0.0;
	int i;
	step = 1.0/(double) num_steps;
	double ini_time = omp_get_wtime();
	# pragma omp parallel
	{
		double x;
		# pragma omp for reduction(+:sum)
		for(i = 0; i < num_steps; i++){
			x = (i + 0.5) * step;
			sum += 4.0/(1.0 + x * x);
		}
	}
	pi = step * sum;
	printf("Duration: [%f] result: [%f]\n",omp_get_wtime() - ini_time,pi);
	return 0;
}
