#include <mpi.h>
#include <iostream>
#include <cstdlib>

#define tag_num 1

using namespace std;

int id, p;

int main(int argc, char* argv[]) {
	int recv_int = 0;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	// find itself
	for (int i = 0; i < p; i++) {
		if (i == id) {
			for (int j = 0; j < p; j++) {
				// send data to all other processors
				if (j != id) {
					double send_data = (double)id / (double)p;
					MPI_Send(&send_data, 1, MPI_DOUBLE, j, tag_num, MPI_COMM_WORLD);
				}
			}
		}
		else {
			double recv_data;
			// other processors receive data from i 
			MPI_Recv(&recv_data, 1, MPI_DOUBLE, i, tag_num, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			cout << id << " received " << recv_data << " from " << i << endl;
		}
	}
	MPI_Finalize();
}