/*------------------------------------------------------------------------------------------------
 * LOESSRegression.cpp
 *
 *  Created on: May 20, 2014
 *      Author: Joseph Egbulefu
 *
 *      Procedure for fitting Local regression. The procedure generalizes that presented in
 *      Cleveland, William S. (1979). "Robust Locally Weighted Regression and Smoothing Scatterplots"
 *      by extending to multiple predictor variables, accepting any weight function
 *      or distance function.
 *
 *
 *
 * -------------------------------------------------------------------------------------------------*/

#include "Regression.h"
#include "MatrixInverse.h"
#include "Dist.h"
#include <boost/numeric/bindings/lapack/gesv.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/traits/ublas_vector2.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <Rcpp.h>



namespace ublas = boost::numeric::ublas;
namespace lapack = boost::numeric::bindings::lapack;

using namespace std;

/*
 * The data is reordered according to a distance measure used for
 * determining which observations lie in  neighborhood
 * of a given row x_i of predictor matrix X. Reordering the
 * data improves efficiency of fitting local regression since
 * determing neighborhood of x_i does not require computing distance
 * metric for all data points.
 *
 * When series is time series (seriesType = TIME) the
 * distance function, f, is ignored, so that ordering
 * of observations is unchanged.
 *
 * @param _y		: input, response vector.
 * @param _X		: input, predictor matrix
 * @param intercept	: determines if intercept column of 1's should augment X
 * @param degree	: degree of polynomial regression
 * @param polyCol	: columns of X which d - 1 higher order polynomial variables generated and included as additional columns in X
 * @param _span		: 0 < span <= 1, determines size of neighborhood
 * @param _sType	: designates if data represents a time series (_sType = TIME) or static series (_sType = STATIC)
 * @param _weightFun: weight function for weighting data points in local least squares.
 * @praram dFun		: distance function for determinining neighborhood.
 */
LOESSRegression :: LOESSRegression(const ublas::vector<double>& _y, ublas::matrix<double>& _X, 	bool intercept,
		int degree, std::vector<int>& polyCol, double _span, SeriesType _sType, Weight<double>& _weightFun,  dist<double> dFun)
		: PolyRegression(_y, _X, intercept, degree, polyCol),
		  span(_span),
		  weight(_weightFun),
		  distFun(dFun),
		  maxDist(boost::numeric::ublas::vector<double>(nrow_x)),
		  seriesType(_sType),
		  W(boost::numeric::ublas::matrix<double>(nrow_x, _span * nrow_x)){

	if(seriesType == STATIC){ /* not time series */

		ublas::vector<double> zero(X.size2());
		ublas::vector<double> temp(X.size1());
		temp.clear();
		zero.clear();

		/* calculate distance of ith predictor */
		/* more efficient to use std::for_each along with STL fucntional/binders ?*/
		for(int i = 0; i < X.size1(); i++){
			temp(i) = distFun(row(X, i), zero);
		}

		/* add additional column for distance vector
		 * and y response vector */
		X.resize(X.size1(), ncol_x + 2, true);

		for(int i = 0; i < X.size1(); i++){
			X(i, ncol_x) = temp(i);
			X(i, ncol_x + 1) = y(i);
		}
		/* order rows of X and y according to distance column */
		order<double>(X, ncol_x, 0, X.size1() - 1, &compare<double>);

		/* reassign y so entries in y correspond to reordered
		 * entries in X  and remove distance column form X */
		y = column(X, ncol_x + 1);
		X.resize(X.size1(), ncol_x, true);
	}
}


/*
 * Fits local regression:  x_i is ith row of predictor matrix.
 * Neighborhood of x_i is the r vectors of nearest proximity to x_i, which
 * since rows of X have been ordered, are the floor(r/2) rows below
 * and ceiling(r/2) rows above ith row.
 *
 *
 * @pre				: rows of the X predictor matrix and response, y, are ordered according to proximity (see constructor).
 * @param beta		: output, std::vector of length nrow_x, with each entry as a ublas::vector of the ncol_x coefficients.
 * @param sigma		: output, standard deviation of regression errors
 * @param residual	: output vector of residuals
 *
 */
void LOESSRegression :: setTimeSeriesWeight(ublas::matrix<double>& tempX, ublas::vector<double>& tempy, int i){

	int r = (int)std::ceil(span * nrow_x);
	double tempDist = 0;
	/* find  max proximity between x_i and x_j in neighborhood */
	for(int s = 0; s < r; s++){
		if(i - s > 0){
			tempDist = distFun(ublas::row(X, i - s), ublas::row(X, i));
			ublas::row(tempX, s) = ublas::row(X, i - s);
			tempy(s) = y(i - s);
		}else{ 	/* do nothing */; }

		if(maxDist(i) < tempDist){ 	maxDist(i) = tempDist;}
	}
	/* calculate weight for each of x_i's neighbors */
	for(int s = 0; s < r; s++){
		if(i - s > 0){
			W(i, s) = weight(ublas::row(X, i - s), ublas::row(X, i), maxDist(i));
		}else{/* do nothing */;}
	}
}

