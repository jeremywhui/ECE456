To run level 1 and 2 of the program, please run:
make clean
make
./main n numThreads
where the sum will be computed from 1^2 to n^2, and
the sum will be computed using numThreads threads.
Each thread will wait a random amount of time between 0 and 1
seconds, then compute and print the amount the totalSum was
increased by. Only after all threads have terminated
will it print the final sum that is stored in the global
variable. Observe that the by using a semaphore, it 
guarantees that the newest value of the totalSum variable
is always used before adding to it, and because the addition 
of integers is commutative, the order in which the threads
add their results will not matter, and at the end the correct 
result will be computed by adding all results together, 
regardless of the order.

To run level 3 of the program, please run:
make clean
make
./level3 n numThreads
This will create one producer thread, and numThreads consumer
threads. The producer thread will wait a random amount of 
time between 0 and 1 seconds, insert the numbers from 1
to n into the circular buffer, as long as there is space
present in the buffer, and will update the pointers
accordingly. The consumer thread will read from the buffer, 
as long as there are values available in the buffer, will
compute the square of the value read, add it into the
totalSum global variable, and increase the number of values
processed (note that reading from the buffer, adding to
the totalSum variable, and incrementing the processed 
variable are all protected by the mutex). Observe that the
main process will only detach all of the threads once
the number processed is at least n, and extra iterations
in the consumer threads will not occur as they will all
be blocked waiting for the remainingSpace semaphore, and
therefore the correct final sum will always be reported.