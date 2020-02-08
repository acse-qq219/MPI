# Blocking Point-to-Point Communication

## Exercise 1: Ring Communication
Write a program that does a ring communication. Each process should send a new number to the next process in addition to the numbers received from the previous process. In other words, process zero should send a single number to process 1, process 1 should send two numbers to process 2 (the original one from the first process together with a new number), process 2 should then send 3 numbers to process 3 etc.. This should continue until the last process sends the entire list of numbers back to the first process. At each stage the process should give its id and the number that it is adding to the list, with process zero then outputting the entire list when it gets back to it. Note that each process should do a single send and a single receive, with an array being sent between the processes.

## Exercise 2: Ring Communication with MPI_Probe
Modify the program from exercise 1 so that each process randomly adds between 1 and 3 new numbers to the list being passed around the ring. You should probe for the size of the data to be received, allocate enough memory to store both this data and the new data to be added to the list, receive this data, append the new data to the end of the dynamically allocated array and then send this data on to the next processor. Remember to free the memory after using it.

## Exercise 3: Everyone speaking to everyone else
Write a program where every processor sends a piece of information to every other processor using MPI_Send and MPI_Recv.
*	The easiest, but slowest way to do this is to allow each process to have a turn to send data, with each other process waiting for that data (if you have 100 processes they will by waiting 99% of the time and only communicating 1% of the time on average). Implement this method.
*	It is much quicker to allow every process to be sending or receiving data at the same time. You can either achieve this with clever ordering of the sends and receives or by using probe to handle communications as they come in (though you still need to watch out for potential blocking – We will be looking at non-blocking communications later. These are a bit more complex to implement, but remove the need to order communications to avoid blocking). See if you can implement this using blocking communications. Don’t worry if it crashes when you first try, it is easy to get something wrong here!

## Key instructions used:
* MPI_Send(void* data, int count, MPI_Datatype datatype, int destination, 
int tag, MPI_Comm communicator)
* MPI_Recv(void* data, int count, MPI_Datatype datatype, int source, int tag,
MPI_Comm communicator, MPI_Status* status)
* MPI_Probe(int source, int tag, MPI_Comm comm, MPI_Status *status)
* MPI_Get_count(const MPI_Status *status, MPI_Datatype datatype, int *count)

## Reference: ACSE-6-Worksheet_1@Imperial College London
