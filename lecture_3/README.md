# Non-blocking Point-to-Point Communication

## Exercise 1: Gather and Scatter Operations
* Have every process calculate a single random number between 0 and 100. Later on we wish to communicate based on the order of these numbers (you don’t need to implement this communication). You now need to ensure that every process knows what this order is. In other words, the process with the lowest random number should be first in the list, the one with the second lowest second etc. (if they have the same number you should order them according to the process id). 
* One tactic to do this is to gather all of the random numbers onto a single process, sort the list while keeping track of the id corresponding to each number, using any sort algorithm you choose (bubble sort is very easy to implement, while quick sort is much quicker, but a bit trickier to do. You could alternatively just rely on the sort algorithm from std, though you will need think about getting the indexes in order). Scatter the list of id order back to all the processes (i.e. send each one there number in the sorted order).

## Exercise 2: MPI_Reduce
* In simulations it is quite common for all processes to calculate a number, but then require that every process know, for instance , the smallest or largest number calculated. An example might be calculating the time step based on the element size and fluid velocity in a finite element simulation, where the maximum time step calculated on each process is different, but the actual time step used must be the smallest one calculated on all processes. 
* Have each process calculate a random “time step” and then use a combination of MPI_Reduce and MPI_Bcast to ensure that every process knows the same smallest time step.

## Exercise 3: MPI_Allreduce
Use MPI_Allreduce to combine the MPI_Reduce and MPI_Bcast in the previous example into a single operation.

## Exercise 4: MPI_Iallreduce
Modify the code from the previous exercise to use a non-blocking MPI_Iallreduce rather than the blocking MPI_Allreduce. In this example there is no real advantage to using a non-blocking collective communication, but in other situations it may be worth doing so in order to allow other calculations to be carried out while waiting for the communications to complete. 


## Key instructions used:
* MPI_Bcast(void* buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
* MPI_Scatter(void* send_data, int send_count, MPI_Datatype datatype, void* recv_data, int recv_data, MPI_Datatype datatype, int root, MPI_Comm communicator)
* MPI_Gather(void* send_data, int send_count, MPI_Datatype datatype, void* recv_data, int recv_data, MPI_Datatype datatype, int root, MPI_Comm communicator)
* MPI_Reduce(void* send_data, void* recv_data, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm communicator)
* MPI_Allreduce(void* send_data, void* recv_data, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm communicator)

## Reference: ACSE-6-Worksheet_3@Imperial College London
