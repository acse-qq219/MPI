#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <time.h>

using namespace std;

int id, p;

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	srand(time(NULL) + id * 1000);

	// generate a random timestep in each process
	double time_step = (double)rand() / RAND_MAX;
	cout << "Time step of process " << id << " is " << time_step << endl;

	double min_time_step;
	// very useful function which can do different opertions to send buffer
	// i.e. here we do "MPI_MIN", which is find the minivalue in "timestep" array
	// similar to "MPI_Gather", it gathers "time_step" from each process
	// and store minimum value to "min_time_step". 
	MPI_Reduce(&time_step, &min_time_step, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);

	// just like its function name, broadcast "min_time_step"
	MPI_Bcast(&min_time_step, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	cout << "Process " << id << " has a minimun time step of " << min_time_step << endl;

	MPI_Finalize();
}