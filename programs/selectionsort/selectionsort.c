#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <time.h>
#pragma cling load("libomp.so")

/*
Copyright belongs to https://madhugnadig.com/articles/parallel-processing/2017/02/25/parallel-computing-in-c-using-openMP.html. Code has been edited by Maruna Derieg. 
*/

// Structure for enabling reduction on the index of elements
struct Compare { int val; int index; };

// Custom reduction for finding the index of the max element.
#pragma omp declare reduction(maximum : struct Compare : omp_out = omp_in.val > omp_out.val ? omp_in : omp_out)

void swap(int* a, int* b);
void selectionSort(int* A, int n);
void verify(int* A, int n);
void verify(int* A, int n);

int main(int argc, char *argv[]){

    //measure time    
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    
    int number, i;
    int* Arr;

    number = 1000000;
    if (argc >1) {
            number = atoi(argv[1]);
    }
    else {
        printf ("No value for list length has been provided (first argument). Default length is 1000000.\n");
    }

    Arr = (int*)malloc(number * sizeof(int));
    if (Arr == NULL) {
        printf("Error: Could not allocate array of size %d\n",number);
    return 1;
    }
    for (i=0; i<number; i++ ) {
        Arr[i] = rand()%number;
    }
    
    selectionSort(Arr, number);

    //measure time
    clock_gettime(CLOCK_REALTIME, &end); 
    double time_spent_sec= (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9;
    printf("Time spent: %g \n",time_spent_sec);
    
    // Verify if the algorithm works as advised
    verify(Arr, number);

    free(Arr);
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

// Verification function
void verify(int* A, int n){
    int failcount = 0;
    for(int iter = 0; iter < n-1; iter++){
        if(A[iter] < A[iter+1]){
            failcount++;
        }
    }
    if (failcount>0){
        printf("Array has not sorted correctly.");
    }
}

//Swap function
void swap(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
} 

