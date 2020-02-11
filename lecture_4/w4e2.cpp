#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <vector>

using namespace std;

#define TAG_NUM 1
#define NUM_PARTICLE_TOTAL 10000
#define MAX_VEL 1.0

double domain_size[2] = { 1.0, 1.0 };

int id, p;
MPI_Datatype MPI_Particle;

class CParticle {
public:
	double x[2];
	double v[2];

	static void buildMPIType();
};

void CParticle::buildMPIType()
{
	int block_lengths[2];
	MPI_Aint offsets[2];
	MPI_Aint addresses[2], add_start;
	MPI_Datatype typelist[2];

	CParticle temp;

	typelist[0] = MPI_DOUBLE;
	block_lengths[0] = 2;
	MPI_Get_address(temp.x, &addresses[0]);

	typelist[1] = MPI_DOUBLE;
	block_lengths[1] = 2;
	MPI_Get_address(temp.v, &addresses[1]);

	MPI_Get_address(&temp, &add_start);
	for (int i = 0; i < 2; i++) offsets[i] = addresses[i] - add_start;

	MPI_Type_create_struct(2, block_lengths, offsets, typelist, &MPI_Particle);
	MPI_Type_commit(&MPI_Particle);
}

CParticle* full_particle_list = nullptr;
vector<CParticle> proc_particle_list;

double Random() {
	return (double)rand() / (RAND_MAX + 1);
}

int proc_from_x(double x) {
	return (int)((p * x) / domain_size[0]);
}

void distribute_particles() {
	if (id == 0) {
		// store "block_length", "addresses", "typelist" and "temp_type" 
		// of all other processes as a vector 
		vector<vector<int>> block_length(p - 1);
		vector<vector<MPI_Aint>> addresses(p - 1);
		vector<vector<MPI_Datatype>> typelist(p - 1);
		vector<MPI_Datatype> temp_type(p - 1); // store instantiated particles

		for (int i = 0; i < NUM_PARTICLE_TOTAL; i++) {
			int send_proc = proc_from_x(full_particle_list[i].x[0]);
			if (send_proc == id) proc_particle_list.push_back(full_particle_list[i]);
			else {
				block_length[send_proc - 1].push_back(1);
				MPI_Aint temp;
				// get the whole length of "particle" object 
				MPI_Get_address(&full_particle_list[i], &temp);
				addresses[send_proc - 1].push_back(temp);
				typelist[send_proc - 1].push_back(MPI_Particle);
			}
		}

		MPI_Request* request = new MPI_Request[p - 1];
		for (int i = 1; i < p; i++) {
			// for each process, we need to know: 
			// how many blocks we have? one block here
			// what's the content of the block? 
			MPI_Type_create_struct(block_length[i - 1].size(), &block_length[i - 1][0],
				&addresses[i - 1][0], &typelist[i - 1][0], &temp_type[i - 1]);
			MPI_Type_commit(&temp_type[i - 1]);

			// "MPI_BOTTOM" is the entry address of the type we created
			MPI_Isend(MPI_BOTTOM, 1, temp_type[i - 1], i, TAG_NUM, MPI_COMM_WORLD, &request[i - 1]);
		}

		// types should be freed once we don't need it anymore
		MPI_Waitall(p - 1, request, MPI_STATUS_IGNORE);

		for (int i = 1; i < p; i++) MPI_Type_free(&temp_type[i - 1]);
	}
	else {
		MPI_Status status;
		int count;

		// use "MPI_Probe" and "MPI_Recv" to receive data 
		MPI_Probe(0, TAG_NUM, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_Particle, &count);
		proc_particle_list.resize(count);
		MPI_Recv(proc_particle_list.data(), count, MPI_Particle, 0, TAG_NUM, MPI_COMM_WORLD, &status);
	}
}

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	srand(time(NULL) + id * 1000);

	CParticle::buildMPIType();
	
	if (id == 0) {
		full_particle_list = new CParticle[NUM_PARTICLE_TOTAL];
		for (int i = 0; i < NUM_PARTICLE_TOTAL; i++) {
			for (int j = 0; j < 2; j++) {
				full_particle_list[i].x[j] = Random() * domain_size[j];
				full_particle_list[i].v[j] = Random() * MAX_VEL;
			}
		}
	}

	distribute_particles();
	cout << "Process " << id << " received " << proc_particle_list.size() << " particles" << endl;

	MPI_Type_free(&MPI_Particle);
	MPI_Finalize();

	delete[] full_particle_list;
}

