/*
 * MatrixOperation.h
 *
 *  Created on: Mar 17, 2019
 *      Author: Claire
 */

#ifndef MATRIXOPERATION_H_
#define MATRIXOPERATION_H_

#include <array>
#include "ComplexNumber.h"

template<int rows, int columns, class T>
class Matrix{
public:

	Matrix() : data{} {}
	std::array<std::array<T, columns>, rows> data;

	T operator()(int x, int y){
		return data[x][y];
	}
};

template<class T>
class Matrix<2, 2, T>{
public:

	Matrix() : data{} {}
	Matrix(std::initializer_list<std::initializer_list<T>> init) {
		auto dp = data.begin();
		for (auto row : init) {
			std::copy(row.begin(), row.end(), dp->begin());
			dp++;
		}
	}


	T operator()(int x, int y){
		return data[x][y];
	}

	std::array<std::array<T, 2>, 2> data;

};

template<class T>
class Matrix<2, 1, T>{
public:

	Matrix() : data{} {}
	Matrix(std::initializer_list<std::initializer_list<T>> init) {
		auto dp = data.begin();
		for (auto row : init) {
			std::copy(row.begin(), row.end(), dp->begin());
			dp++;
		}
	}

	T &operator()(int x, int y){
		return data[x][y];
	}


	T operator()(int x, int y) const {
		return data[x][y];
	}

	std::array<std::array<T, 1>, 2> data;

};

inline class Matrix<2, 1, ComplexNumber> operator*(Matrix<2, 2, double> &l, Matrix<2, 1, ComplexNumber> &r){

	Matrix<2, 1, ComplexNumber> result;
	ComplexNumber a;

	result(0, 0) = l(0, 0) * r(0, 0) + l(0, 1) * r(1, 0);
	result(1, 0) = l(1, 0) * r(0, 0) + l(1, 1) * r(1, 0);

	return result;

};

#endif /* MATRIXOPERATION_H_ */
