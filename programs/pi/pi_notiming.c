//**Copyright to this code belongs to ... Original code has been edited by Maruna Derieg.**//

#include <omp.h>
#pragma cling load("libomp.so")
#include <stdio.h>
#include <stdlib.h>
#define INTERVALS 150000000
#define NUMTHREADS 64

double f(double a)
{
    return (4.0 / (1.0 + a*a));
}

double CalcPi(int n, int thread_nr)
{
    const double fH   = 1.0 / (double) n;
    double fSum = 0.0;
    double fX;
    int i;

# pragma omp parallel num_threads(thread_nr)  
    {
    double fX;
    # pragma omp single nowait
        printf("Number of Threads: %d \n",omp_get_num_threads());
# pragma omp for reduction(+:fSum)
    for (i = 0; i < n; i += 1)
    {
        fX = fH * ((double)i + 0.5);
        fSum += f(fX);
    }
    }
    return fH * fSum;
}

int main(int argc, char *argv[]){
    //initialize data
    double fPi;
    int num_threads = NUMTHREADS;
    int n = INTERVALS;
    
    //check for passed arguments
    if (argc == 1) {
        printf("No arguments have been passed.\n"); 
        printf("Default value for number of threads (first argument) is: %d.\n", num_threads);
        printf ("Default value for number of intervals (second argument) is: %d.\n", n);
    }
    
    else if (argc == 2) {
        num_threads = atoi(argv[1]);
        printf("No argument for number of intervals has been passed.\n"); 
        printf ("Default value for number of intervals (second argument) is: %d.\n", n);
    }
    
    else if (argc == 3) {
        num_threads = atoi(argv[1]);
        n = atoi(argv[2]);
    }
    
    else {
        printf("Error: Too many arguments have been passed.\n");
        printf("Default value for number of threads (first argument) is: %d.\n", num_threads);
        printf ("Default value for number of intervals (second argument) is: %d.\n", n);
    }

    
    fPi = CalcPi(n, num_threads);
    
    printf("Calculated Pi is: %f \n",fPi);
    
    return 0;
}