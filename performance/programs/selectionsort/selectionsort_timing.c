#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#pragma cling load("libomp.so")
#define LENGTH 10000
#define NUMTHREADS 64
#define REPEATS 10 


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
    int size=LENGTH, num_threads=NUMTHREADS, repeats=REPEATS;
    double time, time_min, time_start, time_end;

    
    //check for passed arguments
    if (argc == 1) {
        printf("No arguments have been passed. Default values are: \n"); 
        printf("1. Number of threads: %d.\n", num_threads);
        printf("2. Number of repeats: %d.\n", repeats);
        printf ("3. Length of array to be sorted: %d.\n", size);
    } 
    else if (argc == 2) {
        num_threads = atoi(argv[1]);
        printf("No arguments for number of repeats and list length have been passed. Default values are: \n"); 
        printf("2. Number of repeats: %d.\n", repeats);
        printf ("3. Length of array to be sorted: %d.\n", size);
    } 
    else if (argc == 3) {
        num_threads = atoi(argv[1]);
        repeats = atoi(argv[2]);
        printf("No argument for list length has been passed. Default value is: \n"); 
        printf ("3. Length of array to be sorted: %d.\n", size);
    } 
    else if (argc == 4) {
        num_threads = atoi(argv[1]);
        repeats = atoi(argv[2]);
        size = atoi(argv[3]);
    } 
    else{
        printf("Error: Too many arguments have been passed. Defaults are set to:\n");
        printf("1. Number of threads: %d.\n", num_threads);
        printf("2. Number of repeats: %d.\n", repeats);
        printf ("3. Length of array to be sorted: %d.\n", size);
    }
    
    // execute selectionsort algorithm multiple times and save smallest execution time to time file
    for(int iteration=0;iteration<repeats;iteration++){
        //generate array of random numbers
        int *Arr = (int*)malloc(size * sizeof(int));
        if (Arr == NULL) {
            printf("Error: Could not allocate array of size %d\n",size);
        return 1;
        }
        for (int i=0; i<size; i++ ) {
            Arr[i] = rand()%size;
        }
        //measure time while running algorithm
        omp_set_num_threads(num_threads);
        time_start = omp_get_wtime();
        selectionSort(Arr, size);         
        time_end = omp_get_wtime();
        time = time_end-time_start;
        if (iteration==0) time_min = time;
        else {
            if (time<time_min) time_min = time;
        }
        //check result and free data
        //validate_sort (size, &Arr[0]);
        free(Arr);
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

