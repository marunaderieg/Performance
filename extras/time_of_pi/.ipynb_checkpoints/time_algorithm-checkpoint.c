//**Copyright to this code belongs to ... Original code has been edited by Maruna Derieg.**//

#include <omp.h>
#pragma cling load("libomp.so")
#include <stdio.h>
#include <stdlib.h>
#define REPEATS 100

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
    int repeats = REPEATS;
    int num_threads = 64;
    int n = 150000000;
    double time,time_min,time_start,time_end,fPi;

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
    
    //calculate pi multiple times and save minimal execution time
    for(int i=0;i<repeats;i++){
        time_start = omp_get_wtime();
        fPi = CalcPi(n, num_threads);
        time_end = omp_get_wtime();
        time = time_end-time_start;
        if (i==0){
            time_min = time;
        }
        else {
            if (time<time_min) time_min = time_end-time_start;
        }
    }
    printf("Smallest execution time of %d runs: %f \n",repeats,time_min);
}