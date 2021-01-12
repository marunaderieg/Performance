#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>
#include <time.h>
#pragma cling load("libomp.so")

 /* Copyright belongs to https://github.com/avcourt, code accessed on https://github.com/avcourt/task-parallelism-omp/blob/master/code/merge_tasks_unnested.c. Code edited by Maruna Derieg. */


void merge(int a[], int size, int temp[]);

void mergesort_serial(int a[], int size, int temp[]);

void mergesort_parallel_omp(int a[], int size, int temp[], int thresh);


int main(int argc, char *argv[]) {
    
    //measure time    
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    
    //initialize data
    int size = 1000000;
    if (argc == 2) {
        size = atoi(argv[1]);
    }
    else {
        printf ("No value for list length has been provided (first argument). Default length is 1000000.\n");
    }
    int thresh = 100;    // Threshold

    // Array allocation
    int *a = malloc(sizeof(int) * size);
    int *temp = malloc(sizeof(int) * size);
    if (a == NULL || temp == NULL) {
        printf("Error: Could not allocate array of size %d\n", size);
        return 1;
    }

    // array initialization
    for (int i = 0; i < size; i++) {
        a[i] = rand() % size;
    }

    // run sort and get time
    #pragma omp parallel
    {
        #pragma omp single nowait
            mergesort_parallel_omp (a, size, temp, thresh);
    }   
 
    //measure time
    clock_gettime(CLOCK_REALTIME, &end); 
    double time_spent_sec= (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9;
    printf("Time spent: %g \n",time_spent_sec);

    //validate if sorted correctly
    for (int i = 1; i < size; i++) {
        if (!(a[i - 1] <= a[i])) {
            printf("Error: final array not sorted => a[%d]=%d > a[%d]=%d\n", i - 1,
                   a[i - 1], i, a[i]);
            return 1;
        }
    }
    return 0;
}

// OpenMP merge sort with given number of threads
void mergesort_parallel_omp(int a[], int size, int temp[], int thresh) {
    if (size <= thresh) {
        mergesort_serial(a, size, temp);
    }
    else {
        #pragma omp task
        {
            mergesort_parallel_omp(a, size / 2, temp, thresh);
        }
        #pragma omp task
        {
            mergesort_parallel_omp(a + size / 2, size - size / 2,
                                   temp + size / 2, thresh);
        }
        #pragma omp taskwait
        {
            merge(a, size, temp);
        }

    }
}


// only called if num_threads = 1
void mergesort_serial(int a[], int size, int temp[]) {
    // Switch to insertion sort for small arrays
    if (size > 1) {
        mergesort_serial(a, size / 2, temp);
        mergesort_serial(a + size / 2, size - size / 2, temp);
        merge(a, size, temp);
    }
}

void merge(int a[], int size, int temp[]) {
    int i1 = 0;
    int i2 = size / 2;
    int tempi = 0;
    while (i1 < size / 2 && i2 < size) {
        if (a[i1] < a[i2]) {
            temp[tempi] = a[i1];
            i1++;
        } else {
            temp[tempi] = a[i2];
            i2++;
        }
        tempi++;
    }
    while (i1 < size / 2) {
        temp[tempi] = a[i1];
        i1++;
        tempi++;
    }
    while (i2 < size) {
        temp[tempi] = a[i2];
        i2++;
        tempi++;
    }
    // Copy sorted temp array into main array, a
    memcpy(a, temp, size * sizeof(int));
}