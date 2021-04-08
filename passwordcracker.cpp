#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <vector>
#include "md5.h"
#include <chrono>

#define MCW MPI_COMM_WORLD
#define TERMINATE 8

MPI_Status myStatus;

std::string hash;
int startIndex;
int endIndex;
int passwordLength = 6;
int size, rank, data;
int myFlag;
auto start = std::chrono::high_resolution_clock::now();
int passwordAttemts = 0;

void generate(char* arr, int i, std::string s, int len) {
    // base case
    if (i == 0){ // when len has been reached
        // check if the password was found
        if(hash.compare(md5(s)) == 0){
            auto stop = std::chrono::high_resolution_clock::now();
            auto durationSeconds = std::chrono::duration_cast<std::chrono::seconds>(stop -start);
            auto durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);
            std::cout << "The password is: " << s << std::endl << std::endl;
            std::cout << "It took " << durationSeconds.count() << " seconds to find" << std::endl;
            std::cout << "It took " << durationMs.count() << " miliseconds to find" << std::endl;
            // tell all the other processes to terminate
            for(int i = 0; i < size; i++){
                if(i != rank){
                    MPI_Send(&data, 1, MPI_INT, i, TERMINATE, MCW);
                }
            }
            MPI_Finalize();
            exit(0);

        } else {
		if(passwordAttemts % 50000 == 0){
		    // check if a process found the password
		    MPI_Iprobe(MPI_ANY_SOURCE, TERMINATE, MCW, &myFlag, &myStatus);
		    if(myFlag){
			MPI_Finalize();
			exit(0);
		    }
		}
        }
	passwordAttemts++;
        return;
    }

    // iterate through the array
    for (int j = 0; j < len; j++) {

        // Create new string with next character
        // Call generate again until string has
        // reached its len
        std::string appended = s + arr[j];
        generate(arr, i - 1, appended, len);
    }

    return;
}

// function to generate all possible passwords
void crack(char* arr, int len) {
    for(int i = startIndex; i <= endIndex; i++){
        std::string startString = "";
        startString += static_cast<char>(arr[i]);
        generate(arr, passwordLength - 1, startString, len);
    }
}


int main(int argc, char **argv) {

	bool includeLower = false;
	bool includeUpper = false;
	bool includeNumbers = false;

	//MPI INIT Stuff
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MCW, &size);
	MPI_Comm_rank(MCW, &rank);
    int hashLength = -1;
    std::string yes = "Y";

	//Get settings in rank 0 then distribute settings to other ranks
	if(rank == 0){
        //std::cout << "md5 hash: " << md5("test") << std::endl;
		//Settings 
		// -Known password length
		// -Provide password hash
		// For now, assume the password has upper case, lower case, and numbers
		bool havePassLen = false;
		while(!havePassLen){
            std::cout<< "\nHow long is the password? " ;
            std::cin >> passwordLength;

            if(passwordLength < 1){
                std::cout << "Please input a password length of at least length 1." << std::endl;
            } else{
                havePassLen = true;
            }
		}

		bool validPWS = false;
		while(!validPWS){
            std::string lower = "";
            std::cout << "Does the password contain lower case characters? (Y/n) ";
            std::cin >> lower;
            if(yes.compare(lower) == 0){
                includeLower = true;
            }

            std::string upper = "";
            std::cout << "Does the password contain upper case characters? (Y/n) ";
            std::cin >> upper;
            if(yes.compare(upper) == 0){
                includeUpper = true;
            }

            std::string numbers = "";
            std::cout << "Does the password contain numbers? (Y/n) ";
            std::cin >> numbers;
            if(yes.compare(numbers) == 0){
                includeNumbers = true;
            }

            if(!includeLower && !includeUpper && !includeNumbers){
                std::cout << "Please enter a valid password space." << std::endl;
            } else {
                validPWS = true;
            }
		}

        //flush buffer
        std::string s;
        std::getline(std::cin, s);

		std::cout << "What is the password hash? ";
		std::getline(std::cin, hash);
		hashLength = hash.size();
	}

	// make sure every process knows the password space
    MPI_Bcast(&includeLower, 1, MPI_C_BOOL, 0, MCW);
    MPI_Bcast(&includeUpper, 1, MPI_C_BOOL, 0, MCW);
    MPI_Bcast(&includeNumbers, 1, MPI_C_BOOL, 0, MCW);

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

    //Calculate password space
    std::vector<int> passwordSpace;
    if(includeLower){
        for(int i = 97; i < 123; i++){ passwordSpace.push_back(i);} // ascii values a-z
    }
    if(includeUpper){
        for(int i = 65; i < 91; i++){ passwordSpace.push_back(i);} // ascii values A-Z
    }
    if(includeNumbers){
        for(int i = 48; i < 58; i++){ passwordSpace.push_back(i);} // ascii values 0-9
    }

    startIndex = (passwordSpace.size() / size) * rank;
    if(rank == (size - 1)){
        endIndex = passwordSpace.size() - 1;
    } else {
        endIndex = (startIndex + (passwordSpace.size() / size));
    }

    char pws[passwordSpace.size()];
    for(int i = 0; i < passwordSpace.size(); i++){
        pws[i] = static_cast<char>(passwordSpace[i]);
    }

    // find the password
    ::start = std::chrono::high_resolution_clock::now();
    crack(pws, passwordSpace.size());

    MPI_Finalize();
	return 0;
}

