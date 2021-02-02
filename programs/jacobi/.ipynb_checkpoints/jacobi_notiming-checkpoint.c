#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>
#ifndef WIN32
    #include <sys/time.h>
#endif
#pragma cling load("libomp.so")
#define U(i,j) u[(i)*n+(j)]
#define UOLD(i,j) uold[(i)*n+(j)]
#define F(i,j) f[(i)*n+(j)]
#define NUMTHREADS 64

/* 
******************************************************************
* When compiling this code on UNIX with gcc, -lm option to link sqrt function must be used (example: gcc jacobi.c -fopenmp -lm -o par.o)

* Subroutine HelmholtzJ
* Solves poisson equation on rectangular grid assuming : 
* (1) Uniform discretization in each direction, and 
* (2) Dirichlect boundary conditions 
* 
* Jacobi method is used in this routine 
*
* Input : n,m   Number of grid points in the X/Y directions 
*         dx,dy Grid spacing in the X/Y directions 
*         alpha Helmholtz eqn. coefficient 
*         omega Relaxation factor 
*         f(n,m) Right hand side function 
*         u(n,m) Dependent variable/Solution
*         tol    Tolerance for iterative solver 
*         maxit  Maximum number of iterations 
*
* Output : u(n,m) - Solution 
*****************************************************************
*/

void jacobi (
             int n, 
             int m, 
             double dx, 
             double dy, 
             double alpha, 
             double omega, 
             double *u, 
             double *f, 
             double tol, 
             int maxit)
{
  int i,j,k;
  double error, resid, ax, ay, b;
  double *uold;
  uold = (double *) malloc(sizeof(double)*n*m);
  if (!uold){
    fprintf(stderr, "Error: cant allocate memory\n");
    exit(1);
  }
  ax = 1.0/(dx * dx); /* X-direction coef */
  ay = 1.0/(dy*dy); /* Y_direction coef */
  b = -2.0/(dx*dx)-2.0/(dy*dy) - alpha; /* Central coeff */
  error = 10.0 * tol;
  k = 1;

  while (k <= maxit && error > tol) {

    error = 0.0;
    #pragma omp parallel
    {
        /* copy new solution into old */
        #pragma omp for private(i)
        for (j=0; j<m; j++)
          for (i=0; i<n; i++){
            UOLD(j,i) = U(j,i);
          }

        /* compute stencil, residual and update */
        #pragma omp for private(i, resid) reduction(+:error) // or collapse(2) instead of private(i)
        for (j=1; j<m-1; j++){
          for (i=1; i<n-1; i++){
            resid =(
                    ax * (UOLD(j,i-1) + UOLD(j,i+1))
                    + ay * (UOLD(j-1,i) + UOLD(j+1,i))
                    + b * UOLD(j,i) - F(j,i)
                    ) / b;

            /* update solution */
            U(j,i) = UOLD(j,i) - omega * resid;

            /* accumulate residual error */
            error =error + resid*resid;

          }
        }
    }
    /* error check */
    k++;
    error = sqrt(error) /(n*m);

  } /* while */
  printf("Total Number of Iterations: %d\n",k);
  printf("Residual: %f\n",error);
}

/******************************************************
* Initializes data 
* Assumes exact solution is u(x,y) = (1-x^2)*(1-y^2)
*
******************************************************/
void initialize(  
                int n,    
                int m,
                double alpha,
                double *dx,
                double *dy,
                double *u,
                double *f)
{
  int i,j,xx,yy;

  *dx = 2.0 / (n-1);
  *dy = 2.0 / (m-1);

  /* Initilize initial condition and RHS */
  #pragma omp parallel for private(i, xx, yy) // or collapse(2) instead of private(i)
      for (j=0; j<m; j++){
        for (i=0; i<n; i++){
          xx = -1.0 + *dx * (i-1);
          yy = -1.0 + *dy * (j-1);
          U(j,i) = 0.0;
          F(j,i) = -alpha * (1.0 - xx*xx) * (1.0 - yy*yy)
                    - 2.0 * (1.0 - xx*xx) - 2.0 * (1.0 - yy*yy);
        }
      }
}

