#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <time.h>

// identifier for this communication
// can be any integer defined by yourself
#define TAG_NUM 1 	

using namespace std;

int id, p;


int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	srand(time(NULL) + id * 100);  

	// if we have 24 processors, ensure processor 23 send data to 0
	// and processor 0 receive data from 23
	int source = (id + p - 1) % p;		// receive from source id
	int distination = (id + 1) % p;		// send to target id
	int data_size = id + 1;				// Add one variable for each processor
	if (id == 0) data_size = p;

	int* data = new int[data_size];		// Allocate new memory for data to transfer

	// the first processor 0
	if (id == 0) {
		data[0] = rand();			// generate a random variable

		// "data" - pointer to the data to be sent
		// "1" - number of items to be sent (processor 0 to 1)
		// "MPI_INT" - int data type in mpi
		MPI_Send(data, 1, MPI_INT, distination, TAG_NUM, MPI_COMM_WORLD);
		cout << "Processor " << 1 << " added " << data[0] << endl;

		// prcessor 23 to 0, so we have data with size "p"
		// "MPI_STATUS_IGNORE" - ignore the structure that contains information about the communication
		MPI_Recv(data, p, MPI_INT, source, TAG_NUM, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		cout << "Processor 0 received:" << endl;
		for (int i = 0; i < p; i++) {
			cout << data[i] << " ";
		}
		cout << endl;
		cout.flush();
	}
	else {
		MPI_Recv(data, data_size - 1, MPI_INT, source, TAG_NUM, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
		// Remember we need to add one more data each processor
		data[data_size - 1] = rand();
		MPI_Send(data, data_size, MPI_INT, distination, TAG_NUM, MPI_COMM_WORLD);
		cout << "Processor " << distination << " added " << data[data_size - 1] << endl;
		cout.flush();
	}
	
	// call new call delete
	delete[] data;
	MPI_Finalize();
}