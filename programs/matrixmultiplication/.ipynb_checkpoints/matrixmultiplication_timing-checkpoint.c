/*copyright to this code belongs to John Burkardt. Code has been edited by Maruna Derieg. Original code was accessed on 9.2.2021 on https://people.sc.fsu.edu/~jburkardt/c_src/mxm_openmp/mxm_openmp.c. */

# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <omp.h>
# pragma cling load("libomp.so")
# define NUMTHREADS 64
# define SIZE 500
#define REPEATS 10

void error_check(int n, int *a, int *b, int *c);

int main (int argc, char *argv[])
{
  //initalize data  
  int i, j, k, n = SIZE, num_threads=NUMTHREADS, repeats=REPEATS;
  double time, time_min, time_start, time_end;

  //check for passed arguments
  if (argc==1){
      printf("No arguments have been passed. Default values are set to: \n");
      printf("1. Number of Threads: %d.\n",num_threads);
      printf("2. Number of Repeats: %d. \n",repeats);
      printf("3. Size of Matrices: %d.\n",n);
  }    
  else if (argc==2){
      num_threads = atoi(argv[1]);
      printf("No arguments for Repeats and Matrix size have been passed. Default values are set to: \n");
      printf("2. Number of Repeats: %d. \n",repeats);
      printf("3. Size of Matrices: %d.\n",n);
  }
  else if(argc==3){
      num_threads = atoi(argv[1]);
      repeats = atoi(argv[2]);
      printf("No argument for Matrix size has been passed. Default value is set to: \n");
      printf("3. Size of Matrices: %d.\n",n);
  }  
  else if (argc==4){
      num_threads = atoi(argv[1]);
      repeats = atoi(argv[2]);
      n = atoi(argv[3]);
  }
  else{
      printf("Too many arguments have been passed. Default values are set to: \n");
      printf("1. Number of Threads: %d.\n",num_threads);
      printf("2. Number of Repeats: %d. \n",repeats);
      printf("3. Size of Matrices: %d.\n",n);     
  }  
    
  //allocate data for matrices a,b
  int *a = (int*)malloc(sizeof(int)*n*n);
  int *b = (int*)malloc(sizeof(int)*n*n); 
  if (a == NULL || b==NULL) {
        printf("Error: Could not allocate array of size %d\n", n*n);
  }  
  //initialize matrices a and b with random numbers
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      *(a + i*n + j) = rand() % 10;
      *(b + i*n + j) = rand() % 10;
    }
  }   
    
  //calculate c multiple times and save smallest elapsed time to time file
  for(int iteration=0;iteration<repeats;iteration++){
    //allocate data for matrix
    int *c = (int*)malloc(sizeof(int)*n*n);
    if (c==NULL) {
        printf("Error: Could not allocate array of size %d\n", n*n);
    }
    omp_set_num_threads(num_threads);
    time_start = omp_get_wtime();
    # pragma omp parallel shared (a, b, c, n) private (i, j, k)
    {
      # pragma omp for
      // calculate A*B=C  
      for (i = 0; i < n; i++)
      {
        for (j = 0; j < n; j++)
        {
          *(c + i*n + j) = 0.0;
          for (k = 0; k < n; k++)
          {
            *(c + i*n + j) = *(c + i*n + j) + *(a + i*n + k) * *(a + k*n + j);
          }
        }
      }
    }
    time_end = omp_get_wtime();
    time = time_end-time_start;
    if (iteration==0) time_min = time;
    else {
        if (time<time_min) time_min = time;
    }
    //check result and free data
    error_check(n,a,b,c);  
    free(c);
  }
    
  //write smallest execution time to time file
  FILE * timefile;
  timefile = fopen ("./time.txt","w");
  fprintf (timefile, "%f",time_min);
  fclose (timefile);
    
  //output and free data
  printf("Smallest execution time of %d runs: %f \n",repeats,time_min);
  free(a);
  free(b);
    
  return 0;
}

void error_check(int n, int *a, int *b, int *c){
  int i, j;
  int *c_copy = (int*)malloc(sizeof(int)*n*n);
  if (c_copy == NULL) {
    printf("Error: Could not allocate array of size %d\n", n*n);
  }
    
  //calculate result
  for (i = 0; i < n; i++){
    for (j = 0; j < n; j++){
      *(c_copy + i*n + j) = 0.0;
      for (int k = 0; k < n; k++){
        *(c_copy + i*n + j) = *(c_copy + i*n + j) + *(a + i*n + k) * *(a + k*n + j);
      }
    }
  }
  //check if solution of parallel algorithm is correct
  for (i = 0; i < n; i++){
    for (j = 0; j < n; j++){
      if (*(c + i*n + j)!=*(c_copy + i*n + j)) 
        printf("Error: matrix matrix multiplication result is not correct");
    }
  }     
  free(c_copy);  
}

