Welcome

Before you can start using the Notebooks you need to install the following dependencies:
    - numpy  
    - matplotlib 

To do this, open a terminal window and run
    - conda install numpy matplotlib

In case you never worked with Jupyter Notebook, here the most important in a nutshel: 
- to execute a cell with code, you need to first click on it, then type Shift+Enter
- once the cell has executed, you will see the output of the cell and a number on the top left corner of the cell
- while the cell is computing, you will see a star at the top left corner of the cell
- sometimes things go wrong. If the cell just doesn't finnish computing, you might want to restart the kernel. You can do this from within the Jupyter notebook GUI, there are some options for that (check on google if you don't find it)
- if you restart the kernel, you might have to rerun some of the previous cells in order for the code to work (for example you probably want to import numpy library again)


If you just want to look at the main notebook called 'performance' you are good to go. You can ignore the rest of this text. In case you wish to play around with code or even test your own programs, you want to keep reading.

There are two different scenarios. 
1. You run all the notebooks on the server that your University has given you access to (e.g. via hopper if you study at FernUniversity Hagen)
2. You run the code on your local machine

The Notebooks are designed for the first scenario. That means I assumed that you are working on a machine that can use up to 64 threads. This does not mean you can't run the code on your local machine. You definitely can, but: you either have to ignore all data points for threads higher than the thread number your machine supports or you adjust the code so it doesn't make any measurements for higher thread numbers. Remember, in openMP you can request as many threads as you wish, but will not be given more threads than your machine supports. Another thing to consider if you work on a local machine is that you might have to recompile the programs located in the './programs' folder. 

No matter whether you work from your local machine or on a server (hopper), you want to place the main folder called 'performance' into your home folder.

In the folder `./programs` you can find various programs which can be used to evaluate speed-up, efficiency and scalability. Each algorithm that you can find there has two different implementation versions: 

1. The parallel algorithm without any program internal time measuring. 
2. The same parallel algorithm, but measuring the execution time from within the program (only on the parallel algorithm itself, not the entire program run).

All the binaries of the first option are called `notiming.o`,
All the binaries of the second option are called `timing.o`.

Passed arguments for the `notiming.o` programs are: 1.Number of threads that should be used in the parallel regions, 2. Problem size (e.g list length for a sorting algorithms). 
Passed arguments for the `timing.o` programs are: 1. Number of threads, 2. Number of Repeats, 3. Problem size. The `timing.o` versions will save the best measured execution time to a text file called `time.txt`. The programs also work without passing any arguments, since there are default values set for all of them. 

When would you want to use which version? 
If you plan to measure the execution time of the entire program, then you want to use the 'notiming.o' version of a program. 
If you plan to measure the execution time only on the algorithm itself, then you want to use the 'timing.o' version of a program. 

How to measure execution time of a 'notiming.o' binary from within a Jupyer Notebook?
You can use the magic command %timeit for this. A time measurement with 10 repeats, 64 threads and problem size 108 could look like this:
timeit_object = %timeit -o -n 10 -r 1 -q ! ./notiming.o 64 108  >/dev/null
To access the measured time you have to run:
time = timeit_object.best 

How to measure execution time of a 'timing.o' binary from within a Jupyer Notebook? Here the repeats are happening from within the program itself. The program will save the execution time to a file called time.txt. A time measurement with 10 repeats, 64 threads and problem size 108 looks like this:
! ./timing.o 64 10 108 >/dev/null
To import the time that has been saved to the time.txt file into the Jupyter Notebook you can do something like: 
time = np.genfromtxt('./time.txt')  -->remember to also run: import numpy as np (importing the numpy library)

How can you use the Notebooks in the 'extra' folder for different programs?
At the beginning of each Notebook in the 'extra' folder you navigate to the folder, that contains the program you want to analyze. All programs have the exact same name (timing.o or notiming.o), but they are all located in their own folder. The code is set up in a way, that you don't have to make any changes to the code if you want to look at a different program. All you have to do, is change to the directory where the program is located at. And the code will still work, without you having to change anything. In case you don't have the main folder of this collection of notebooks called 'performance' saved in the home folder you might have to adjust the first code line so that you navigate to the right folder. 

How can you analyze your own programs?
1. In the 'programs' folder create a new folder and give it the name of your program.
2. Place a binary called 'timing.o' or 'notiming.o' or both into that folder. 
3. Go to the 'extra' folder, open the notebook that interests you and navigate to the folder of your program from within the notebook (code for this is already written, you just have to replace the name of the current program with your own program)

How do you have to adjust your own programs, so that the code will work?
It will be easier for you to adjust your code into a 'notiming.o' version. In order to do this, you have to write your code so that it accepts two arguments: 1.thread number, 2. problem size. Problem size could be the length of a list that you want to sort, or the size of a picture,... Make sure to set the amount of threads that the user has requested from within the program. In case you use openMP and the user of the program has passed 64 as a first argument (the user wants 64 threads), then you need "omp_set_num_threads(64)" somewhere in your code (before the parallel region starts). You don't need to measure any wall time withing the 'notiming' program versions, as this will be done externally from within the jupyter notebook.

If you want to have a timing.o version of your program, you have to implement the time measuring inside the program. Your code needs to accept 3 arguments: 1.thread number, 2.repeats, 3.problem size. Further you have to export the result of the timing measurements to a file located in the same folder called 'time.txt'. In the programs you can find in the 'programs' folder the time is measured multiple times, and only the best out of all measurements is written to the 'time.txt' file. Make sure to write only one single 'time result' to the 'time.txt' file, and replace it every time you rerun the code. In case your program is written in c or c++, you can have a look at the programs in the 'programs' folder if you need inspiration about how to do this. 

What programming language does your code need to be written in? 
Whatever you want! This is the beauty of these notebooks...you can use c, c++, or any other programming language as long as you follow the requirements I have just described. Same applies to your preferred language for parallel programming. 

Should you have a question a find a bug, please write an email to maruna@gmx.ch.