void LOESSRegression :: setStaticSeriesWeight(ublas::matrix<double>& tempX,  ublas::vector<double>& tempy, int i){

	int r = (int)std::ceil(span * nrow_x);
	int r2_floor = r/2, s = 0;
	double tempDist = 0;

	/* find  max proximity between x_i and x_j in neighborhood */
	for(s = 0; s < r2_floor; s++){
		if(i - s < 0){
			tempDist = distFun(row(X, i + s), ublas::row(X, i));
			ublas::row(tempX, r2_floor + s) = ublas::row(X, i + s);
			tempy(r2_floor + s) = y(i + s);
		}else if(i + s > nrow_x){
			tempDist = distFun(row(X, i - s), ublas::row(X, i));
			ublas::row(tempX, s) = ublas::row(X, i - s);
			tempy(s) = y(i - s);
		}else{
			tempDist = std::max(distFun(ublas::row(X, i - s), ublas::row(X, i)), distFun(ublas::row(X, i + s), ublas::row(X, i)));
			ublas::row(tempX, s) = ublas::row(X, i - s);
			ublas::row(tempX, r2_floor + s) = ublas::row(X, i + s);
			tempy(s) = y(i - s);
			tempy(r2_floor + s) = y(i + s);
		}
		if(maxDist(i) < tempDist){ maxDist(i) = tempDist; }
	}

	if((r % 2 != 0) && (i + s < nrow_x)){
		ublas::row(tempX, r2_floor + s) = ublas::row(X, i + s);
		tempy(r2_floor + s) = y(i + s);
		tempDist = distFun(ublas::row(X, i + s), ublas::row(X, i));

		if(maxDist(i) < tempDist){ maxDist(i) = tempDist; }
	}
	/* calculate weight for each of x_i's neighbors */

	for(s = 0; s < r2_floor; s++){
		if(i - s < 0){
			W(i, r2_floor + s) = weight(ublas::row(X, i + s), ublas::row(X, i), maxDist(i));
		}else if(i + s > nrow_x){
			W(i, s) = weight(ublas::row(X, i - s), ublas::row(X, i), maxDist(i));
		}else{
			W(i, s) = weight(ublas::row(X, i - s), ublas::row(X, i), maxDist(i));
			W(i, r2_floor + s) = weight(ublas::row(X, i + s), ublas::row(X, i), maxDist(i));
		}
	}

	if((r % 2 != 0) && (i + s < nrow_x)){
		W(i, r2_floor + s) = weight(ublas::row(X, i + s), ublas::row(X, i), maxDist(i));
	}

}

/*
 * Fits local regression:  x_i is ith row of predictor matrix.
 * Neighborhood of x_i is the r vectors of nearest proximity to x_i, which
 * since rows of X have been ordered, are the floor(r/2) rows below
 * and ceiling(r/2) rows above ith row.
 *
 *
 * @pre				: rows of the X predictor matrix and response, y, are ordered according to proximity (see constructor).
 * @param beta		: output, std::vector of length nrow_x, with each entry as a ublas::vector of the ncol_x coefficients.
 * @param sigma		: output, standard deviation of regression errors
 * @param residual	: output vector of residuals
 *
 */
void LOESSRegression :: fit(std::vector<ublas::vector<double> >& beta, double& sigma, std::vector<double>& residual){

	int r = (int)std::ceil(span * nrow_x), ierr = 0;
	int r2_floor = r/2, s = 0;
	ublas::matrix<double> tempX(r, ncol_x);
	ublas::matrix<double,ublas::column_major> A(ncol_x, ncol_x);
	ublas::vector<double> b(ncol_x), tempy(r);
	double sse = 0,  tempDist = 0;

	A.clear();
	b.clear();
	tempX.clear();
	tempy.clear();
	maxDist.clear();

	for(int i = 0; i < nrow_x; i++){
		/* calculate regression coefficients for x_i */
		if(seriesType == STATIC)
			setStaticSeriesWeight(tempX, tempy, i);
		else if(seriesType == TIME)
			setTimeSeriesWeight(tempX, tempy, i);

		ierr = solve(tempX, tempy, A, b, ublas::row(W, i));

		if(ierr == 0){
			beta.push_back(b);
		}else if(ierr > 0){
			throw std::logic_error("Failure during computation");
		}else{
			throw std::logic_error("Failure: check inputs");
		}

		yhat(i) = ublas::inner_prod(b, ublas::row(X, i));
		residual[i] = y(i) - yhat(i);
		sse += std::pow((double) residual[i], 2.0);

		A.clear();
		b.clear();
		tempX.clear();
		tempy.clear();

	}

	sigma = std::sqrt(sse / (nrow_x - ncol_x));
}


/*
 * Solves for regression coefficients for local regression for a
 * particular x_i and corresponding y_i, using neighborhood predictor matrix
 * and response vector and solving the least squares Normal equations.
 *
 * @param X_curr: predictor matrix in neighborhood of current observation
 * @param y_curr: response vector associated with X_curr
 * @param A		: input, Linear system matrix for solving Normal equations
 * @param b		: input/output linear system result vector
 * @param w		: weights for constructing entries of A and b
 *
 * @result: output's coefficients in b,
 * 			returns error code, see Regression::solve();
 */
int LOESSRegression :: solve(const ublas::matrix<double>& X_curr,  const ublas::vector<double>& y_curr,
		  ublas::matrix<double, ublas::column_major>& A, ublas::vector<double>& b, const ublas::vector<double>& w){

	int r = (int)std::ceil(span * nrow_x), ierr = 0;

	for(int k = 0; k < r; k++){
		for(int v = 0; v < ncol_x; v++){
			for(int j = 0; j < ncol_x; j++){
				if(v == 0){
					A(v, j) += (float)(w(k) * X_curr(k, j));
				}else{
					A(v, j) += (float)(w(k) * X_curr(k, v) * X_curr(k, j));
				}
			}
			if(v == 0)
				b(v) += (float)(w(k) * y_curr(k));
			else
				b(v) += (float)(w(k) * y_curr(k) * X_curr(k, v));
		}
	}

	/* solve system: A temp = b, for observations i */
	ierr = Regression::solve(A, b);
	return ierr;
}



