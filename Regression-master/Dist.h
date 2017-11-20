/*
 * Dist.h
 *
 *  Created on: May 9, 2014
 *      Author: Joseph Egbulefu
 *
 *
 */

#ifndef DIST_H_
#define DIST_H_

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/blas.hpp>


//#include <boost/numeric/bindings/lapack/gesv.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/traits/ublas_vector2.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>


/*
 *  calculates eucledian diistance between points a and b;
 *
 */
using namespace boost::numeric::ublas;

template<class T>
using dist = double (*)(const boost::numeric::ublas::vector<T>&, const boost::numeric::ublas::vector<T>&);
template<class T>
using comp = bool (*)(const T &, const T&);

template<class T>
double identity(const boost::numeric::ublas::vector<T>& a, const boost::numeric::ublas::vector<T>& b){

	double ssq = 0;
	if(a.size() != b.size()){
		throw std::logic_error("size of input vectors must be equal.");
	}
	return 1;
}


template<class T>
double euclid_dist(const boost::numeric::ublas::vector<T>& a, const boost::numeric::ublas::vector<T>& b){

	double ssq = 0;
	if(a.size() == b.size()){
		for(int i = 0; i < a.size(); i++){
			ssq += std::pow((double)(a(i) - b(i)), 2);
		}
	}else{
		throw std::logic_error("size of input vectors must be equal.");
	}
	return std::sqrt(ssq);
}

template<class T>
double manhattan_dist(const boost::numeric::ublas::vector<T>& a, const boost::numeric::ublas::vector<T>& b){

	double d = 0;
	if(a.size() == b.size()){
		for(int i = 0; i < a.size(); i++){
			d += std::abs(a(i) - b(i));
		}
	}else{
		throw std::logic_error("size of input vectors must be equal.");
	}
	return d;
}

template<class T>
bool compare(const T & a, const T & b){ return (a < b); }


template<class T> void swapRow(boost::numeric::ublas::matrix<T>& data, int a, int b){
	T temp;
	for(int i = 0; i < data.size2(); i++){
		temp = data(a, i);
		data(a, i) = data(b, i);
		data(b, i) = temp;
	}
}

template<class T>
void order(boost::numeric::ublas::matrix<T>& data, int partCol, int left, int right, comp<T> compare){
	int i = left, j = right;
	T pivot = data((int)((left + right) / 2), partCol);

	while(i <= j){
		while(compare(data(i, partCol), pivot))
			i++;
		while(compare(pivot, data(j, partCol)))
			j--;
		if(i<=j){
			swapRow<T>(data, i, j);
			i++;
			j--;
		}
	}

	if(left < j)
		order<T>(data, partCol, left, j, compare);
	if(i < right)
		order<T>(data, partCol,  i, right, compare);
}

#endif /* DIST_H_ */
