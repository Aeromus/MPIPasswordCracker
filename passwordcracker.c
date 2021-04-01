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
	int passwordLength = -1;
	std::string hash = "";
	std::string yes = "Y";
	//Get settings in rank 0 then distribute settings to other ranks
	if(rank == 0){
		//Settings 
		// -Known password length
		// -Provide password hash
		// 

		std::string knownLength = "";
		std::cout << "\nIs the length of the password known? (Y/n) ";
		std::getline(std::cin, knownLength);
		if(yes.compare(knownLength) == 0){
			std::cout<< "\nHow long is the password?  " ;
			std::cin >> passwordLength;

			//flush buffer
			std::string s;
			std::getline(std::cin, s);

		}

		std::cout << "\nWhat is the password hash? ";
		std::getline(std::cin, hash);

		std::cout<< "The password hash is " << hash <<std::endl;
		std::cout<< "The lenght of the password is " << passwordLength << std::endl;
	}


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

