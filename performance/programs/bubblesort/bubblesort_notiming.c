/* Copyright for this code belongs to https://github.com/Malhadas/Parallel-BubbleSort-/blob/master/bubble.c. Code has been edited by Maruna Derieg. */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#pragma cling load("libomp.so")
#define LENGTH 10000
#define NUMTHREADS 64

void bubble_sort(int a[], int n);
void validate_sort (int n, int *data);
void swap();
void bubblesort_parallel (int a[], int N);

int main(int argc, char* argv[]) {

    //initialize data
    int n=LENGTH, num_threads=NUMTHREADS, i, j;

    //check for passed arguments
    if (argc == 1) {
        printf("No arguments have been passed. Default values are: \n"); 
        printf("1. Number of threads: %d.\n", num_threads);
        printf ("2. Length of array to be sorted: %d.\n", n);
    }  
    else if (argc == 2) {
        num_threads = atoi(argv[1]);
        printf("No argument for list length has been passed. Default value is: \n"); 
        printf ("2. Length of array to be sorted: %d.\n", n);
    } 
    else if (argc == 3) {
        num_threads = atoi(argv[1]);
        n = atoi(argv[2]);
    } 
    else{
        printf("Error: Too many arguments have been passed. Defaults are set to:\n");
        printf("1. Number of threads: %d.\n", num_threads);
        printf ("2. Length of array to be sorted: %d.\n", n);
    }
    
    //generate the array.
    int *a = (int *)malloc (sizeof (int) * n);
    if (a == NULL) {
        printf("Error: Could not allocate array of n %d\n", n);
        return 1;
    }
    for (i=0; i<n; i++ ) {
        a[i] = rand()%n;
    }
    
    //bubblesort algorithm
    omp_set_num_threads(num_threads);
    bubblesort_parallel(a, n);
    
    //check if result is correct and free up space
    validate_sort (n, &a[0]);
    free (a);
    
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