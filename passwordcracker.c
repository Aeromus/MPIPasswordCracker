#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#define MCW MPI_COMM_WORLD


int main(int argc, char **argv) {

	int rank, size, data;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MCW, &size);
	MPI_Comm_rank(MCW, &rank);

	MPI_Send(&rank, 1, MPI_INT, (rank+1)%size, 0, MCW);
	MPI_Recv(&data, 1, MPI_INT, MPI_ANY_SOURCE, 0, MCW, MPI_STATUS_IGNORE);
	std::cout<<"I am "<<rank<<" of "<<size<<"; got a message from "<<data<<"\n";

	MPI_Finalize();

	return 0;
}

