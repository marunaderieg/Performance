#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#pragma cling load("libomp.so")
#define LENGTH 10000
#define NUMTHREADS 64

/*
Copyright belongs to https://madhugnadig.com/articles/parallel-processing/2017/02/25/parallel-computing-in-c-using-openMP.html. Code has been edited by Maruna Derieg. 
*/

// Structure for enabling reduction on the index of elements
struct Compare { int val; int index; };

// Custom reduction for finding the index of the max element.
#pragma omp declare reduction(maximum : struct Compare : omp_out = omp_in.val > omp_out.val ? omp_in : omp_out)

void swap(int* a, int* b);
void selectionSort(int* A, int n);
void validate_sort (int n, int *data);

int main(int argc, char *argv[]){

    //initialzie data
    int size=LENGTH, num_threads=NUMTHREADS, *Arr;
    
    //check for passed arguments
    if (argc == 1) {
        printf("No arguments have been passed. Default values are: \n"); 
        printf("1. Number of threads: %d.\n", num_threads);
        printf ("2. Length of array to be sorted: %d.\n", size);
    }  
    else if (argc == 2) {
        num_threads = atoi(argv[1]);
        printf("No argument for list length has been passed. Default value is: \n"); 
        printf ("2. Length of array to be sorted: %d.\n", size);
    } 
    else if (argc == 3) {
        num_threads = atoi(argv[1]);
        size = atoi(argv[2]);
    } 
    else{
        printf("Error: Too many arguments have been passed. Defaults are set to:\n");
        printf("1. Number of threads: %d.\n", num_threads);
        printf ("2. Length of array to be sorted: %d.\n", size);
    }

    //generate array of random numbers
    Arr = (int*)malloc(size * sizeof(int));
    if (Arr == NULL) {
        printf("Error: Could not allocate array of size %d\n",size);
    return 1;
    }
    for (int i=0; i<size; i++ ) {
        Arr[i] = rand()%size;
    }
    
    //run selectionsort algorithm
    omp_set_num_threads(num_threads);
    selectionSort(Arr, size);
    
    //error check and free up data
    //validate_sort (size, &Arr[0]);
    //free(Arr);
    
    return 0;

}

void selectionSort(int* A, int n){

    for(int startpos =0; startpos < n; startpos++){
        // Declare the structure required for reduction
        struct Compare max;
        // Initialize the variables
        max.val = A[startpos];
        max.index = startpos;

        // Parallel for loop with custom reduction, at the end of the loop, max will have the max element and its index.
        #pragma omp parallel for reduction(maximum:max)
        for(int i=startpos +1; i< n; ++i){
            if(A[i] > max.val){
                max.val = A[i];
                max.index = i;
            }
        }

        swap(&A[startpos], &A[max.index]);
    }
}

// Error Check
void validate_sort (int n, int *data){
    int i;
    for (i = 0; i < n - 1; i++) {
        if (data[i] < data[i+1]) {
            printf ("ERROR: Validate failed\n");
            break;
        }
    }
}

//Swap function
void swap(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
} 

