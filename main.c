#define _POSIX_C_SOURCE 200809L
#include <omp.h>
#include <openacc.h>
#include <malloc.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

inline float op(float a, float b, float c)
{
	return a * b + c;
}

int main()
{
	float *a, *b, *c, *d;
	int err = 0;
	//char mychar;
	
	err |= posix_memalign((void**)&a, 32, 1024l * 1024l * 1024l * sizeof(float));
	err |= posix_memalign((void**)&b, 32, 1024l * 1024l * 1024l * sizeof(float));
	err |= posix_memalign((void**)&c, 32, 1024l * 1024l * 1024l * sizeof(float));
	err |= posix_memalign((void**)&d, 32, 1024l * 1024l * 1024l * sizeof(float));
	
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-result"
	//scanf("%c", &mychar);
	#pragma GCC diagnostic pop
	
	if (err != 0)
	{
		printf("Couldn't allocate memory, error code %d.", err);
		return 1;
	}
	
	printf("Allocated memory, error code %d.\n", err);
	
	#pragma omp parallel num_threads(32)
	{
		unsigned int seed = omp_get_thread_num();
		
		#pragma omp for
		for (long long i = 0; i < 1024l * 1024l * 1024l; i++)
		{
			//printf("here %d\n", i);
			a[i] = (float)rand_r(&seed) / (float)RAND_MAX;
			b[i] = (float)rand_r(&seed) / (float)RAND_MAX;
			c[i] = (float)rand_r(&seed) / (float)RAND_MAX;
		}
	}
	
	printf("Done setting random values.\n");
	
	double singledur = omp_get_wtime();
	
	for (long long i = 0; i < 1024l * 1024l * 1024l; i++)
	{
		d[i] = op(a[i], b[i], c[i]);
	}
	
	singledur = omp_get_wtime() - singledur;
	
	printf("Serial CPU took %lf to compute\n", singledur);
	
	double cpudur = omp_get_wtime();
	
	#pragma omp parallel for
	for (long long i = 0; i < 1024l * 1024l * 1024l; i++)
	{
		d[i] = op(a[i], b[i], c[i]);
	}
	
	cpudur = omp_get_wtime() - cpudur;
	
	printf("Parallel CPU took %lf to compute\n", cpudur);
	
	double gpudur = omp_get_wtime();
	
	#pragma acc parallel loop
	for (long long i = 0; i < 1024l * 1024l * 1024l; i++)
	{
		d[i] = op(a[i], b[i], c[i]);
	}
	
	gpudur = omp_get_wtime() - gpudur;
	
	printf("GPU took %lf to compute\n", gpudur);
	
	free(a);
	free(b);
	free(c);
	free(d);
	return 0;
	
}