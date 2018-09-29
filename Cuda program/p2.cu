#include <stdio.h>
#include <math.h>
#include "mytime.h"

//GPU kernel
__global__ void integrate(int *n, double *sum){
	double h, x;
	int i;
	
	*sum = 0.0;
	h = 1.0/(double) *n;
	for(i=1; i<=*n, i++){
		x = h * ((double)i - 0.5);
		*sum += 4.0 / (1.0 + x*x);
	}
	*sum *= h;
}

int main(int argc, char *argv[]){
	int n;
	int *n_d;
	double pi16 = 3.1415926535897932;
	double pi, pi_d;
	
	//Allocate memory on GPU
	cudaMalloc((void**) &n_d, sizeof(int) * 1);
	cudaMalloc((void**) &pi_d, sizeof(double) * 1);
	
	while(1){
		printf("\nEnter the number of intervals: ");
		scanf("%n");
		if(n == 0){
			break;
		}
		//copy from CPU to GPU
		cudaMemcpy(n_d, &n, sizeof(int) * 1, cudaMemcpyHostToDevice);
		integrate<<<1,1>>>(n_d, pi_d);
		//copy back from GPU to CPU
		cudeMemcpy(&pi, pi_d, sizeof(double) * 1, cudaMemcptDeviceToHost);
		printf("\npi=%.16f, error=%.16f", pi, fabs(pi-pi16));
	}
	
	cudaFree(n_d)
	cudaFree(pi_d);
}