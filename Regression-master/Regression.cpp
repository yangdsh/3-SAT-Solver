//============================================================================
// Name        : Regression.cpp
// Author      : Joseph Egbulefu
// Version     :
// Copyright   : Your copyright notice
// Description : Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include "Regression.h"
#include <vector>
#include <iostream>


#include <boost/math/distributions/students_t.hpp>
#include <boost/numeric/bindings/lapack/gesv.hpp>
//#include <boost/numeric/bindings/lapack/gesvd.hpp>


#include <boost/numeric/bindings/traits/ublas_vector2.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>



namespace ublas = boost::numeric::ublas;
namespace lapack = boost::numeric::bindings::lapack;

Regression :: Regression(const ublas::vector<double>& _y,  ublas::matrix<double>& _X)
		: nrow_x(_X.size1()), ncol_x(_X.size2()),
		  X(const_cast<matrix<double>&>(_X)), y(_y), yhat(y.size()){}



int Regression:: solve(ublas::matrix<double,ublas::column_major>& A, ublas::vector<double>& b){

	int ierr = lapack::gesv(A, b);
	return ierr;
}

