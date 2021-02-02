/* Copyright for this code belongs to https://github.com/Malhadas/Parallel-BubbleSort-/blob/master/bubble.c. Code has been edited by Maruna Derieg. */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void bubble_sort(int a[], int n);
void validate_sort (int n, int *data);
void swap();
void bubblesort_parallel (int a[], int N);

int main(int argc, char* argv[]) {
    //measure time    
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    
    //initialize data
    int i,j;
    int n = 1000000;
    if (argc > 1) {
            n = atoi(argv[1]);
    }
    else {
        printf ("No value for list length (first argument) has been provided. Default length is 1000000.\n");
    }
    
    // Generate the array.
    int *a = (int *)malloc (sizeof (int) * n);
    if (a == NULL) {
        printf("Error: Could not allocate array of size %d\n", n);
        return 1;
    }
    for (i=0; i<n; i++ ) {
        a[i] = rand()%n;
    }
    
    //quicksort algorithm
    bubblesort_parallel(a, n);
    
    //measure time
    clock_gettime(CLOCK_REALTIME, &end); 
    double time_spent_sec= (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9;
    printf("Time spent: %g \n",time_spent_sec);
    
    //check if result is correct and free up space
    validate_sort (n, &a[0]);
    free (a);
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