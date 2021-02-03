#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#pragma cling load("libomp.so")
#define NUMTHREADS 64
#define N 40 //we calculate the n't member of the fibonacci sequence


int ser_fib(int n)
{
  int x, y;
  if (n < 2)
    return n;

  x = ser_fib(n - 1);

  y = ser_fib(n - 2);

  return x+y;
}

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
void error_check(int n, int result){
    int true_result = ser_fib(n);
    if (true_result!=result) printf("There is a bug. Result should be %d",true_result);
} */

int main(int argc, char *argv[]){
    //initalize data
    int n=N, fibonacci, num_threads=NUMTHREADS;
    
    //check for passed arguments
    if (argc==1) {
        printf("You didn't pass any arguments. Arguments to be passed are: \n 1.Number of Threads, 2.Which element of fibonacci sequence (type integer).\n");
        printf("Default Value for number of threads is set to: %d.\n",num_threads);
        printf("Default Value for element of the fibonacci sequence is %d.\n",n);
    }
    
    else if (argc==2) {
        num_threads = atoi(argv[1]);
        printf("No value for n has been passed (second argument).\n"); 
        printf("N defines which element of the fibonacci seqence is computed. Default is set to: %d.\n",n);
    }
    
    else if (argc==3) {
        num_threads = atoi(argv[1]);
        n = atoi(argv[2]);
    }
    
    else {
        printf("You passed too many arguments. Arguments to be passed are: \n");
        printf("1.Number of Threads, 2.Which element of fibonacci sequence (type integer).\n");
        printf("Default Value for number of threads is set to: %d.\n",num_threads);
        printf("Default Value for element of the fibonacci sequence is %d.\n",n);
    }

    omp_set_num_threads(num_threads);
#pragma omp parallel
    #pragma omp single
    {
    fibonacci=fib(n);
    }

    printf("fib(%d)=%d \n",n,fibonacci);
    //error_check(n,fibonacci);
}