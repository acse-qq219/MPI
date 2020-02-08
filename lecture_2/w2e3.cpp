#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <time.h>

#define TAG_NUM 1

using namespace std;

int id, p;
int rows, cols; // dimensions
int id_row, id_col; // for a id number, its row and column position

// calculcate the dimension of processors we have to m x n
// e.g. if we have 24, divide it to 4 x 6
// to find the maximum divisor
void findDimensions(int p, int& rows, int& cols) {
	// the maximum gap is 1 and p
	int max_gap = p;
	// so we test numbers smaller than sqrt(p) + 1
	// otherwise can be duplicate
	// e.g. 26, top here is 6, what we want is 2 x 13
	// no need to do calculation after 6
	int top = sqrt(p) + 1; 
	for (int i = 1; i <= top; i++) {
		// if p can be i evenly divided
		if (p % i == 0) {
			// then we can narrow gap
			int gap = abs(p / i - i);
			if (gap < max_gap) {
				// updata gap and our dimension
				max_gap = gap;
				rows = i;
				cols = p / i;
			}
		}
	}

	if (id == 0) {
		cout << p << " can be divided to " << rows << " x " << cols << endl;
		cout.flush();
	}
}

// according to input id, get its location (row and column index)
void idToIndex(int id, int& id_row, int& id_col) {
	id_row = id / cols;
	id_col = id % cols;
}

// get id number according to known id row index and column index
int idFromIndex(int id_row, int id_col) {
	if (id_row >= rows || id_row < 0) return -1;
	if (id_col >= cols || id_col < 0) return -1;

	return id_row * cols + id_col; // consider all ids as a 1-D array
}

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	srand(time(NULL) + id * 10);

	findDimensions(p, rows, cols);
	idToIndex(id, id_row, id_col);
	
	// we allocate memory for data including 8 ids(3x3 matrix except itself)
	// and three sub attribute: 0: id; 1: row index; 2: column index  
	int* data_send = new int[8 * 3];
	int* data_recv = new int[8 * 3];

	int cnt = 0;
	// 8 ids, 2 opertions
	MPI_Request* request = new MPI_Request[8 * 2];

	// consider all situations as 3x3 matrix regradless of corner or edge
	// if we get index out of boundary, "idFromIndex" will return -1, no operations will be taken then
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			int com_i = id_row + i;
			int com_j = id_col + j;
			int com_id = idFromIndex(com_i, com_j);

			if (com_id != id && com_id >= 0 && com_id < p) {
				data_send[cnt * 3] = id;
				data_send[cnt * 3 + 1] = id_row;
				data_send[cnt * 3 + 2] = id_col;

				// start from each 0 position (id), then send or receive 3 integers
				MPI_Isend(&data_send[cnt * 3], 3, MPI_INT, com_id, TAG_NUM, MPI_COMM_WORLD, &request[cnt * 2]);
				MPI_Irecv(&data_recv[cnt * 3], 3, MPI_INT, com_id, TAG_NUM, MPI_COMM_WORLD, &request[cnt * 2 + 1]);
				cnt++;
			}
		}
	}

	MPI_Waitall(cnt * 2, request, MPI_STATUS_IGNORE);
	
	for (int i = 0; i < cnt; i++) {
		cout << id << " from " << data_recv[i * 3] << " (" << data_recv[i * 3 + 1]
			<< ", " << data_recv[i * 3 + 2] << ")" << endl;
	}

	MPI_Finalize();

	delete[] data_send;
	delete[] data_recv;
	delete[] request;
}