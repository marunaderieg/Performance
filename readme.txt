Welcome. 

You will read about:
- How to work with Jupyter Notebook
- Test the performance of the benchmarks in the 'programs' folder
- How can you analyze your own programs?
- Using the Notebook 'Performance' on your local machine
- Support

_________________________________________________________

How to work with Jupyter Notebook

Here the most important in a nutshell: 
- to execute a cell with code, you need to first click on it, then type Shift+Enter
- once the cell has executed, you will see the output of the cell and a number on the top left corner of the cell
- while the cell is computing, you will see a star at the top left corner of the cell
- sometimes things go wrong. If the cell just doesn't finish computing, you might want to restart the kernel. You can do this from within the Jupyter notebook GUI; just click on the circle arrow symbol on the top center. Alternatively you can also click on 'Python 3' on the top right corner and select Python 3.
- if you restart the kernel, you might have to rerun some of the previous cells in order for the code to work

If you just want to look at the main notebook called 'performance' you are good to go. Don't read the rest of this file.
If you are the type of student who likes to investigate things an a deeper level then you want to keep reading... 

_________________________________________________________


How to test the performance of the benchmarks in the 'programs' folder?

In the folder `./programs` you can find various programs which can be used to evaluate speed-up, efficiency and scalability. Each benchmark that you can find there has two different implementation versions: 

1. The parallelized program without any program internal time measuring. 
2. The same parallelized program, but measuring the execution time from within the program. In this version the execution time is measured only on the parallel algorithm itself, not the entire program run.

All the binaries of the first option are called `notiming.o`, 
All the binaries of the second option are called `timing.o`.

Passed arguments for the `notiming.o` programs are: 1.Number of threads that should be used in the parallel regions, 2. Problem size (e.g list length for a sorting algorithms). 

Passed arguments for the `timing.o` programs are: 1. Number of threads, 2. Number of repeats (how many times the execution time should be measured), 3. Problem size. The `timing.o` versions will save the best measured execution time to a text file called `time.txt`, located in the `/tmp` folder. 

Default Values: The programs also work without passing any arguments, since there are default values set for all of them. The default value for number of threads is 64, and the default for repeats is 10. You can check in the Notebook extras/weak_scaling.ipynb what the default values of the problem size is set to. 

When would you want to use which version (timing.o or notiming.o)?
- If you plan to measure the execution time of the entire program, then you want to use the 'notiming.o' version of a program. 
- If you plan to measure the execution time only on the algorithm itself, then you want to use the 'timing.o' version of a program. 

How to measure execution time of a 'notiming.o' executable from within a Jupyter Notebook?
You can use the magic command %timeit for this. A time measurement with 10 repeats, 64 threads and problem size 108 could look like this:
timeit_object = %timeit -o -n 10 -r 1 -q ! ./notiming.o 64 108  >/dev/null
To access the measured time you can run: time = timeit_object.best 

How to measure execution time of a 'timing.o' executable from within a Jupyter Notebook?
Here the repeated time measurements are made within the program itself. The program will save the execution time to a file called `/tmp/time.txt`. A time measurement with 10 repeats, 64 threads and problem size 108 looks like this:
! ./timing.o 64 10 108 >/dev/null
To import the time from the `/tmp/time.txt` file into the Jupyter Notebook you can do something like: 
time = np.genfromtxt('/tmp/time.txt') 

How can you use the Notebooks in the 'extra' folder to test different programs?

At the beginning of each Notebook you see a code line that navigates to the folder which contains the program you want to analyze. All executables have the exact same name (timing.o or notiming.o), but they are all located in their own folder. The code is set up in a way, that you don't have to make any changes to the code if you want to look at a different program. All you have to do, is navigate to the directory where the program is located at. The code will still work, without you having to change anything. 

_________________________________________________________


How can you analyze your own programs? 

If you have written a parallel program and you want to test it for performance, you can use the code I have already written. All you have to do is:
1. Adjust your program, so that it uses the same passed values as all the other programs in the `./programs` folder.
2. Place a binary called 'timing.o' or 'notiming.o' (or both) into your `work` folder.
3. Go to the 'extra' folder and open the notebook that interests you.
4. In the first code line of the Notebook, change the path to "/home/jovyan/work/timing.o" or "/home/jovyan/work/notiming.o". Note, that you need to use the absolute path name.

How do you have to adjust your own programs, so that the code will work?

notiming.o version:
It will be easier for you to adjust your code into a 'notiming.o' version. In order to do this, you have to write your program so that it accepts 2 arguments: 1.thread number, 2. problem size. Problem size could be the length of a list that you want to sort, or the size of a picture. Remember to set the amount of threads that the user has requested from within the program. In case you use openMP and the user of the program has passed 64 as a first argument (the user wants 64 threads), then you need "omp_set_num_threads(64)" somewhere in your code (before the parallel region starts). Also set the default values for thread number and problem size, so that the program can be called without passing any arguments. You don't need to measure any wall time withing the 'notiming' program versions, as this will be done externally from within the Jupyter notebook.

timing.o version:
If you want to have a timing.o version of your program, you have to implement the time measuring inside the program. Your program needs to accept 3 arguments: 1.thread number, 2.repeats, 3.problem size. Remember to also set default values for all 3 arguments. Further you have to export the smallest measured execution time to a text file called 'time.txt', located at `/tmp`. In the benchmarks that you can find in the 'programs' folder the time is measured multiple times, and only the best out of all measurements is written to the 'time.txt' file. Make sure to write only one single time measurement to the 'time.txt' file, and replace it every time you rerun the code. In case your program is written in c or c++, you can write the smallest execution time `time_min` to the `/tmp/time.txt` like this:
    FILE * timefile;
    timefile = fopen ("/tmp/time.txt","w");
    fprintf (timefile, "%f",time_min);
    fclose (timefile);

What programming language does your code need to be written in?

Whatever you want! 
You can use c, c++, or any other programming language as long as you follow the requirements I have just described. Same applies to your preferred language for parallel programming. 

_________________________________________________________

Using the Notebook 'Performance' on your local machine: 

There are two different scenarios. 
1. You run all the notebooks on the server that your University has given you access to (e.g. via hopper if you study at FernUniversity Hagen).
2. You run the code on your local machine.

The Notebooks are designed for the first scenario. It will be easiest for you, if you run it on the server of your University. 
When I wrote the Notebooks I assumed that you are working on a machine that can use up to 64 threads. Further I assumed that you work on a system based on Unix (Mac, Linux, WSL or WSL2). This does not mean you can't run the code on your local machine. You definitely can, but you might have to do some adjustments:

1. Place the main folder `performance` into your home folder.
2. If you don't have numpy and matplotlib installed, you need to install these dependencies (run `conda install numpy matplotlib` in the terminal)
3. If your local machine does not support at least 64 threads, you either have to ignore all data points for threads higher than the thread number your machine supports or you adjust the code so it doesn't make any measurements for higher thread numbers. Remember, in openMP you can request as many threads as you wish, but will not be given more threads than your machine supports. 
4. Depending on your system you might have to recompile the programs in the `programs` folder. Remember to compile them with the -fopenmp option. If you work on a unix based system, you can recompile all the programs with the `make file`. Just open the terminal, navigate to the `performance/programs` folder and run `$ make clean && make all`. If you are unlucky you need to do some adjustments on your system first before you can compile openmp programs on your system. 
5. If you work on Windows and don't use WSL or WSL2 then you have to adjust the path names so that they work with your system. In case you don't have a /tmp folder you need to use another folder instead. 