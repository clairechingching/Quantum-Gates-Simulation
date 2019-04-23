/*
 * ComplexNumber.h
 *
 *  Created on: Mar 16, 2019
 *      Author: Claire
 */

#ifndef COMPLEXNUMBER_H_
#define COMPLEXNUMBER_H_
#include <iostream>


struct ComplexNumber{

	double a = 0;
	double b = 0;

	ComplexNumber& operator= (const ComplexNumber &c)
	{
		a = c.a;
		b = c.b;

	    return *this;
	}

	inline friend std::ostream & operator << (std::ostream &os, const ComplexNumber &c){
		os << c.a << " + " << c.b << "i"  << std::endl;
		return os;
	}

};

inline ComplexNumber operator +(ComplexNumber x, ComplexNumber y){

	ComplexNumber sum;

	sum.a = x.a + y.a;
	sum.b = x.b + y.b;

	return sum;
}

inline ComplexNumber operator +(double x, ComplexNumber y){

	ComplexNumber sum;

	sum.a = x + y.a;
	sum.b = y.b;

	return sum;
}

inline ComplexNumber operator *(ComplexNumber x, ComplexNumber y){

	ComplexNumber product;

	product.a = x.a * y.a - x.b * y.b;
	product.b = x.a * y.b + x.b * y.a;

	return product;
}

inline ComplexNumber operator *(double x, ComplexNumber y){

	ComplexNumber product;

	product.a = x * y.a;
	product.b = x * y.b;

	return product;
}



#endif /* COMPLEXNUMBER_H_ */
