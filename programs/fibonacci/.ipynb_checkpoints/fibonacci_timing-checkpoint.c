#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#pragma cling load("libomp.so")
#define NUMTHREADS 64
#define REPEATS 10
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
    int n=N, fibonacci, num_threads=NUMTHREADS, repeats=REPEATS;
    double time,time_min,time_start,time_end;
    
    //check for passed arguments
    if (argc==1) {
        printf("You didn't pass any arguments.\n");
        printf("Default Value for number of threads (first argument) is set to: %d.\n",num_threads);
        printf("Default Value for number of repeats (second argumnet) is set to: %d.\n",repeats);
        printf("Default Value for element of the fibonacci sequence (third argument) is %d.\n",n);
    }
    
    else if (argc==2) {
        num_threads = atoi(argv[1]);
        printf("No value for number of repeats has been passed (second argument). Default is set to %d.\n",repeats);
        printf("No value for element of the fibonacci sequence has been passed (third argument). Default is set to: %d.\n",n);
    }
    
    else if (argc==3) {
        num_threads = atoi(argv[1]);
        repeats = atoi(argv[2]);
        printf("No value for n has been passed (third argument).\n N defines which element of the fibonacci seqence is computed. Default is set to: %d.\n",n);
    }
    
    else if (argc==4) {
        num_threads = atoi(argv[1]);
        repeats = atoi(argv[2]);
        n = atoi(argv[3]);
    }
    
    else {
        printf("You passed too many arguments. Arguments to be passed are: \n");
        printf("1.Number of Threads, 2.Number of Repeats, 3.Which element of fibonacci sequence (type integer).\n");
        printf("Default Value for number of threads is set to: %d.\n",num_threads);
        printf("Default Value for number of repeats is set to: %d.\n",repeats);
        printf("Default Value for element of the fibonacci sequence is %d.\n",n);
    }

    omp_set_num_threads(num_threads);
    
    //calculate fibonacci multiple times and save minimal execution time
    for(int i=0;i<repeats;i++){
        time_start = omp_get_wtime();
    #pragma omp parallel
        #pragma omp single
        {
        fibonacci=fib(n);
        }
        time_end = omp_get_wtime();
        time = time_end-time_start;
        if (i==0) time_min = time;
        else {
            if (time<time_min) time_min = time;
        }
    }
    
    //write smallest execution time to time file
    FILE * timefile;
    timefile = fopen ("./time.txt","w");
    fprintf (timefile, "%f",time_min);
    fclose (timefile);
    
    //output
    printf("fib(%d)=%d \n",n,fibonacci);
    printf("Smallest execution time of %d runs: %f \n",repeats,time_min);

    //error_check(n,fibonacci);
}