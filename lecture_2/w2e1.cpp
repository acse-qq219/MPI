#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <time.h>

using namespace std;

int id, p;
bool* recv_list, * send_list;
int num_recv, num_send;
int send_pair[2] = {0, 0};

// we do 10 rounds this time
// so let's make each task tag different
int tag_num = 1; 

// in this function, we will create a list to communicate
void create_communication_list() {
	// find two ID number we want to communicate
	// not equal to each other, not equal to id
	while (send_pair[0] == id) {
		send_pair[0] = rand() % p;
	}
	while (send_pair[1] == id || send_pair[1] == send_pair[0]) {
		send_pair[1] = rand() % p;
	}

	// the locations of processor we want to send are true, others are false
	for (int i = 0; i < p; i++) { send_list[i] = false; recv_list[i] = false; }
	for (int i = 0; i < 2; i++) send_list[send_pair[i]] = true;

	// setup the sends and receives for the communication list
	MPI_Request* request = new MPI_Request[(p - 1) * 2];
	int cnt = 0;

	for (int i = 0; i < p; i++) {
		// other processors do recv and send operations
		if (i != id) {
			/* 1. similar like "MPI_Recv" but exit straight away without waiting for 
			   communications to complete.
			   2. "MPI_Irecv" doesn't have a "status" variable because it exits brefore 
			   communications finished.	
			   3. request - 0: recv, 1: send, 2: recv, 3: send, etc. 
			   This is to know when the non-blocking operation handled completes*/
			// tell current process which process will send information to it 
			MPI_Irecv(&recv_list[i], 1, MPI_BYTE, i, tag_num, MPI_COMM_WORLD, &request[cnt]);
			cnt++;
			// tell other processes which process will current process send to
			MPI_Isend(&send_list[i], 1, MPI_BYTE, i, tag_num, MPI_COMM_WORLD, &request[cnt]);
			cnt++;
		}
		// for current process, it won't send information to itself
		else recv_list[i] = false;
	}

	// wait a list of communications to complete
	// "cnt" - number of communications will be processing
	// "request" - array contains "cnt" request objects, one for each communication 
	MPI_Waitall(cnt, request, MPI_STATUS_IGNORE);
	tag_num++;

	num_recv = 0;
	for (int i = 0; i < p; i++) {
		if (recv_list[i]) num_recv++;
	}

	cout << "processor " << id << " will be receiving data from "
		<< num_recv << " processors" << endl;
	cout.flush();

	delete[] request;
}

void do_communication_round(int com_round) {
	int data_send[2];
	int* data_recv = new int[num_recv];
	int num_received = 0;
	int num_sent = 0;
	int cnt = 0;

	MPI_Request* request = new MPI_Request[2 + num_recv];

	// create the data to send and do communications
	for (int i = 0; i < p; i++) {
		if (recv_list[i]) {
			MPI_Irecv(&data_recv[num_received], 1, MPI_INT, i, tag_num, MPI_COMM_WORLD, &request[cnt]);
			num_received++;
			cnt++;
		}
		if (send_list[i]) {
			data_send[num_sent] = rand();
			MPI_Isend(&data_send[num_sent], 1, MPI_INT, i, tag_num, MPI_COMM_WORLD, &request[cnt]);
			num_sent++;
			cnt++;
		}
	}

	MPI_Waitall(cnt, request, MPI_STATUS_IGNORE);
	tag_num++;

	// output data can't be done until "MPI_Watiall" called
	num_received = num_sent = 0; // reset to 0 for further print
	for (int i = 0; i < p; i++) {
		if (send_list[i]) {
			cout << com_round << ": " << id << " processor sent " << data_send[num_sent]
				<< " to precessor " << i << endl;
			cout.flush();
			num_send++;
		}
		if (recv_list[i]) {
			cout << com_round << ": " << id << " processor received "
				<< data_recv[num_received] << " from processor " << i << endl;
			cout.flush();
			num_recv++;
		}
	}

	delete[] data_recv;
	delete[] request;
}

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	srand(time(NULL) + id * 10);

	recv_list = new bool[p];  // allocate memory to receive list of data will be received
	send_list = new bool[p];  // allocate memory to send list of data will be sent
	create_communication_list();

	for (int i = 0; i < 10; i++) {
		do_communication_round(i);
	}

	delete[] recv_list;
	delete[] send_list;
	MPI_Finalize();

	return 0;
}