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
# define M 1080
# define N 1920
# define REPEATS 10

int i4_min (int i1, int i2);

int main (int argc, char *argv[]) {
    
/*initialize data*/
    int num_threads=NUMTHREAD, m=M, n=N, repeats=REPEATS, count_max=2000, c, i, j, jhi, jlo, k;
    char *output_filename = "/tmp/mandelbrot_picture.txt";
    FILE *output_unit;
    double time, time_min, time_start, time_end;
    double x_max=1.25, x_min= -2.25, y_max = 1.75, y_min= - 1.75, x, x1, x2, y, y1, y2;
    
/* check for passed arguments */
    if (argc == 1) {
        printf("No arguments have been passed.\n"); 
        printf("Default value for number of threads (first argument) is: %d.\n", num_threads);
        printf("Default value for repeats (second argument) is: %d.\n", repeats);
        printf ("Default values for dimensions m and n of mandelbrot image are (third and forth argument): m=%d, n=%d.\n", m, n);
    }
    else if (argc == 2) {
        num_threads = atoi(argv[1]);
        printf("No argument for repeats and dimensions of mandelbrot image have been passed.\n"); 
        printf("Default value for repeats (second argument) is: %d.\n", repeats);
        printf ("Default values for dimensions m and n of mandelbrot image are (third and forth argument): m=%d, n=%d.\n", m, n);
    }
    else if (argc == 3) {
        num_threads = atoi(argv[1]);
        repeats = atoi(argv[2]);
        printf("No arguments for dimensions of mandelbrot image have been passed.\n"); 
        printf ("Default values for dimensions m and n of mandelbrot image are (third and forth argument): m=%d, n=%d.\n", m, n);
    }
    else if (argc ==4){
        printf("You passed only one number for dimensions m and n of mandelbrot image (third and forth argument). By default, n is set to m if no argument for n is given.\n");
        num_threads = atoi(argv[1]);
        repeats = atoi(argv[2]);
        m = atoi(argv[3]);
        n = atoi(argv[3]);
    }
    else if (argc == 5) {
        num_threads = atoi(argv[1]);
        repeats = atoi(argv[2]);
        m = atoi(argv[3]);
        n = atoi(argv[4]);
    }
    else {
        printf("Error: Too many arguments have been passed.\n");
        printf("Default value for number of threads (first argument) is: %d.\n", num_threads);
        printf("Default value for repeats (second argument) is: %d.\n", repeats);
        printf ("Default values for dimensions m and n of mandelbrot image are (third and forth argument): m=%d, n=%d.\n", m, n);
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

/* calculate mandelbrot set multiple times and save minimal execution time*/
  omp_set_num_threads(num_threads);
  for(int iteration=0;iteration<repeats;iteration++){
    time_start = omp_get_wtime();
        
    # pragma omp parallel shared ( b, count, count_max, g, r, x_max, x_min, y_max, y_min ) private ( i, j, k, x, x1, x2, y, y1, y2 )
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
    time_end = omp_get_wtime();
    time = time_end-time_start;
    if (iteration==0) time_min = time;
    else {
        if (time<time_min) time_min = time;
    }
  }
    
/* Write data to an ASCII PPM file. */
  output_unit = fopen ( output_filename, "w" );
  fprintf ( output_unit, "%d,%d,%d\n",m,n,0);
  for ( i = 0; i < m; i++ ){
    for ( jlo = 0; jlo < n; jlo = jlo + 4 ){
      jhi = i4_min ( jlo + 4, n );
      for ( j = jlo; j < jhi; j++ ){
        fprintf ( output_unit, "%d,%d,%d\n", *(r + i*n + j), *(g + i*n + j), *(b + i*n + j) );
      }
      fprintf ( output_unit, "\n" );
    }
  }
  fclose ( output_unit );

/*write smallest execution time to time file*/
  FILE *timefile;
  timefile = fopen ("/tmp/time.txt","w");
  fprintf (timefile, "%f",time_min);
  fclose (timefile);
    
/*output*/
  printf("Smallest execution time of %d runs: %f \n",repeats,time_min);
  printf ("Graphics data written to \"%s\".\n", output_filename);
    
/*free data*/
  free(count);
  free(r);
  free(g);
  free(b);
    
  return 0;
}

int i4_min ( int i1, int i2 ) {
/* returns the smaller of two integers*/
  int value;
  if ( i1 < i2 ) value = i1;
  else value = i2;
  return value;
}
