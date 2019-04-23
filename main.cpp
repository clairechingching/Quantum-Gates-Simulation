/* Author: Claire(Yung-Ching, Fan)
 * Project: Final Project - Simulation of Applying Quantum Gates on Nth Qbit
 * Notes:
 * 	1. The quantum status is usually represented by complex number, which square indicates the probability.
 * 		To simplify, I create a 'ComplexNumber' class for complex number to only use two double number for each quantum status.
 * 	2. Use unique pointer manage the QbitSystem, since the size of QbitSystem grows exponentially, so the memory would be exhausted
 * 		if not release properly.
 * 	3. For quicker calculation of power of two, which is intensively used in the program, I create a function to get the power of two directly.
 * 	4. For more efficient matrix multiplication, I made templates for different data type, since the data type of Quantum Gate might be
 * 		complex number or double.
 * 	5. To implement concurrency, I split the loop into two halves and let each of the two threads tackle with one of them. Since each loop
 * 		is working on independent pairs of quantum status, each thread wouldn't affect each other.
 */

#include "ComplexNumber.h"
#include "PowerofTwo.h"
#include "MatrixOperation.h"
#include <iostream>
#include <memory>
#include <chrono>
#include <cmath>
#include <string>
#include <vector>
#include <thread>
#include <shared_mutex>

using namespace std;

struct QuantumStatus{
	unique_ptr<ComplexNumber> status;
};

class QbitSystem{
public:

	// Initialize n Qbits system
	QbitSystem(int n){

		QbitSystemSize = PowofTwo(n);

		// Initialize the first item to (1 + 0i)
		QuantumStatus init;
		init.status = make_unique<ComplexNumber>();
		init.status->a = 1;

		QuantumStatus q;
		q.status = make_unique<ComplexNumber>();
		System.push_back(move(init));
		for(int i = 0; i < QbitSystemSize-1; i++){
			QuantumStatus q;
			q.status = make_unique<ComplexNumber>();
			System.push_back(move(q));
		}

	}

	void Parallel_ApplyGate(string gate, int nth){

		thread t1(&QbitSystem::ApplyGate, this, gate, nth, 0, QbitSystemSize/4);
		thread t2(&QbitSystem::ApplyGate, this, gate, nth, QbitSystemSize/4, QbitSystemSize/2);

		t1.join();
		t2.join();
	}

	void ApplyGate(string G, int nth, int start, int end){

		unique_lock<shared_mutex> lock(m);

		// Matrix of Gate * Matrix of coefficient
		long int diff = PowofTwo(nth);

		Matrix<2, 2, double> Hgate = {{1/sqrt(2), 1/sqrt(2)}, {1/sqrt(2), -1/sqrt(2)}};

		for(int i = start; i < end; i++){

			ApplyGateFunction(i, diff, Hgate);

		}

	}

	template<class T>
	void ApplyGateFunction(int i, int diff, Matrix<2, 2, T> gate){

		int div = i/diff;
		int mod = i%diff;


		int x = div*diff*2 + mod;

		int y = x + diff;

		// Fetch status from the Qbit system
		ComplexNumber coef1;
		coef1.a = System[x].status->a;
		coef1.b = System[x].status->b;

		ComplexNumber coef2;
		coef2.a = System[y].status->a;
		coef2.b = System[y].status->b;
		Matrix<2, 1, ComplexNumber> orig = {{coef1, coef2}};

		// Calculate new status and throw back to the Qbit system
		Matrix<2, 1, ComplexNumber> after;
		after = gate * orig;

		coef1 = after(0, 0);
		coef2 = after(1, 0);

		System[x].status->a = coef1.a;
		System[x].status->b = coef1.b;
		System[y].status->a = coef2.a;
		System[y].status->b = coef2.b;

	}

	void print_result(){
		for(int i = 0; i < QbitSystemSize; i++){

			ComplexNumber result;
			ComplexNumber temp;

			temp.a = System[i].status->a;
			temp.b = System[i].status->b;

			result = temp*temp;

			cout << result;
		}
	}

	long int QbitSystemSize;
	vector<QuantumStatus> System;
	shared_mutex m;

};

int main(){

	int n = 0;
	int nth = 0;
	string answer = "Y";
	string gate = "H";

	cout << "Enter the number of Qbits: " << endl;
	cin >> n;

	QbitSystem S(n);

	//  While user wants to keep applying gates
	//  cout the initiate state of Qbit
		S.print_result();

		cout << "Apply H Quantum Gate?" << endl;
		cin >> answer;

	while(answer == "Y"){

		cout << "Which Nth Qbit do you want to apply on: " << endl;
		cin >> nth;

		auto start = chrono::system_clock::now();

		S.Parallel_ApplyGate(gate, nth);
		cout << chrono::duration<double>(chrono::system_clock::now() - start).count() << " seconds\n";

		S.print_result();

		cout << "Apply H Quantum Gate?" << endl;
		cin >> answer;

	}

}
