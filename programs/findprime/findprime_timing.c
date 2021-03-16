//Copyright belongs to RWTH-Aachen University, ICT. Original code has been modified by Maruna Derieg.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#pragma cling load("libomp.so")
#define START 0
#define END 1000000
#define NUMTHREADS 64
#define REPEATS 10


static int gProgress, gPrimesFound;
long globalPrimes[1000000];

void ShowProgress( int val, int range ) 
{
    int percentDone = 0;
#pragma omp critical 
    {
    gProgress++;
    percentDone = (int)((float)gProgress/(float)range *200.0f + 0.5f);
    }
    if( percentDone % 10 == 0 ) printf("\b\b\b\b%3d%%", percentDone);
}

int TestForPrime(int val)
{
    int limit, factor = 3;

    limit = (long)(sqrtf((float)val)+0.5f);
    while( (factor <= limit) && (val % factor))
        factor ++;

    return (factor > limit);
}

void FindPrimes(int start, int end)
{
    // start is always odd
    int range = end - start + 1;
    int i;

#pragma omp parallel for
    for( i = start; i <= end; i += 2 )
    {
       if( TestForPrime(i) )
       {
    #pragma omp critical
            globalPrimes[gPrimesFound++] = i; // datarace when incrementing
       }
       //ShowProgress(i, range);
    }
}

int main (int argc, char *argv[])
{
    //initialize data
    int start=START, end=END, num_threads=NUMTHREADS, repeats=REPEATS;
    double time,time_min,time_start,time_end;
    
    //read passed arguments
    if (argc==1){
        printf("No arguments have been passed. Default values are set to: \n");
        printf("1. Number of Threads = %d.\n",num_threads);
        printf("2. Number of Repeats = %d.\n",repeats);
        printf("3. Start of Interval, within which prime numbers will be searched for: %d.\n",start);
        printf("4. End of Interval, within which prime numbers will be searched for: %d.\n",end);
    }
    else if (argc==2){
        num_threads = atoi(argv[1]);
        printf("No arguments for Repeats and Interval have been passed. Default values are set to: \n");
        printf("2. Number of Repeats = %d.\n",repeats);
        printf("3. Start of Interval, within which prime numbers will be searched for: %d.\n",start);
        printf("4. End of Interval, within which prime numbers will be searched for: %d.\n",end);
    }
    else if (argc==3){
        num_threads = atoi(argv[1]);
        repeats = atoi(argv[2]);
        printf("No arguments for Interval have been passed. Default values are set to: \n");
        printf("3. Start of Interval, within which prime numbers will be searched for: %d.\n",start);
        printf("4. End of Interval, within which prime numbers will be searched for: %d.\n",end);
    }
    else if (argc==4){
        num_threads = atoi(argv[1]);
        repeats = atoi(argv[2]);
        printf("Error: No arguments for end of Interval has been passed. Default values are set to: \n");
        printf("3. Start of Interval, within which prime numbers will be searched for: %d.\n",start);
        printf("4. End of Interval, within which prime numbers will be searched for: %d.\n",end);
    }
    else if (argc==5){
        num_threads = atoi(argv[1]);
        repeats = atoi(argv[2]);
        start = atoi(argv[3]);
        end = atoi(argv[4]);
        if (end<start){
            start = START;
            end = END;
            printf("Error: End of interval has to be larger or equal to start of interval.\n"); 
            printf("Default values are set to: \n");
            printf("3. Start of Interval, within which prime numbers will be searched for: %d.\n",start);
            printf("4. End of Interval, within which prime numbers will be searched for: %d.\n",end);    
        }
        if (start<0){
            start = START;
            end = END;
            printf("Error: Start and End of interval have to be positive integers.\n"); 
            printf("Default values are set to: \n");
            printf("3. Start of Interval, within which prime numbers will be searched for: %d.\n",start);
            printf("4. End of Interval, within which prime numbers will be searched for: %d.\n",end);    
        }
    }
    
    else {
        printf("Too many arguments have been passed. Default values are set to: \n");
        printf("1. Number of Threads = %d\n",num_threads);
        printf("2. Number of Repeats = %d.\n",repeats);
        printf("3. Start of Interval, within which prime numbers will be searched for: %d.\n",start);
        printf("4. End of Interval, within which prime numbers will be searched for: %d.\n",end);
    }
    
    // ensure we start with an odd number
    if((start % 2) == 0 )
        start = start + 1;  

    //compute prime numbers multiple times and write minimal execution time to time file
    omp_set_num_threads(num_threads);
    
    for(int i=0;i<repeats;i++){
        gProgress = 0;
        gPrimesFound = 0;
        time_start = omp_get_wtime();
        FindPrimes(start, end);
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
    printf("\n%d primes found between %d and %d.\n", gPrimesFound, start, end);
    printf("Smallest execution time of %d runs: %f \n",repeats,time_min);
    
    return 0;
}