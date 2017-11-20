/*
 * LinearRegression.cpp
 *
 *  Created on: May 9, 2014
 *      Author: Joseph Egbulefu
 */

#include "Regression.h"
#include <Rcpp.h>
#include <boost/math/distributions/students_t.hpp>
#include <boost/numeric/bindings/lapack/gesv.hpp>
#include <boost/numeric/bindings/lapack/gesvd.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/traits/ublas_vector2.hpp>



namespace ublas = boost::numeric::ublas;
namespace lapack = boost::numeric::bindings::lapack;

/*
* Constructor:
*
* @param _y:		input, response vector.
* @param _X:		input, predictor matrix
* @param intercept: determines if intercept column of 1's should augment X
*/
LinearRegression :: LinearRegression(const ublas::vector<double>& _y, ublas::matrix<double>& _X, bool intercept)
: Regression(_y, _X){

	if(intercept){
		ncol_x = _X.size2() + 1;
		//ublas::matrix<double> tempX = X;
		X =  ublas::matrix<double>(nrow_x, ncol_x);
		for(int i = 0; i < nrow_x; i++){ X(i, 0) = 1.0; }
		for(int j = 1; j < X.size2(); j++){
			for(int i = 0; i < X.size1(); i++){
				X(i, j) = _X(i, j-1);
			}
		}
	}
}


LinearRegression :: ~LinearRegression(){}


/*
 * Fits multiple linear regression
 *
 * @param beta:	output, std::vector of length nrow_x, with each entry as a ublas::vector
 * 			of the ncol_x coefficients.
 * @param sigma: output, standard deviation of regression errors
 * @param residual: output vector of residuals
 *
 */
void LinearRegression :: fit(std::vector<ublas::vector<double> >& beta, double& sigma, std::vector<double>& residual){

	/* calculate regression coefficient */
	ublas::matrix<double, column_major> XtX(ncol_x, ncol_x), tX(ncol_x, nrow_x);
	ublas::vector<double>tXy(ncol_x);
	double sse = 0;

	Rcpp::Rcout << " ncol_x: " << ncol_x << "\n";

	tX = trans(X);
	XtX = prod(tX, X);
	tXy = prod(tX, y);

	int ierr = Regression::solve(XtX, tXy);

	beta.push_back(tXy);
	/* compute regression fit and residual */
	yhat = prod(X, beta[0]);
	for(int i = 0; i < y.size(); i++){
		residual[i] = y(i) - yhat(i);
	}
	/* estimate of sigma */
	for(int i=0; i < nrow_x; i++){ sse += std::pow((double) residual[i], 2.0); }
	sigma = std::sqrt(sse / (nrow_x - ncol_x));
}

/*
 * Calculates confidence intervals for for regression coefficients
 * and confidence intervals for the regression line.
 *
 *@param sigma: standard deviation of residuals
 *@param alpha: CI level
 *@param inverse of X^tX
 *@param ciBeta
 *@param ciReg
 */
void LinearRegression::confInterval(double sigma, double alpha, ublas::matrix<double,
		ublas::column_major>& inv_XtX, vector<double>& ciBeta, matrix<double>& ciReg){
	double scalar, qt_alpha;
	bool suc;
	ublas::vector<double> s(inv_XtX.size2());
	ublas::matrix<double, column_major>  varBeta(ncol_x, ncol_x), U(ncol_x, ncol_x), V(ncol_x, ncol_x),
			VT(ncol_x, ncol_x), S(ncol_x, ncol_x), VS(ncol_x, ncol_x), sqrtVBeta(ncol_x, ncol_x);
	ublas::vector<double> Xi(ncol_x),  w(ncol_x), varBetaXi(ncol_x), stdev_reg(nrow_x), stdev_pred(nrow_x);


	lapack::gesvd('A', 'A', inv_XtX, s, U, VT, w);
	V = trans(VT);
	varBetaXi = prod(varBeta, Xi);
	qt_alpha = student_Tquantile(alpha);
	for(int i = 0; i < ncol_x; i++){
		for(int j = 0; j < ncol_x; j++){
			if(i == j){ S(j, j) = s(j); }
			else{ S(i, j) = 0;}
		}
	}

	VS = prod(V, S);
	sqrtVBeta = prod(VS, VT);

	for(int j = 0; j < ncol_x; j++){
		ciBeta(j) = qt_alpha * sqrtVBeta(j,j);
	}

	for(int i=0; i < nrow_x; i++){
		Xi = row(X, i);
		varBeta = pow(sigma, 2.0) * inv_XtX;
		varBetaXi = prod(varBeta, Xi);
		scalar = inner_prod(Xi, varBetaXi);
		ciReg(i, 0) = student_Tquantile(alpha) * sqrt(pow(sigma, 2) / nrow_x + (double)scalar);
		ciReg(i, 1) = student_Tquantile(alpha) * sqrt(pow(sigma, 2) * (1 + 1 / nrow_x) + (double)scalar);
	}
}

double LinearRegression :: student_Tquantile(double alpha){
	boost::math::students_t tdist(nrow_x - ncol_x);
	double T = quantile(complement(tdist, alpha / 2));
	return T;
}


