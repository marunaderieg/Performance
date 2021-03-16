/*copyright to this code belongs to John Burkardt. Code has been edited by Maruna Derieg. Original code was accessed on 9.2.2021 on https://people.sc.fsu.edu/~jburkardt/c_src/mxm_openmp/mxm_openmp.c. */

# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <omp.h>
# pragma cling load("libomp.so")
# define NUMTHREADS 64
# define SIZE 500

void error_check(int n, int *a, int *b, int *c);

int main (int argc, char *argv[])
{
  //initalize data  
  int i, j, k, n = SIZE, num_threads=NUMTHREADS;

  //check for passed arguments
  if (argc==1){
      printf("No arguments have been passed. Default values are set to: \n");
      printf("1. Number of Threads: %d.\n",num_threads);
      printf("2. Size of Matrices: %d.\n",n);
  }    
  else if (argc==2){
      num_threads = atoi(argv[1]);
      printf("No argument for Matrix size has been passed. Default value is set to: %d.\n",n);
  }
  else if (argc==3){
      num_threads = atoi(argv[1]);
      n = atoi(argv[2]);
  }
  else{
      printf("Too many arguments have been passed. Default values are set to: \n");
      printf("1. Number of Threads: %d.\n",num_threads);
      printf("2. Size of Matrices: %d.\n",n);      
  }  
    
  //allocate data for matrices a,b,c
    int *a = (int*)malloc(sizeof(int)*n*n);
    int *b = (int*)malloc(sizeof(int)*n*n);
    int *c = (int*)malloc(sizeof(int)*n*n);
    if (a == NULL || b==NULL || c==NULL) {
        printf("Error: Could not allocate array of size %d\n", n*n);
    }
    
  //initialize matrices a,b,a_copy,b_copy with random numbers
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      *(a + i*n + j) = rand() % 10;
      *(b + i*n + j) = rand() % 10;
    }
  }  
    
  /*print A
  for (i = 0; i < n; i++){
    for (j = 0; j < n; j++){
      printf("%d ", *(a + i*n + j));
    }
    printf("\n");
  }   
  printf("\n"); */ 
    
  /*print B  
  for (i = 0; i < n; i++){
    for (j = 0; j < n; j++) printf("%d ", *(b + i*n + j));
    printf("\n");
  }   
  printf("\n"); */ 
    
  //start of parallel region
  omp_set_num_threads(num_threads);
# pragma omp parallel shared (a, b, c, n) private (i, j, k)
{
  //compute C = A * B.
  # pragma omp for
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
     
  /*print C
  for (i = 0; i < n; i++){
    for (j = 0; j < n; j++){
      printf("%d ", *(c + i*n + j));
    }
    printf("\n");
  } */

  //check if result is correct and free data 
  //error_check(n,a,b,c);  
  //free(a);
  //free(b);
  //free(c);
    
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

