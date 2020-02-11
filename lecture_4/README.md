# Non-blocking Point-to-Point Communication

## Exercise 1: Creating an MPI type for a class 
* When carrying out Lagrangian simulations it is useful to have a class that stores the position of a point/particle (as well as all its other properties). Create a class to store a 2D position and velocity. Choose a maximum vertical and horizontal extent for your domain and then create 10 000 randomly located particles within these extents on processor zero. Divide the domain up into vertical stripes of equal width so that there can be a domain assigned to each of the processes.
* Create an MPI type that can send all the information in an object of the class’ type together. Send the particles individually from the root to the appropriate processor according to its horizontal position. This can be done using either blocking or non-blocking sends and receives. You can send an empty communication to indicate that all the particles have been sent. 


## Exercise 2: Creating a temporary MPI type
In the previous exercise doing a large number of communications to send all the particles is very inefficient. Modify the above code to do the transfer as a single communication for each process. On the zero process you need to create temporary variables for the transfer of the data to each of the processes. This is because the data for each processes will be randomly scattered within the list of data created on processor zero. On the other processes you will need to use a probe to determine how many particles they are to receive. You will not need to create temporary types on these processes as you can store them as continuous memory and you can use a blocking receive as you are only receiving a single piece of information. 

## Key instructions used:
* MPI_Get_address(const void* location, MPI_Aint* address);
* MPI_Type_create_struct(int count, const int array_of_blocklengths[], const MPI_Aint array_of_displacements[], const MPI_Datatype array_of_types[], MPI_Datatype* newtype)
* MPI_Type_commit(MPI_Datatype* datatype)

## Reference: ACSE-6-Worksheet_4@Imperial College London
