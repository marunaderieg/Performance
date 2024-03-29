 /* Copyright belongs to https://github.com/avcourt, code accessed on https://github.com/avcourt/task-parallelism-omp/blob/master/code/quick_tasks.c. Code edited by Maruna Derieg. */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#pragma cling load("libomp.so")
#define LENGTH 1000000
#define NUMTHREADS 64

int partition (int p,   int r,   int *data){
    int x = data[p];
    int k = p;
    int l = r + 1;
    int t;
    while (1) {
        do
            k++;
        while ((data[k] <= x) && (k < r));
        do
            l--;
        while (data[l] > x);
        while (k < l) {
            t = data[k];
            data[k] = data[l];
            data[l] = t;
            do
                k++;
            while (data[k] <= x);
            do
                l--;
            while (data[l] > x);
        }
        t = data[p];
        data[p] = data[l];
        data[l] = t;
        return l;
    }
}

void seq_quick_sort (int p, int r, int *data){
    if (p < r) {
        int q = partition (p, r, data);
        seq_quick_sort (p, q - 1, data);
        seq_quick_sort (q + 1, r, data);
    }
}

void quick_sort (int p, int r, int *data, int low_limit) {
    if (p < r) {
        if ((r - p) < low_limit) {
            seq_quick_sort(p, r, data);
        }
        else {
            int q = partition (p, r, data);
            #pragma omp task firstprivate(data, low_limit, r, q)
                quick_sort (p, q - 1, data, low_limit);
            #pragma omp task firstprivate(data, low_limit, r, q)
                quick_sort (q + 1, r, data, low_limit);
        }
    }
}

void par_quick_sort (int n, int *data, int low_limit) {
    #pragma omp parallel
    {
        #pragma omp single nowait
            quick_sort (0, n, data, low_limit);
    }
}

void validate_sort (int n, int *data){
    int i;
    for (i = 0; i < n - 1; i++) {
        if (data[i] > data[i+1]) {
            printf ("ERROR: Validate failed\n");
            break;
        }
    }
}

int main (int argc, char *argv[]){

    //initialize data
    int n=LENGTH, num_threads=NUMTHREADS, low_limit=100, *data;
    
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
    
    // Generate the array.
    data = (int *)malloc (sizeof (int) * n);
    if (data == NULL) {
        printf("Error: Could not allocate array of size %d\n", n);
        return 1;
    }
    for (int i=0; i<n; i++ ) {
        data[i] = rand()%n;
    }
    
    //quicksort algorithm
    omp_set_num_threads(num_threads);
    par_quick_sort (n-1, &data[0], low_limit);
    
    //check if result is correct and free up space
    //validate_sort (n, &data[0]);
    //free (data);

    return 0;
}
