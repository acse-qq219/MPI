#include <mpi.h>
#include <iostream>
#include <cstdlib>

#define tag_num 1

using namespace std;

int id, p;
int other_half_id, send_data;

void comm_range(int bottom, int top) {
	// if we have 5 processors from 0 - 5
	// range = 3 (2.5), mid = 3
	// note that though this isn't a strict half-divide method
	// what we need is split all processos to two parts even with different sizes
	int range = (top - bottom) / 2 + 1;		// mid range 
	int mid = bottom + range;				// mid value

	if (id < mid) {							// now we consider lower half 
		int lower_offset = id - bottom;
		for (int i = 0; i < range; i++) {
			other_half_id = (lower_offset + i) % range + mid; // other higher half processor id
			if (other_half_id <= top) {
				send_data = id;
				MPI_Send(&send_data, 1, MPI_INT, other_half_id, tag_num, MPI_COMM_WORLD);
				cout << "processor " << id << " sent " << send_data
					<< " to processor " << other_half_id << endl;
				cout.flush();

				int recv_data;
				MPI_Recv(&recv_data, 1, MPI_INT, other_half_id, tag_num, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				cout << "processor " << id << " received " << recv_data
					<< " from processor " << other_half_id << endl;
				cout.flush();
			}
		}
	}
	else {		// now we consider higher half 
		int higher_offset = id - mid;
		for (int i = 0; i < range; i++) {
			other_half_id = (higher_offset - i + range) % range + bottom;  // other lower half processor id
			int recv_data;
			MPI_Recv(&recv_data, 1, MPI_INT, other_half_id, tag_num, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			cout << "processor " << id << " received " << recv_data
				<< " from processor " << other_half_id << endl;
			cout.flush();

			send_data = id;
			MPI_Send(&send_data, 1, MPI_INT, other_half_id, tag_num, MPI_COMM_WORLD);
			cout << "processor " << id << " sent " << send_data
				<< " to processor " << other_half_id << endl;
			cout.flush();
		}
	}
}


int main(int argc, char* argv[])
{
	int comm_mid;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	comm_mid = (p - 1) / 2 + 1;
	comm_range(0, comm_mid);		// do lower half first
	comm_range(comm_mid, p - 1);	// do higher half then

	MPI_Finalize();
}