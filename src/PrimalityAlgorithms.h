
#include <vector>
#include <cstdint>
#include <iostream>
#include <chrono>
#include <queue>
#include <thread>
#include <gmp.h>
#include "Algorithms.h"

using namespace std;
using namespace std::chrono;

#ifndef PRIMALITYALGORITHMS_H_
#define PRIMALITYALGORITHMS_H_

enum AlgorithmType {

	trial_division,
	miller_rabin,
	mpz,
	all_algorithms

};

// Split initial vector to match number of vectors to aviable cores;
template<typename T>
std::queue<std::vector<T>> split_vector(const std::vector<T>& initial_vector, size_t number_of_sub_vectors)
{
    std::queue<std::vector<T>> out_queue;

    size_t length = initial_vector.size() / number_of_sub_vectors;
    size_t remain = initial_vector.size() % number_of_sub_vectors;

    size_t begin = 0;
    size_t end = 0;

    for (size_t i = 0; i < std::min(number_of_sub_vectors, initial_vector.size()); ++i)
    {
        end += (remain > 0) ? (length + !!(remain--)) : length;

        out_queue.push(std::vector<T>(initial_vector.begin() + begin, initial_vector.begin() + end));

        begin = end;
    }

    return out_queue;
}
//Just print vector (for tests)
template<typename T>
ostream& operator<< (ostream& out, const vector<T>& v) {
    out << "{";
    size_t last = v.size() - 1;
    for (size_t i = 0; i < v.size(); ++i) {
        out << v[i];
        if (i != last)
            out << ", ";
    }
    out << "}";
    return out;
}

//type of
template <typename T>
class PrimalityAlgorithms{

public:

	PrimalityAlgorithms(const vector<T> & vect, const int cores) : initial_vector(vect), num_of_cores(cores){

		// Check if template parameter is numeric
		static_assert(std::is_arithmetic<T>::value, "Type must be numeric");

		// Creating queue of vectors  for multithreading

		// Split vector to smaller vectors queue. subvectors = num of cores;

		if(initial_vector.size()  < num_of_cores){
			// Not optimal:(
			vectors_queue = split_vector(initial_vector,initial_vector.size());

		}
		else {

			vectors_queue = split_vector(initial_vector,num_of_cores);

		}

	};

	void set_algorithm(AlgorithmType type);
	template <enum AlgorithmType e>
	void execute_algorithm();
	vector<T> get_results();

private :
	const vector<T> initial_vector;
	unsigned int num_of_cores;

	// queue of subfectors which will be processed by every thread
	queue<vector<T>> vectors_queue;

	vector<T> vect_of_primal_int;

	vector<thread> threads;

	void trial_div_algorithm(const vector<T> & vect);
	void rabin_miller_algorithm(const vector<T> & vect);
	void mpz(const vector<T> & vect);
	void run_primal_check_thread(const vector<T>  &  vect,AlgorithmType e);


};

#endif
template <typename T>
void PrimalityAlgorithms<T>::run_primal_check_thread(const vector<T>  &  vect,AlgorithmType e){

	thread find_primals_thread([&](){

		switch (e){

			 case AlgorithmType::miller_rabin :

					// cout << "Executing miller_rabin algorithm ..." <<endl;
					 rabin_miller_algorithm(vect);

					 break;
			 case AlgorithmType::trial_division:

					// cout << "Executing trial_division algorithm ..." <<endl;
					 trial_div_algorithm(vect);

					 break;
			 case AlgorithmType::mpz:

					// cout << "Executing mpz_probab_prime_p algorithm ..." <<endl;
					 mpz(vect);

					 break;


			 case AlgorithmType::all_algorithms:
					 cout << "Executing all algorithms ..." <<endl;
					 trial_div_algorithm(vect);
					 rabin_miller_algorithm(vect);
					 mpz(vect);

					 break;
		}



	});

	threads.push_back(move(find_primals_thread));

}

template <typename T>
template <enum AlgorithmType e>
void PrimalityAlgorithms<T>::execute_algorithm(){

	// Foreach sub collection run primal algorithm

	for(int i = vectors_queue.size();  i!=0;i--){
			auto vect = vectors_queue.front();

			run_primal_check_thread(vect,e);
			vectors_queue.pop();

	}
	for (unsigned int i=0; i<threads.size();i++){

					threads[i].join();

	}


}

template <typename T>
void PrimalityAlgorithms<T>::trial_div_algorithm(const vector<T> & vect)
{
	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	for(auto elem : vect){

		if(trial_div::trial_div(elem))  vect_of_primal_int.push_back(elem);

	}

	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>( t2 - t1 ).count();
	cout <<"Thread : "<< std::this_thread::get_id() << " trial_div_algorithm algorithm duration"<< duration <<endl;

}

template <typename T>
void PrimalityAlgorithms<T>::rabin_miller_algorithm(const vector<T> & vect)
{
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	int num_of_iterations = 4;

	for(auto elem : vect){


		bool isPrime = miller_r::is_prime(elem,num_of_iterations);
		if(isPrime)  vect_of_primal_int.push_back(elem);



	}
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>( t2 - t1 ).count();
	cout <<"Thread : "<< std::this_thread::get_id() <<" rabin_miller_algorithm  duration "<< duration << "ms"<<endl;
}

template <typename T>
void PrimalityAlgorithms<T>::mpz(const vector<T> & vect)
{
		high_resolution_clock::time_point t1 = high_resolution_clock::now();

		mpz_t n;
		mpz_init(n);
		for(auto elem : vect){
			mpz_set_ui(n,elem);
			if (mpz_probab_prime_p(n,4)==2)  vect_of_primal_int.push_back(elem);

		}



		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>( t2 - t1 ).count();
		cout << "mpz algorithm duration "<< duration << " ms"<<endl;
}
template <typename T>
vector<T> PrimalityAlgorithms<T>::get_results(){
	return vect_of_primal_int;
}

