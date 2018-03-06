//============================================================================
// Name        : primality.cpp
// Author      : Data Przemysław
// Version     :
// Copyright   : CC
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstdint>
#include <vector>
#include <fstream>
#include <sys/sysinfo.h>
#include "PrimalityAlgorithms.h"
using namespace std;

template <typename T>
vector<T> read_values_from_file(string path){
	vector<T> output_vector;
	try
	{
		T temp;
		ifstream file;

		file.open(path);
		while (file >> temp){



			output_vector.push_back(temp);


			}
			file.close();

	}
	catch(const exception & e){
		cout << "Exception during loading data" << endl;
		cout << e.what() << endl;
	}
	return output_vector;

}

int main() {

	int procNum =  get_nprocs();

	vector<uint64_t> vect = read_values_from_file<uint64_t>("./Debug/data.txt");

	PrimalityAlgorithms<uint64_t> p(vect,procNum);
	//Execute all algorithms
	p.execute_algorithm<AlgorithmType::miller_rabin>();

	cout << p.get_results() << endl;


	return 0;
}





