Assignment 1 - Warm Up Programming Project
ECE 456 - Network-Centric Programming

Author: Jeremy Hui (jwh139)

This program takes in a file name and one or more substrings,
and counts the number of occurences of each substring in the file.
This is implemented using either the standard I/O C library,
or with system calls by using the optional --systemcalls option.

Please note the below usage for the program:
Usage: ./p01 [--help | --systemcalls] filename arg1 [arg2 ...]
    -h --help            Display this usage information.
    -s --systemcalls     Switch to the system calls implementation.

In particular, please observe that --systemcalls option may be used
anywhere, and the positions of the option does not matter. The
command line arguments will be automatically read.

To run the project, please go to the `jwh139-p01/` directory
run `make clean` and `make`. Then follow the usage as shown
above. Please also view the results in runtime.txt.

NOTE: The runtime of the systemcalls implementation was also tested
when the buffer was set to 1 byte, rather than having the buffer
dynamically allocated to the size of the file. Note that this is the
runtime when the buffer was set to 1 byte:
10485760
10485759

real    0m10.142s
user    0m1.441s
sys     0m8.700s

While this is the runtime when the buffer was set to the entire
file size:
10485760
10485759

real    0m2.435s
user    0m0.270s
sys     0m2.163s

From these results, it is apparent that the runtime is much faster
when using a larger buffer, because this allows for less direct 
reads from the file located in secondary storage, and instead
dynamic memory can be used, where reading from this memory requires
less time.