/*
 * MatrixInverse.h
 *
 *  Created on: April 24, 2014
 *      Author: joe1
 */

#ifndef MATRIXINVERSE_H_
#define MATRIXINVERSE_H_

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/blas.hpp>
#include <boost/numeric/bindings/lapack/posv.hpp>
#include <iostream>

/* Matrix inversion routine.
 Uses lu_factorize and lu_substitute in uBLAS to invert a matrix */

using namespace boost::numeric::ublas;
template<class T>
bool inverse(const matrix<T>& input, matrix<T>& inverse)
{
	typedef permutation_matrix<std::size_t> pmatrix;

	// create a working copy of the input
	matrix<T> A(input);

	// create a permutation matrix for the LU-factorization
	pmatrix pm(A.size1());

	// perform LU-factorization

	int res = lu_factorize(A, pm);
	if (res != 0)
		return false;

	// create identity matrix of "inverse"
	inverse.assign(identity_matrix<T> (A.size1()));

	// backsubstitute to get the inverse
	lu_substitute(A, pm, inverse);

	/* LU factorization likely failed */
	return true;
}






using namespace boost::numeric::ublas;
template<class T>
void matprod(const matrix<T>& A, const matrix<T>& B, matrix<T>& C){
	matrix<T> temp;
	if((A.size2() == B.size1()) && (C.size1() == A.size1()) && (C.size2() == B.size2())){
		temp = prod(A,B);
		for(int j = 0; j < temp.size2(); j++){
			for(int i = 0; i < temp.size1(); i++){
				C(i,j) = temp(i,j);
			}
		}
	}else{
		throw new std::logic_error("Input/output matrix dimensions do not match");
	}
}

template<class T>
void matprod(const matrix<T>& A, const vector<T>& b, vector<T>& c){
	vector<T> temp;
	if((A.size2() == b.size()) && (c.size() == A.size1())){

		//temp = prod(A, b);
		for(int i = 0; i < c.size(); i++){
			c(i) = inner_prod(row(A,i), b);
		}
	}else{
		throw new std::logic_error("Input/output matrix dimensions do not match");
	}
}

template<class T>
void matprod(const vector<T>& b, const matrix<T>& A, vector<T>& c){

	if((A.size1() == b.size()) && (c.size() == A.size2())){
		c = prod(b, A);
	}else{
		throw new std::logic_error("Input/output matrix dimensions do not match");
	}
}

template<class T>
T innerprod(const vector<T>& a, const vector<T>& b){

	if(a.size() == b.size()){
		return inner_prod(a, b);
	}else{
		throw new std::logic_error("Input/output matrix dimensions do not match");
	}
}


template<class T>
void linearSolve(const matrix<T>& A, const vector<T>& b, vector<T>& c){
	matrix<double> inv_AtA(A.size2(), A.size2()), AtA(A.size2(), A.size2()), tA(A.size2(), A.size1());
	vector<double> tAb(b.size());

	if( (A.size2() == b.size()) && (b.size() == c.size())){
		matprod(tA, A, AtA);

		//identity_matrix<double> I(A.size2());
		//vector<double> sdreg(A.size1());

		//double sse = 0, sigma = 0;
		inverse(AtA, inv_AtA);
		matprod(tA, b, tAb);
		matprod(inv_AtA, tAb, c);
	}else{
		throw new std::logic_error("Input/output matrix dimensions do not match");
	}
}






#endif /* MATRIXINVERSE_H_ */
