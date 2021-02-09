/* Copyright for this code belongs to https://github.com/Malhadas/Parallel-BubbleSort-/blob/master/bubble.c. Code has been edited by Maruna Derieg. */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#pragma cling load("libomp.so")
#define LENGTH 10000
#define NUMTHREADS 64
#define REPEATS 10

void bubble_sort(int a[], int n);
void validate_sort (int n, int *data);
void swap();
void bubblesort_parallel (int a[], int N);

int main(int argc, char* argv[]) {

    //initialize data
    int n=LENGTH, num_threads=NUMTHREADS, repeats=REPEATS, i, j;
    double time, time_min, time_start, time_end;


    //check for passed arguments
    if (argc == 1) {
        printf("No arguments have been passed. Default values are: \n"); 
        printf("1. Number of threads: %d.\n", num_threads);
        printf("2. Number of repeats: %d.\n", repeats);
        printf ("3. Length of array to be sorted: %d.\n", n);
    } 
    else if (argc == 2) {
        num_threads = atoi(argv[1]);
        printf("No arguments for number of repeats and list length have been passed. Default values are: \n"); 
        printf("2. Number of repeats: %d.\n", repeats);
        printf ("3. Length of array to be sorted: %d.\n", n);
    } 
    else if (argc == 3) {
        num_threads = atoi(argv[1]);
        repeats = atoi(argv[2]);
        printf("No argument for list length has been passed. Default value is: \n"); 
        printf ("3. Length of array to be sorted: %d.\n", n);
    } 
    else if (argc == 4) {
        num_threads = atoi(argv[1]);
        repeats = atoi(argv[2]);
        n = atoi(argv[3]);
    } 
    else{
        printf("Error: Too many arguments have been passed. Defaults are set to:\n");
        printf("1. Number of threads: %d.\n", num_threads);
        printf("2. Number of repeats: %d.\n", repeats);
        printf ("3. Length of array to be sorted: %d.\n", n);
    }

    // execute bubblesort algorithm multiple times and save smallest execution time to time file
    for(int iteration=0;iteration<repeats;iteration++){
        //generate the array.
        int *a = (int *)malloc (sizeof (int) * n);
        if (a == NULL) {
            printf("Error: Could not allocate array of n %d\n", n);
            return 1;
        }
        for (i=0; i<n; i++ ) {
            a[i] = rand()%n;
        }
        //measure time while running algorithm
        omp_set_num_threads(num_threads);
        time_start = omp_get_wtime();
        bubblesort_parallel(a, n);
        time_end = omp_get_wtime();
        time = time_end-time_start;
        if (iteration==0) time_min = time;
        else {
            if (time<time_min) time_min = time;
        }
        //check result and free data
        validate_sort (n, &a[0]);
        free(a);
    }

    //write smallest execution time to time file
    FILE * timefile;
    timefile = fopen ("./time.txt","w");
    fprintf (timefile, "%f",time_min);
    fclose (timefile);
    
    //output
    printf("Smallest execution time of %d runs: %f \n",repeats,time_min);
    
    return 0;
} 


void bubblesort_parallel (int a[], int N) {
    int i=0, j=0; 
    int first;
    for( i = 0; i < N-1; i++ )
    {
        first = i % 2; 
        #pragma omp parallel for default(none),shared(a,first,N)
        for( j = first; j < N-1; j += 1 )
        {
            if(a[j]>a[j+1])
            {
                swap(&a[j], &a[j+1]);
            }
        }
    }
}


void swap(int *num1, int *num2)
{

    int temp = *num1;
    *num1 =  *num2;
    *num2 = temp;
}

void validate_sort (int n, int *a){
    int i;
    for (i = 0; i < n - 1; i++) {
        if (a[i] > a[i+1]) {
            printf ("ERROR: Validate failed\n");
            break;
        }
    }
}