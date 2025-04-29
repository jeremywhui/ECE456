To run this program, please run
make clean
make
./main n numProcesses
where the sum will be computed from 1 to n, and
the sum will be computed using numProcesses processes.
The program will then print the partial sum that
is computed by each child process, and only after all
children have terminated will it print the final sum
by combining all the results of the children processes.
Observe that by using a pipe, the children processes
will immediately write to the write end when it finishes
computing the requested result, and then the parent
process will read numProcesses integers from the read
end (in order to retrieve and combine all the results
computed by the processes). Because the addition of
integers is commutative, the order in which the children
processes write their results to the pipe will not matter,
and at the end the correct result will be computed by
adding all results together, regardless of the order in which 
they entered the pipe, thus avoiding race conditions.