/************************************************************
* Checks error between numerical and exact solution 
*
************************************************************/
void error_check(
                 int n,
                 int m,
                 double alpha,
                 double dx,
                 double dy,
                 double *u,
                 double *f)
{
  int i,j;
  double xx, yy, temp, error;

  dx = 2.0 / (n-1);
  dy = 2.0 / (m-1);
  error = 0.0;
    
  #pragma omp parallel for private(i, xx, yy, temp) reduction(+:error) // or collapse(2) instead of private(i)
  for (j=0; j<m; j++){
    for (i=0; i<n; i++){
      xx = -1.0 + dx * (i-1);
      yy = -1.0 + dy * (j-1);
      temp = U(j,i) - (1.0 - xx*xx) * (1.0 - yy*yy);
      error += temp*temp;
    }
  }

  error = sqrt(error)/(n*m);
  printf("Solution Error: %f\n",error);
}

int main(int argc, char *argv[]){
    /*initialize data*/
    int n=100, m=100, mits=50, num_threads=NUMTHREADS;
    double tol=1e-15, relax=1.0, alpha=0.8;
    double *u, *f, dx, dy, r1;
    char test;

    /*check for passed arguments*/
    if (argc == 1) {
        printf("No arguments have been passed. Defaults are set to: \n");
        printf("Argument 1: Number of Threads: %d \n",num_threads);
        printf("Argument 2 and 3: Grid dimension in x,y direction: %d, %d \n",n,m);
        printf("Argument 4: Helmholts constant (alpha): %f\n",alpha);
        printf("Argument 5: Successive over-relaxation parameter: %f\n",relax);
        printf("Argument 6: Error tolerance for iterrative solver: 1e-15\n");
        printf("Argument 7: Maximum iterations for solver: %d\n",mits);      
    }
    else if (argc==2){
        num_threads = atoi(argv[1]);
        printf("Number of Threads is set to %d\n",num_threads);
        printf("No arguments for Jacobi solver have been passed. Defaults are set to: \n");
        printf("Argument 2 and 3: Grid dimension in x,y direction: %d, %d \n",n,m);
        printf("Argument 4: Helmholts constant (alpha): %f\n",alpha);
        printf("Argument 5: Successive over-relaxation parameter: %f\n",relax);
        printf("Argument 6: Error tolerance for iterrative solver: 1e-15\n");
        printf("Argument 7: Maximum iterations for solver: %d\n",mits);   
    }
    else if (argc<8){
        num_threads=atoi(argv[1]);
        printf("Number of Threads is set to %d\n",num_threads);
        printf("An incomplete list of arguments for the Jacobi solver has been passed. Provide complete list of arguments to override default values. Defaults are set to: \n");
        printf("Argument 2 and 3: Grid dimension in x,y direction: %d, %d \n",n,m);
        printf("Argument 4: Helmholts constant (alpha): %f\n",alpha);
        printf("Argument 5: Successive over-relaxation parameter: %f\n",relax);
        printf("Argument 6: Error tolerance for iterrative solver: 1e-15\n");
        printf("Argument 7: Maximum iterations for solver: %d\n",mits);  
    }
    else if (argc ==8) {
        num_threads=atoi(argv[1]);
        n = atoi(argv[2]);
        m = atoi(argv[3]);
        alpha = atoi(argv[4]);
        relax = atoi(argv[5]);
        tol = atoi(argv[6]);
        mits = atoi(argv[7]);
    }
    else {
        printf("You passed too many arguments. Provide 0,1 or 7 arguments. Defaults are set to:\n");
        printf("Argument 1: Number of Threads: %d \n",num_threads);
        printf("Argument 2 and 3: Grid dimension in x,y direction: %d, %d \n",n,m);
        printf("Argument 4: Helmholts constant (alpha): %f\n",alpha);
        printf("Argument 5: Successive over-relaxation parameter: %f\n",relax);
        printf("Argument 6: Error tolerance for iterrative solver: 1e-15\n");
        printf("Argument 7: Maximum iterations for solver: %d\n",mits);  
    }

    /*allocate data*/
    u = (double *) malloc(n*m*sizeof(double));
    f = (double *) malloc(n*m*sizeof(double));
    if (!u || !f){
      printf("Error: Can't allocate memory\n");
      return 1;
    }

    /* set number of threads*/
    omp_set_num_threads(num_threads);
    
    /* arrays are allocated and initialzed */
    initialize(n, m, alpha, &dx, &dy, u, f);

    /* Solve Helmholtz eqiation */
    r1 = omp_get_wtime();
    jacobi(n, m, dx, dy, alpha, relax, u,f, tol, mits);
    r1 = omp_get_wtime() - r1;
    
    /* output and error check */
    printf("elapsed time: %f\n",r1);
    printf("MFlops: %f \n",mits*(m-2)*(n-2)*0.000001*13/r1);
    error_check(n, m, alpha, dx, dy, u, f); 
}