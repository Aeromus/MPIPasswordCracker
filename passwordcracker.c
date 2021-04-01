#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>


#define MCW MPI_COMM_WORLD


int main(int argc, char **argv) {

	int rank, size, data;

	//MPI INIT Stuff
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MCW, &size);
	MPI_Comm_rank(MCW, &rank);
	MPI_Status mystatus;
	srand(rank);

	//Get settings in rank 0 then distribute settings to other ranks

	//Calculate password space



	//Get input hash

	//Generate password space a-z A-Z 0-9

	//Loop till password found

	//Check current generated string

	//if password == string break

	//iterate to next string

	//if no next string look for work from other processes







	//Once password found report string and hash


	//Used to prevent stack smashing & Desync issues
	MPI_Finalize();

	return 0;
}

