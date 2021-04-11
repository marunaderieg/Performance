CC := gcc
CFLAGS = -O3 -fopenmp  -std=c99 
LIBS = -lm
SUBDIRS = bubblesort fibonacci findprime jacobi mandelbrot matrixmultiplication mergesort pi quicksort selectionsort

.PHONY: subdirs $(SUBDIRS)
subdirs: $(SUBDIRS)

$(SUBDIRS):
	cd $@ && $(CC) $(CFLAGS) $@_notiming.c $(LIBS) -o notiming.o
	cd $@ && $(CC) $(CFLAGS) $@_timing.c $(LIBS) -o timing.o
    
fibonacci/timing_serial.o: fibonacci/fibonacci_timing_serial.c
	$(CC) $(CFLAGS) $< $(LIBS) -o $@

fibonacci/notiming_serial.o: fibonacci/fibonacci_notiming_serial.c
	$(CC) $(CFLAGS) $< $(LIBS) -o $@

quicksort/overhead.o: quicksort/quicksort_overhead.c
	$(CC) $(CFLAGS) $< $(LIBS) -o $@

all: fibonacci/timing_serial.o fibonacci/notiming_serial.o  quicksort/overhead.o $(SUBDIRS) 

.PHONY: all $(SUBDIRS) 

clean:
	rm */*.o