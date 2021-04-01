//Copyright belongs to RWTH-Aachen University, ICT. Original code has been modified by Maruna Derieg.

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#pragma cling load("libomp.so")
#define REPEATS 10
#define N 40 //we calculate the n'th member of the fibonacci sequence

int ser_fib(int n)
{
  int x, y;
  if (n < 2)
    return n;

  x = ser_fib(n - 1);

  y = ser_fib(n - 2);

  return x+y;
}

/*
int fib(int n)
{
  int x, y;
  if (n < 2)
    return n;
  else if (n < 30)
    return ser_fib(n);

  #pragma omp task shared(x)
  {
    x = fib(n - 1);
  }

  #pragma omp task shared(y)
  {
    y = fib(n - 2);
  }

  #pragma omp taskwait

  return x+y;

}
/*

/*
void error_check(int n, int result){
    int true_result = ser_fib(n);
    if (true_result!=result) printf("There is a bug. Result should be %d",true_result);
} */

int main(int argc, char *argv[]){
    int i=0, n=N, fibonacci, repeats=REPEATS;
    double time,time_min,time_start,time_end;
    
    //check for passed arguments
    if (argc==1) {
        printf("You didn't pass any arguments.\n");
        printf("Default Value for number of repeats (first argumnet) is set to: %d.\n",repeats);
        printf("Default Value for element of the fibonacci sequence (second argument) is %d.\n",n);
    }
    
    else if (argc==2) {
        repeats = atoi(argv[1]);
        printf("No value for element of the fibonacci sequence has been passed (second argument). Default is set to: %d.\n",n);
    }
    
    else if (argc==3) {
        repeats = atoi(argv[1]);
        n = atoi(argv[2]);

    }
    
    else {
        printf("You passed too many arguments. Arguments to be passed are: \n");
        printf("1.Number of Repeats, 2.Which element of fibonacci sequence (type integer).\n");
        printf("Default Value for number of repeats is set to: %d.\n",repeats);
        printf("Default Value for element of the fibonacci sequence is %d.\n",n);
    }

    
    //calculate fibonacci multiple times and save minimal execution time
    for(i=0;i<repeats;i++){
        time_start = omp_get_wtime();
        fibonacci = ser_fib(n);
        time_end = omp_get_wtime();
        time = time_end-time_start;
        if (i==0) time_min = time;
        else {
            if (time<time_min) time_min = time;
        }
    }
    
    //write smallest execution time to time file
    FILE * timefile;
    timefile = fopen ("/tmp/time.txt","w");
    fprintf (timefile, "%f",time_min);
    fclose (timefile);
    
    //output
    printf("fib(%d)=%d \n",n,fibonacci);
    printf("Smallest execution time of %d runs: %f \n",repeats,time_min);

    //error_check(n,fibonacci);
}