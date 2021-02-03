//**Copyright to this code belongs to ... Original code has been edited by Maruna Derieg.**//

#include <omp.h>
#pragma cling load("libomp.so")
#include <stdio.h>
#include <stdlib.h>
#define REPEATS 10
#define NUMTHREADS 64
#define INTERVALS 150000000

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
    int num_threads = NUMTHREADS;
    int n = INTERVALS;
    double time,time_min,time_start,time_end,fPi;

    
    //check for passed arguments
    if (argc == 1) {
        printf("No arguments have been passed.\n"); 
        printf("Default value for number of threads (first argument) is: %d.\n", num_threads);
        printf("Default value for repeats (second argument) is: %d.\n", repeats);
        printf ("Default value for number of intervals (third argument) is: %d.\n", n);
    }
    
    else if (argc == 2) {
        num_threads = atoi(argv[1]);
        printf("No argument for repeats and number of intervals have been passed.\n"); 
        printf("Default value for repeats (second argument) is: %d.\n", repeats);
        printf ("Default value for number of intervals (third argument) is: %d.\n", n);
    }
    
    else if (argc == 3) {
        num_threads = atoi(argv[1]);
        repeats = atoi(argv[2]);
        printf("No argument for number of intervals has been passed.\n");
        printf ("Default value for number of intervals (third argument) is: %d.\n", n);
    }
    
    else if (argc == 4) {
        num_threads = atoi(argv[1]);
        repeats = atoi(argv[2]);
        n = atoi(argv[3]);
    }
    
    else {
        printf("Error: Too many arguments have been passed.\n")
        printf("Default value for number of threads (first argument) is: %d.\n", num_threads);
        printf("Default value for repeats (second argument) is: %d.\n", repeats);
        printf ("Default value for number of intervals (third argument) is: %d.\n", n);
    }

    
    //calculate pi multiple times and save minimal execution time
    for(int i=0;i<repeats;i++){
        time_start = omp_get_wtime();
        fPi = CalcPi(n, num_threads);
        time_end = omp_get_wtime();
        time = time_end-time_start;
        if (i==0) time_min = time;
        else {
            if (time<time_min) time_min = time;
        }
    }
    
    //write smallest execution time to time file
    FILE * timefile;
    timefile = fopen ("./time.txt","w");
    fprintf (timefile, "%f",time_min);
    fclose (timefile);
    
    //output
    printf("Smallest execution time of %d runs: %f \n",repeats,time_min);
}