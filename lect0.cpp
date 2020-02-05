#include <mpi.h>	// Header for all MPI functions
#include <iostream>

using namespace std;

int id, p;

// main() needs to take arguments to pass to MPI_Init()
int main(int argc, char* argv[])	
{
	// Set up MPI communications
	MPI_Init(&argc, &argv); 

	// Read the rank number of current process
	// you may consider 'id' as the ID Number of the processor
	// e.g. if we allocate 4 processors, you can get id = 0, 1, 2, 3
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	// Read the total number of processes that have been assigned
	// e.g. if we allocate 4 processors, you can get p = 4 here
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	cout << "Processor " << id << " of " << p << endl;
	cout.flush();

	// Wait for all processes finished
	// This can be used to synchronize all processes in the communication
	MPI_Barrier(MPI_COMM_WORLD);

	// Remember that processor 0 is used as receiving and sending data
	// Therefore the communication normally starts and ends at processor 0 
	if (id == 0) cout << "Every process has got to this point now!" 
		<< endl << "Hello Word!" << endl;

	// Allow MPI to exit gracefully
	MPI_Finalize();
}