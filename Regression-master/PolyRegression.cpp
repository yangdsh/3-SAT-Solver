/*
 * PolyRegression.cpp
 *
 *  Created on: Jun 2, 2014
 *      Author: Joseph Egbulefu
 */

#include "Regression.h"


namespace ublas = boost::numeric::ublas;


/*
 *
 */
PolyRegression :: PolyRegression(const ublas::vector<double>& _y, ublas::matrix<double>& _X,
		bool intercept, int d, std::vector<int>& scols) :  LinearRegression(_y, _X, intercept), degree(d){

	int prev_ncols = ncol_x;

 	if(degree > 1){
		if(scols.size() > 0){
			/* add degree - 1 additional columns for dth degree polynomial regression */
			ncol_x = X.size2() + degree - 1;
			X.resize(X.size1(), ncol_x, true);
			int jz = 0, j = 0;

			for(jz = 0; jz < scols.size(); jz++){
				for(int k = 1; k < degree; k++){
					j = prev_ncols + jz * degree + k - 1;
					for(int i = 0; i < X.size1(); i++){
						X(i, j) = pow((double)X(i, scols.at(jz)), k + 1);
					}
				}
				jz++;
			}
		}
	}
}

void PolyRegression :: fit(std::vector<ublas::vector<double> >& beta, double& sigma, std::vector<double>& residual){
	LinearRegression::fit(beta, sigma, residual);
}
