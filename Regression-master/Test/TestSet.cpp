/*
 * TestSet.cpp
 *
 *  Created on: Apr 29, 2014
 *      Author: Joseph Egbulefu
 */


#include <iostream>
#include <vector>
#include <Rcpp.h>
#include "Rinternals.h"
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/blas.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/traits/ublas_vector2.hpp>

//#include "TestSet.h"
#include "../Weight.h"
#include "../Dist.h"
#include "../Regression.h"


using namespace std;
namespace ublas = boost::numeric::ublas;



void testLinearRegression(ublas::vector<double>& y, ublas::matrix<double>& X, bool& intercept,
		std::vector<ublas::vector<double> >& beta, double& sigma, std::vector<double>& residual){

		LinearRegression linear(y,  X, intercept);
		linear.fit(beta, sigma, residual);
}

void testPolyRegression(ublas::vector<double>& y, ublas::matrix<double>& X,
		bool& intercept, int degree, std::vector<int>& polyCol,
		std::vector<ublas::vector<double> >& beta, double& sigma, std::vector<double>& residual){

		PolyRegression poly(y, X, intercept, degree, polyCol);
		poly.fit(beta, sigma, residual);
}

void testLOESSRegression(ublas::vector<double>& y, ublas::matrix<double>& X, bool intercept,
		int degree, std::vector<int>& polyCol, double span, SeriesType sType, Weight<double>& weight,
		dist<double> distFun, std::vector<ublas::vector<double> >& beta, double& sigma, std::vector<double>& residual){
	LOESSRegression loess(y, X, intercept, degree, polyCol, span, sType, weight, distFun);
	loess.fit(beta, sigma, residual);
}

void testRLOESSRegression(const ublas::vector<double>& y, ublas::matrix<double>& X, bool intercept,
	int degree, std::vector<int>& polyCol, double span, SeriesType sType,
	Weight<double>& locWeight, dist<double> distFun, Weight<double>& robWeight, int numRobustItr,
	std::vector<ublas::vector<double> >& beta, double& sigma, std::vector<double>& residual){

	RLOESSRegression rloess(y, X, intercept, degree , polyCol,  span, sType, numRobustItr, locWeight, robWeight, distFun);

	rloess.fit(beta, sigma, residual);
}

RcppExport::SEXP testRegression(SEXP _x, SEXP _y, SEXP _intercept, SEXP _regType, SEXP _degree, SEXP _polyCol,
		 SEXP _seriesType, SEXP _span, SEXP _locWeightStr, SEXP _distFunStr, SEXP _robustWeightStr, SEXP _numRobustItr){

	Rcpp::NumericMatrix xx(_x);
	Rcpp::NumericVector yy(_y);

	bool intercept = Rcpp::as<bool>(_intercept);
	std::string regType = Rcpp::as<std::string>(_regType);

	int degree = Rcpp::as<int>(_degree);
	int numRobustItr;
	double span = Rcpp::as<double>(_span);


	SeriesType sType;
	dist<double> distFun;
	Weight<double> wt(span);
	Weight<double>& weight = wt, rWeight = wt;
	std::vector<int>polyCol;

	if(!Rf_isNull(_polyCol)){
		Rcpp::NumericVector polycol(_polyCol);
		polyCol.resize(polycol.size());
		for(int i = 0; i < polyCol.size(); i++){ polyCol[i] = polycol[i]; }
	}

	if(!Rf_isNull(_seriesType)){
		std::string seriestype = Rcpp::as<std::string>(_seriesType);
		if(seriestype == "STATIC"){ sType = STATIC; }
		else if(seriestype == "TIME"){ sType = TIME; }
	}

	if(!Rf_isNull(_distFunStr)){
		std::string distFunStr = Rcpp::as<std::string>(_distFunStr);
		if(distFunStr == "Euclidean"){
			distFun = euclid_dist<double>;
			weight =  Weight<double>(span, distFun);
		}
	}

	if(!Rf_isNull(_locWeightStr)){
		std::string locWeightStr = Rcpp::as<std::string>(_locWeightStr);
		if(locWeightStr == "Tricube"){
			weight = Tricube<double>(span, distFun);
		}else if(locWeightStr == "Bisquare"){
			weight = Bisquare<double>(span, distFun);
		}
	}

	if(!Rf_isNull(_robustWeightStr)){
		std::string robustWeightStr = Rcpp::as<std::string>(_robustWeightStr);
		if(robustWeightStr == "Tricube"){
			rWeight = Tricube<double>(span, distFun);
		}else if(robustWeightStr == "Bisquare"){
			rWeight = Bisquare<double>(span, distFun);
		}
	}

	if(!Rf_isNull(_numRobustItr)){
		numRobustItr = Rcpp::as<int>(_numRobustItr);
	}

	ublas::matrix<double> X(xx.nrow(), xx.ncol());
	ublas::vector<double> y(yy.size());
	double sigma;
	std::vector<double> residual(yy.size());
	std::vector<ublas::vector<double> > beta(0);


	for(int i=0; i < X.size1(); i++){
		y(i) = yy(i);
		for(int j = 0; j < X.size2(); j++){
			X(i, j) = xx(i, j);
		}
	}

	if(regType == "Linear"){
		testLinearRegression(y, X, intercept, beta, sigma, residual);
	}else if(regType == "Poly"){
		testPolyRegression(y, X, intercept, degree, polyCol, beta, sigma, residual);
	}else if(regType == "LOESS"){
		testLOESSRegression(y, X, intercept, degree, polyCol, span, sType, 	weight, distFun, beta, sigma,  residual);
	}else if(regType == "RLOESS"){
		testRLOESSRegression(y, X, intercept, degree, polyCol, span, sType,
				weight, distFun, rWeight, numRobustItr, beta, sigma,  residual);
	}

	Rcpp::List out;
	out["beta"] = beta;
	out["sigma"] = sigma;
	out["residual"] = residual;

	return out;
}



