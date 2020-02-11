#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <vector>

using namespace std;

int id, p;

// assume we have 10000 particles
#define NUM_PARTICLE_TOTAL 10000
#define TAG_NUM 1
#define MAX_VEL 1.0  // maximum velocity
MPI_Datatype MPI_Particle;

class CParticle {
public:
	// we care position and velocity only
	// 2 for vertical and horizontal extent
	double x[2];
	double v[2];

	// staic - you can access it without having an instance of the class
	static void builMPIType();
};

void CParticle::builMPIType() {
	int block_lengths[2]; // number of continuous variable of that type
	MPI_Aint offsets[2];  // datatype contains memory address
	MPI_Aint addresses[2], add_start;
	MPI_Datatype typelist[2]; // datatype contains data type like "MPI_INT", etc.

	CParticle temp;
	typelist[0] = MPI_DOUBLE;
	block_lengths[0] = 2;

	// get the pointer to temp.x
	MPI_Get_address(temp.x, &addresses[0]); 

	typelist[1] = MPI_DOUBLE;
	block_lengths[1] = 2;
	// get the pointer to temp.v
	MPI_Get_address(temp.v, &addresses[1]);

	// get the memory location of the beginning of the object
	MPI_Get_address(&temp, &add_start);
	// for every continuous variables, store its offset address
	for (int i = 0; i < 2; i++) offsets[i] = addresses[i] - add_start;

	// create structure 
	MPI_Type_create_struct(2, block_lengths, offsets, typelist, &MPI_Particle);
	// once created, the structure much be committed before it can be used in any communications
	MPI_Type_commit(&MPI_Particle);
}

// instantiation of new structure
CParticle* full_particle_list = nullptr;
// declare a vector list to store all "partcile"
vector<CParticle> proc_particle_list;

double domain_size[2] = { 1.0, 1.0 }; // maximum vertical and horizontal extent for domain size

int proc_from_x(double x) {
	return (int)((p * x) / domain_size[0]);
}

void distribute_particles() {
	if (id == 0) {
		for (int i = 0; i < NUM_PARTICLE_TOTAL; i++) {
			// get process id according to its x value
			int send_proc = proc_from_x(full_particle_list[i].x[0]);

			// if process 0 itself, don't send, push itself to the full_list
			if (send_proc == id) proc_particle_list.push_back(full_particle_list[i]);
			// else send the "particle" to corresponding process
			else MPI_Send(&full_particle_list[i], 1, MPI_Particle, send_proc, TAG_NUM, MPI_COMM_WORLD);
		}

		// send an empty communicatin to indicate all particles have been sent
		for (int i = 1; i < p; i++) {
			MPI_Send(nullptr, 0, MPI_Particle, i, TAG_NUM, MPI_COMM_WORLD);
		}
	}

	// other processes receive null CParticle object "temp"
	else {
		MPI_Status status;
		do {
			int count;
			CParticle temp;
			MPI_Recv(&temp, 1, MPI_Particle, 0, TAG_NUM, MPI_COMM_WORLD, &status);
			MPI_Get_count(&status, MPI_Particle, &count);
			if (count == 0) break; // receive all "particle" until null one
			else proc_particle_list.push_back(temp);
		} while (true);
	}
}

double Random() {
	// +1 on RAND_MAX to ensure that the returned value is strictly less than 1
	return (double)rand() / ((double)RAND_MAX + 1.0);
}

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	srand(time(NULL) + id * 1000);

	CParticle::builMPIType();

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
	cout.flush();

	MPI_Type_free(&MPI_Particle);
	MPI_Finalize();

	delete[] full_particle_list;
}