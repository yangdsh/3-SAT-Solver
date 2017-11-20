/*
 * LOESSRegression.cpp
 *
 *  Created on: Jun 2, 2014
 *      Author: Joseph Egbulefu
 */

#include "Regression.h"
#include "Weight.h"

#include <vector>
#include <iostream>
#include <Rcpp.h>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/median.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/accumulators/statistics/p_square_quantile.hpp>
#include <boost/accumulators/framework/extractor.hpp>
#include <boost/numeric/bindings/traits/ublas_vector2.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>


namespace accum = boost::accumulators;
namespace ublas = boost::numeric::ublas;


RLOESSRegression :: RLOESSRegression(const ublas::vector<double>& _y, ublas::matrix<double>& _X,
		bool intercept, int _degree, std::vector<int>& _polyCol, double _span, SeriesType _sType, int _robustItr,
		Weight<double>& _locWeight, Weight<double>& _robWeight, dist<double> dFun)
		:  LOESSRegression(_y, _X, intercept, _degree, _polyCol, _span, _sType, _locWeight, dFun),
		   robustItr(_robustItr), weight(_robWeight),
		   W(ublas::matrix<double>(nrow_x, _span * nrow_x)){}


void RLOESSRegression :: setTimeSeriesWeight(int i, double med, ublas::matrix<double>& tempX,  ublas::vector<double>& tempy){

	int r = (int)std::ceil(span * nrow_x);
	ublas::vector<double> vy(1), vfit(1);

	for(int s = 0; s < r; s++){
		if(i - s > 0){
			ublas::row(tempX, s) = ublas::row(X, i - s);
			tempy(s) = y(i - s);

			vy(0) = y(i - s);
			vfit(0) = yhat(i - s);
			W(i, s) = weight(vy, vfit, med) * LOESSRegression::W(i, s);
		}else{ 	/* do nothing */; }
	}
}

void RLOESSRegression :: setStaticSeriesWeight(int i, double med, ublas::matrix<double>& tempX,  ublas::vector<double>& tempy){

	int r = (int)std::ceil(span * nrow_x);
	int r2_floor = r/2, s = 0;
	ublas::vector<double> vy(1), vfit(1);

	for(s = 0; s < r2_floor; s++){
		if(i - s < 0){
			ublas::row(tempX, r2_floor + s) = ublas::row(X, i + s);
			tempy(r2_floor + s) = y(i + s);

			vy(0) = y(i + s);
			vfit(0) = yhat(i + s);
			W(i, r2_floor + s) = weight(vy, vfit, med) * LOESSRegression::W(i, s);
		}else if(i + s > nrow_x){
			ublas::row(tempX, s) = ublas::row(X, i - s);
			tempy(s) = y(i - s);

			vy(0) = y(i - s);
			vfit(0) = yhat(i - s);
			W(i, s) = weight(vy, vfit, med) * LOESSRegression::W(i, s);
		}else{
			ublas::row(tempX, s) = ublas::row(X, i - s);
			ublas::row(tempX, r2_floor + s) = ublas::row(X, i + s);
			tempy(s) = y(i - s);
			tempy(r2_floor + s) = y(i + s);

			vy(0) = y(i - s);
			vfit(0) = yhat(i - s);
			W(i, s) = weight(vy, vfit, med);
			vy(0) = y(i + s);
			vfit(0) = yhat(i + s);
			W(i, r2_floor + s) = weight(vy, vfit, med) * LOESSRegression::W(i, s);;
		}
	}

	if((r % 2 != 0) && (i + s < nrow_x)){
		ublas::row(tempX, r2_floor + s) = ublas::row(X, i + s);
		tempy(r2_floor + s) = y(i + s);

		vy(0) = y(i + s);
		vfit(0) = yhat(i + s);
		W(i, r2_floor + s) = weight(vy, vfit, med)  * LOESSRegression::W(i, s);
	}
}



void RLOESSRegression :: fit(std::vector<ublas::vector<double> >& beta, double& sigma, std::vector<double>& residual){

	int r = span * nrow_x, ierr;
	double sse = 0, med = 1;
	ublas::matrix<double> tempX(nrow_x, r);
	ublas::matrix<double,ublas::column_major> A(ncol_x, ncol_x);
	ublas::vector<double> b(ncol_x), tempy(r);

	LOESSRegression::fit(beta, sigma, residual);

	for(int t = 0; t < robustItr; t++){
		/* calculate median using boost accumilator */
		accum::accumulator_set<double, accum::stats<accum::tag::median(accum::with_p_square_quantile) > > acc;
		std::for_each(residual.begin(), residual.end(), boost::bind<void>( boost::ref(acc), _1 ) );

		med = accum::median(acc);

		for(int i = 0; i < nrow_x; i++){
			if(seriesType == STATIC)
				setStaticSeriesWeight(i, med, tempX, tempy);
			else if(seriesType == TIME)
				setTimeSeriesWeight(i, med, tempX, tempy);

			ierr = LOESSRegression::solve(tempX, tempy, A, b, ublas::row(W, i));

			if((ierr == 0) && (t == robustItr - 1)){
				beta.push_back(b);
				sse += std::pow((double) residual[i], 2.0);
			}else if(ierr > 0){
				throw std::logic_error("Failure during computation");
			}else{
				throw std::logic_error("Failure: check inputs");
			}
			yhat(i) = ublas::inner_prod(b, ublas::row(X, i));
			residual[i] = y(i) - yhat(i);

			A.clear();
			b.clear();
			tempX.clear();
			tempy.clear();
		}
	}
	sigma = sse / (nrow_x - ncol_x);
}
