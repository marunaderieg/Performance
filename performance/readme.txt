dependencies to install:
    - numpy  (conda install numpy)
    - matplotlib (conda install matplotlib)


In the folder `./programs` you can find various programs which can be used to evaluate speed-up, efficiency, scalability. Each algorithm that you can find there has two different implementation versions. 

1. The parallel algorithm without any program internal time measuring 
2. The same parallel algorithm, but measuing the execution only on the parallel algorithm itself

With the second option we don't measure the time for loading the program and runtime libraries, as well as the initialization of data, output of the program, validation of the solution, etc. All the binaries of the first option are called `notiming.o`, whereas the binaries of the second option are called `timing.o`.

Passed arguments for the `notiming.o` programms are: 1.Number of threads that should be used in the parallel regions, 2.Arguments for the algorithm (e.g list length for soriting algoirhtms). Passed arguments for the `timing.o` programs are: 1. Number of threads, 2. Number of Repeats, 3. Arguments for the algorithm. The `timing.o` versions will save the smallest measured execution time in a text file called `time.txt`. The programs also work without passing any arguments, since there are default values set for all of them. 
