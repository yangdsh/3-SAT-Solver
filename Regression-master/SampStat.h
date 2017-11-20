/*
 * SampStat.h
 *
 *  Created on: May 11, 2014
 *      Author: student
 */

#ifndef SAMPSTAT_H_
#define SAMPSTAT_H_

#include <boost/numeric/ublas/vector.hpp>
#include <algorithm>

namespace ublas = boost::numeric::ublas;
/*
 * assumes T.operator+()
 * and T.operator/() are defined for T
 */
template<class T> T median(ublas :: vector<T> x){
	int m = x.size() / 2;
	sort(x.begin(), x.end());
	if((double) m == 0){
		return (x(m - 1) + x(m))  / 2;
	}else{
		return x(m - 1);
	}
}

template<class T> ublas::vector<T> median(ublas :: matrix<T> X){

	ublas::vector<T> out(X.size2());
	for(int i = 0; i < X.size2(); i++){
		out(i) = median<T>(column(X, i));
	}
	return out;
}



#endif /* SAMPSTAT_H_ */
