//**Copyright to this code belongs to ... Original code has been edited by Maruna Derieg.**//

#include <omp.h>
#pragma cling load("libomp.so")
#include <stdio.h>
#include <stdlib.h>
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
    int n = 150000000;
    //check for passed arguments
    if (argc > 1) {
        num_threads = atoi(argv[1]);
    }
    else {
        printf ("No value for requested number of threads has been passed (first argument). Default is 64.\n");
    } 
    if (argc > 2) {
        n = atoi(argv[2]);
    }
    else {
        printf ("No value for number of intervals has been passed (second argument). Default is 150'000'000.\n");
    }
    
    fPi = CalcPi(n, num_threads);
    
    printf("Calculated Pi is: %f \n",fPi);
}