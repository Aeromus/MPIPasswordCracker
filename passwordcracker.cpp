#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <vector>
#include "md5.h"

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
	bool passwordFound = false;
    int hashLength = -1;

	//Get settings in rank 0 then distribute settings to other ranks
	if(rank == 0){
		//Settings 
		// -Known password length
		// -Provide password hash
		// For now, assume the password has upper case, lower case, and numbers

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
        hashLength = hash.size();

		//std::cout<< "The password hash is " << hash <<std::endl;
		//std::cout<< "The lenght of the password is " << passwordLength << std::endl;

		//Example of how to do use the MD5 function. Its tested and gives correct results
		std::cout << "md5 test: " << md5("test123") << std::endl;
	}

    // make sure every process has the length of the hash string
    MPI_Bcast(&hashLength, 1, MPI_INT, 0, MCW);

    // send/recv the hash string as a char array
    if(rank == 0){
        for(int i = 0; i < size; i++){
            MPI_Send(hash.c_str(), hash.size(), MPI_CHAR, i, 0, MCW);
        }
    }else {
        char hashCharArr[hashLength];
        MPI_Recv(hashCharArr, hashLength, MPI_CHAR, 0, 0, MCW, MPI_STATUS_IGNORE);
        hash = hashCharArr;
    }

    // broadcast the password length
    MPI_Bcast(&passwordLength, 1, MPI_INT, 0, MCW);

    // Check if the communication worked
    std::cout << "rank " << rank << " has password length: " << passwordLength <<std::endl;
    std::cout << "rank " << rank << " has password hash: " << hash <<std::endl;


    //Calculate password space
    std::vector<int> passwordSpace;
    for(int i = 65; i < 91; i++){ passwordSpace.push_back(i);} // ascii values A-Z
    for(int i = 97; i < 123; i++){ passwordSpace.push_back(i);} // ascii values a-z
    for(int i = 48; i < 58; i++){ passwordSpace.push_back(i);} // ascii values 0-9
    int start_index = (passwordSpace.size() / size) * rank;
    int end_index = (rank == size -1) ? (passwordSpace.size()) : (start_index + (passwordSpace.size() / size));

    // Check if the password space was computed correctly
    std::cout << "rank " << rank << " has password space: " << start_index << "-" << end_index << std::endl;


	//Main Loop
	while(!passwordFound){

		int myflag;

		



		std::string newHash = "";
		//Do hashing

		//Check current generated string


		if(hash.compare(newHash) == 0){
			passwordFound = true;

			//Send the I found it signal to other processes

		}

		//iterate to next string

		//Check if password has been found by other processes
		MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MCW, &myflag, &mystatus);
		//If password found don't loop anymore
		if(myflag){
			passwordFound = true;
		}

	}	



	//Once password found report string and hash


	//Used to prevent stack smashing & Desync issues
	MPI_Finalize();

	return 0;
}

