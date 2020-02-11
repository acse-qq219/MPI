#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <time.h>

using namespace std;

int id, p;

// realize bubble sort algorithm, to sort elements from minimun to maximum
void bubble_sort(int* list, int* index) {
	bool swapped; // if we have swapped two values 
	int temp;

	do
	{
		swapped = false;
		for (int i = 0; i < p - 1; i++) {
			if (list[i] > list[i + 1]) {
				// if current value is larger than next one
				// swap their value 
				temp = list[i];
				list[i] = list[i + 1];
				list[i + 1] = temp;

				// swap their index
				temp = index[i];
				index[i] = index[i + 1];
				index[i + 1] = temp;
				swapped = true;
			}
		}
	} while (swapped);
}

// sort the list of processes, record the actual location of each process in sorted list
int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	srand(time(NULL) + id * 100);

	int rand_priority = rand() % 100;
	int* recv_list = nullptr;
	int* send_list = nullptr;
	int* order_list = nullptr;
	
	if (id == 0) {
		recv_list = new int[p];
		send_list = new int[p];
		order_list = new int[p];
	}

	// gather "rand_priority" of every other processes to "recv_list" in precess 0
	MPI_Gather(&rand_priority, 1, MPI_INT, recv_list, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (id == 0) {
		for (int i = 0; i < p; i++) {
			order_list[i] = i;
		}

		// sort the array of "rand_priority"
		bubble_sort(recv_list, order_list);


		for (int i = 0; i < p; i++) {
			// we need get corresponding location to correct process
			send_list[order_list[i]] = i;  // the rank of each process
		}
	}

	int id_order;
	// deliver each element of "send_list" in process 0 to "id_order" of every other processes.
	MPI_Scatter(send_list, 1, MPI_INT, &id_order, 1, MPI_INT, 0, MPI_COMM_WORLD);

	cout << "Process " << id << " is " << id_order << " in the list ("
		<< rand_priority << " priority number)" << endl;
	cout.flush();

	MPI_Finalize();

	delete[] recv_list;
	delete[] send_list;
	delete[] order_list;
}