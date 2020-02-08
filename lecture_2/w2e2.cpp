#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <time.h>

#define TAG_NUM 1
#define NUM_SEND 10000

using namespace std;

int id, p;

// add 10 each time for 100 times
void sumTen() {
	int sum = 0;
	for (int i = 0; i < 100; i++) sum += 10;
}

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	srand(time(NULL) + id * 10);

	MPI_Request* request = new MPI_Request[(p - 1) * 2];
	double** send_data = new double* [p]; // declare 2-D array
	double** recv_data = new double* [p];

	for (int i = 0; i < p; i++) {
		send_data[i] = new double[NUM_SEND];
		recv_data[i] = new double[NUM_SEND];

		// initialize all value
		for (int j = 0; j < NUM_SEND; j++) {
			send_data[i][j] = j / (double)NUM_SEND;
		}
	}

	int flag = 0;
	int cnt = 0;

	for (int i = 0; i < p; i++) {
		if (i != id) {
			// receive data from other processes with size 10000 each 
			MPI_Irecv(recv_data[i], NUM_SEND, MPI_DOUBLE, i, TAG_NUM, MPI_COMM_WORLD, &request[cnt]);
			cnt++;
			// send data to other process with size 10000 each
			MPI_Isend(send_data[i], NUM_SEND, MPI_DOUBLE, i, TAG_NUM, MPI_COMM_WORLD, &request[cnt]);
			cnt++;
		}
		else {
			for (int j = 0; j < NUM_SEND; j++) {
				recv_data[i][j] = send_data[i][j];
			}
		}
	}

	int work_cnt = 0;
	// "MPI_Testall" is similar as "MPI_Waitall", to see if all communications completed
	// "cnt" - numbers of processes we have
	// "request" - array of communications status
	// "flag" - according to "request", if completed, flag = true, else false 
	while (MPI_Testall(cnt, request, &flag, MPI_STATUS_IGNORE) == MPI_SUCCESS && flag == 0) {
		// when we are waiting communications completed, do some jobs we want
		sumTen();
		work_cnt++;
	}

	cout << "Process " << id << " did " << work_cnt << " cycles of work while waiting." << endl;
	cout.flush();

	delete[] request;
	delete[] recv_data;
	delete[] send_data;

	MPI_Finalize();
}
