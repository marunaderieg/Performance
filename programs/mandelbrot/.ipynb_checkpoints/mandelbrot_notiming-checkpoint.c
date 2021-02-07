/*
Copyright of the original code belongs to John Burkardt. Original Code accessed on 6.2.2021 on https://people.sc.fsu.edu/~jburkardt/c_src/mandelbrot_openmp/mandelbrot_openmp.c. Code has been edited by Maruna Derieg. 
*/

#include <omp.h>
#pragma cling load("libomp.so")
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>
# include <omp.h>
# define NUMTHREAD 64
# define M 1000
# define N 1000

int i4_min (int i1, int i2);

int main (int argc, char *argv[]) {
    
/*initialize data*/
    int num_threads=NUMTHREAD, m=M, n=N, count_max=2000, c, i, j, jhi, jlo, k;
    char *output_filename = "picture.txt";
    FILE *output_unit;
    double x_max=1.25, x_min= -2.25, y_max = 1.75, y_min= - 1.75, x, x1, x2, y, y1, y2;
    
/* check for passed arguments*/
    if (argc == 1) {
        printf("No arguments have been passed.\n"); 
        printf("Default value for number of threads (first argument) is: %d.\n", num_threads);
        printf ("Default values for dimensions m and n of mandelbrot image are (second and third argument): m=%d, n=%d.\n", m, n);
    }
    else if (argc == 2) {
        num_threads = atoi(argv[1]);
        printf("No arguments for dimensions of mandelbrot image have been passed.\n"); 
        printf ("Default values for dimensions m and n of mandelbrot image are (second and third argument): m=%d, n=%d.\n", m, n);
    }
    else if (argc ==3){
        printf("You passed only one number for dimensions m and n of mandelbrot image (second and third argument). By default, n is set to m if no argument for n is given.\n");
        num_threads = atoi(argv[1]);
        m = atoi(argv[2]);
        n = atoi(argv[2]);
    } 
    else if (argc == 4) {
        num_threads = atoi(argv[1]);
        m = atoi(argv[2]);
        n = atoi(argv[3]);
    }
    else {
        printf("Error: Too many arguments have been passed.\n");
        printf("Default value for number of threads (first argument) is: %d.\n", num_threads);
        printf ("Default values for dimensions m and n of mandelbrot image are (second and third argument): m=%d, n=%d.\n", m, n);
    }
    
/* allocate data */
    int *r = (int*)malloc(sizeof(int)*m*n);
    int *g = (int*)malloc(sizeof(int)*m*n);
    int *b = (int*)malloc(sizeof(int)*m*n);
    int *count = (int*)malloc(sizeof(int)*m*n);
    if (b == NULL || count==NULL || g==NULL || r==NULL) {
        printf("Error: Could not allocate array of size %d\n", n*m);
        return 1;
    }

/* calculate mandelbrot*/    
    # pragma omp parallel \
      shared ( b, count, count_max, g, r, x_max, x_min, y_max, y_min ) \
      private ( i, j, k, x, x1, x2, y, y1, y2 )
    {
    # pragma omp for

      for ( i = 0; i < m; i++ )
      {
        y = ( ( double ) (     i - 1 ) * y_max   
            + ( double ) ( m - i     ) * y_min ) 
            / ( double ) ( m     - 1 );

        for ( j = 0; j < n; j++ )
        {
          x = ( ( double ) (     j - 1 ) * x_max   
              + ( double ) ( n - j     ) * x_min ) 
              / ( double ) ( n     - 1 );

          *(count + i*n + j) = 0;

          x1 = x;
          y1 = y;

          for ( k = 1; k <= count_max; k++ )
          {
            x2 = x1 * x1 - y1 * y1 + x;
            y2 = 2 * x1 * y1 + y;

            if ( x2 < -2.0 || 2.0 < x2 || y2 < -2.0 || 2.0 < y2 )
            {
              *(count + i*n + j) = k;
              break;
            }
            x1 = x2;
            y1 = y2;
          }

          if ( ( *(count + i*n + j) % 2 ) == 1 )
          {
            *(r + i*n + j) = 255;
            *(g + i*n + j) = 255;
            *(b + i*n + j) = 255;
          }
          else
          {
            c = ( int ) ( 255.0 * sqrt ( sqrt ( sqrt ( 
              ( ( double ) ( *(count + i*n + j) ) / ( double ) ( count_max ) ) ) ) ) );
            *(r + i*n + j) = 3 * c / 5;
            *(g + i*n + j) = 3 * c / 5;
            *(b + i*n + j) = c;
          }
        }
      }
    }
     
  return 0;
}

int i4_min ( int i1, int i2 ) {
/* returns the smaller of two integers*/
  int value;
  if ( i1 < i2 ) value = i1;
  else value = i2;
  return value;
}
