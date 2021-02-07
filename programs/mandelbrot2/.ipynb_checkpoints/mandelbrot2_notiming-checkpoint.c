#include <omp.h>
#pragma cling load("libomp.so")
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define MAX_ITER 8000
#define X_MIN -2.5
#define X_MAX 1.5
#define PI 3.1415927
#define N 1000
#define M 1000
#define NUMTHREADS 64

int main (int argc, char *argv[]) {
    
    //initalize data
    int num_threads = NUMTHREADS;
    int m = M;
    int n = N;
    double rx, ry, zx, zy, zx2, zy2;
    int x, y, i, j, iteration;
    unsigned char *r;
    unsigned char *g;
    unsigned char *b;
    char *output_filename = "picture.txt";
    FILE *output_unit;
    const double
        as = -(m/(double)n) *(X_MAX - X_MIN) /2,
        y_min = -as,
        y_max = as,
        pw = (X_MAX - X_MIN) / n,
        ph = (y_max - y_min) / m;

    // check for passed arguments
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
    
    //allocate data
    r = malloc(n*m*sizeof(char));
    g = malloc(n*m*sizeof(char));
    b = malloc(n*m*sizeof(char));
    
    //calculate mandelbrot set
    #pragma omp parallel for shared(r,g,b) private(x, y, ry, rx, zx, zy, zx2, zy2, i)
    for(j = 0; j < m * n; j++) {
        x = j % n;
        y = j / n;
        ry = y_min + y * ph;
        rx = X_MIN + x * pw;
        zx = 0.0;
        zy = 0.0;
        zx2 = 0.0;
        zy2 = 0.0;
        for(i = 0; i < MAX_ITER && ((zx2 + zy2) < 4); i++) {
            zy = 2 * zx * zy + ry;
            zx = zx2 - zy2 + rx;
            zx2 = zx * zx; zy2 = zy * zy;
        }
        //r[j] = 255 - (cos(i * PI /(double)MAX_ITER) + 1)/2 * 255;
        //g[j] = 255 - (sin(i * PI /(double)MAX_ITER) + 1)/3 * 255;
        //b[j] = 255 - (i/(double)MAX_ITER) * 255;
        *(r + j) = 255 - (cos(i * PI /(double)MAX_ITER) + 1)/2 * 255;
        *(g + j) = 255 - (sin(i * PI /(double)MAX_ITER) + 1)/3 * 255;
        *(b + j) = 255 - (i/(double)MAX_ITER) * 255;
    }

    //free data
    free(r);
    free(g);
    free(b);
    
  return 0;
}