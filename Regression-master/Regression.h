/*
 * LinearRegression.h
 *
 *  Created on: May 9, 2014
 *      Author: Joseph Egbulefu
 */

#ifndef LINEARREGRESSION_H_
#define LINEARREGRESSION_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include<vector>
#include <boost/numeric/ublas/blas.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/traits/ublas_vector2.hpp>

#include "Weight.h"
#include "Dist.h"

using namespace boost::numeric::ublas;

enum SeriesType { STATIC, TIME};

class Regression{
public:

	Regression(): y(vector<double>()),  yhat(vector<double>()), X(matrix<double>()), ncol_x(0), nrow_x(0){}
	Regression(const vector<double>& _y,  matrix<double>& _X);

	virtual void fit(std::vector<boost::numeric::ublas::vector<double> >&, double&, std::vector<double>&)=0;

	int solve(boost::numeric::ublas::matrix<double, column_major>&, boost::numeric::ublas::vector<double>&);


	virtual ~Regression(){};

	int nrow_x, ncol_x;

	boost::numeric::ublas::matrix<double> X;
	boost::numeric::ublas::vector<double> y;
	boost::numeric::ublas::vector<double> yhat;
};


class LinearRegression : public Regression{

public:
	LinearRegression(const vector<double>&, matrix<double>&, bool);

	virtual ~LinearRegression();

	virtual void fit(std::vector<vector<double> >&, double&, std::vector<double>&);
	virtual void confInterval(double, double, matrix<double, column_major>&, vector<double>&, matrix<double>&);
	double student_Tquantile(double);

};

class PolyRegression : public LinearRegression{

public:

	PolyRegression(const vector<double>& _y, matrix<double>& _X,
					bool intercept, int d, std::vector<int>& scols);

	virtual void fit(std::vector<vector<double> >&, double&, std::vector<double>&);
	int degree;
};


class LOESSRegression : public PolyRegression{

public:
	LOESSRegression(const vector<double>&, matrix<double>&, bool intercept,
			int degree, std::vector<int>& scols, double span, SeriesType sType,  Weight<double>&, dist<double>);

	virtual void fit(std::vector<vector<double> >&, double&, std::vector<double>&);

	virtual void setStaticSeriesWeight(matrix<double>& tempX, vector<double>& tempy, int index);
	virtual void setTimeSeriesWeight(matrix<double>& tempX, vector<double>& tempy, int index);

	int solve(const matrix<double>& X_curr,  const vector<double>& y_curr,
				matrix<double, column_major>& A, vector<double>& b,  const vector<double>& w);


	double span;
	dist<double> distFun;
	SeriesType seriesType;
	vector<double> maxDist;
	matrix<double> W;
	Weight<double>& weight;
};



class RLOESSRegression : public LOESSRegression{

public:
	RLOESSRegression(const vector<double>&, matrix<double>&, bool intercept,
			int degree, std::vector<int>& _polyCol, double span, SeriesType,
			int numRobustItr, Weight<double>&, Weight<double>&, dist<double>);

	virtual void fit(std::vector<vector<double> >&, double&, std::vector<double>&);
	virtual void setStaticSeriesWeight(int i, double med, matrix<double>& tempX,  vector<double>& tempy);
	virtual void setTimeSeriesWeight(int i, double med, matrix<double>& tempX,  vector<double>& tempy);

	int robustItr;
	Weight<double>& weight;
	matrix<double> W;
};




#endif /* LINEARREGRESSION_H_ */
