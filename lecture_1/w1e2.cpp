#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <time.h>

#define tag_num 1

using namespace std;

int id, p;

int main(int argc, char* argv[]) {
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	srand(time(NULL) + id * 100);

	int source = (id + p - 1) % p;
	int destination = (id + 1) % p;
	int num_recv, num_additional;
	int* data;

	if (id == 0) {
		num_additional = rand() % 3 + 1; // pass 1 to 3 random variables
		data = new int[num_additional];
		cout << id << " sent: ";
		for (int i = 0; i < num_additional; i++) {
			data[i] = rand();
			cout << data[i] << " ";
		}
		cout << endl;
		MPI_Send(data, num_additional, MPI_INT, destination, tag_num, MPI_COMM_WORLD);
		delete[] data;

		// allow information about a communication to be read before communication finished
		// "status" - a pointer to the structure containing the status information
		MPI_Probe(source, tag_num, MPI_COMM_WORLD, &status);

		// used with "MPI_Probe" to get number of items received
		MPI_Get_count(&status, MPI_INT, &num_recv);
		
		// final step, processor 0 get values from 23, no need to add size
		data = new int[num_recv];
		MPI_Recv(data, num_recv, MPI_INT, source, tag_num, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		cout << "Processor " << id << " received:";
		for (int i = 0; i < num_recv; i++) cout << " " << data[i];
		cout << endl;
		delete[] data;
	}
	else {
		MPI_Probe(source, tag_num, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &num_recv);
		num_additional = rand() % 3 + 1;
		// accumulation with new "num_additional"
		data = new int[num_additional + num_recv];
		MPI_Recv(data, num_recv, MPI_INT, source, tag_num, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
		cout << id << " sent:";
		for (int i = 0; i < num_additional; i++) {
			data[num_recv + i] = rand();
			cout << " " << data[num_recv + i];
		}
		cout << endl;
		MPI_Send(data, num_additional + num_recv, MPI_INT, destination, tag_num, MPI_COMM_WORLD);
		delete[] data;
	}

	MPI_Finalize();
}