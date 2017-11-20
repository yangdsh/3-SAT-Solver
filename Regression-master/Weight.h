/*
 * Weight.h
 *
 *  Created on: April 20, 2014
 *      Author: Joseph Egbulefu
 */

#ifndef WEIGHT_H_
#define WEIGHT_H_


#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/blas.hpp>
#include "Dist.h"



template<class T> class Weight{
public:
	double span;
	dist<double> d;
	Weight(double _span): span(_span), d(identity<double>){};


	Weight(double _span, dist<T> _d): span(_span), d(_d){};
	virtual ~Weight(){};

	virtual double operator()(const boost::numeric::ublas::vector<T>&, const boost::numeric::ublas::vector<T>&, double){ return 1.0;};
};


template< class T> class Tricube : public Weight<T>{
public:
	Tricube(double _span, dist<T> _d): Weight<T>(_span, _d){};
	virtual ~Tricube(){};
/*
	virtual double operator()(const boost::numeric::ublas::vector<T>& x,
			const boost::numeric::ublas::vector<T>& center, double mDist);
*/
	virtual double operator()(const boost::numeric::ublas::vector<T>& x,
			const boost::numeric::ublas::vector<T>& center, double mDist){

		dist<double> d = &euclid_dist<double>;
		return pow(1 - pow(fabs((double)(d(x, center))) / mDist, 3), 3);
	}

};

template<class T> class Bisquare : public Weight<T>{
public:

	Bisquare(double _span, dist<T> _d): Weight<T>(_span, _d){};

	virtual ~Bisquare(){};
/*
	virtual double operator()(const boost::numeric::ublas::vector<T>& x,
			const boost::numeric::ublas::vector<T>& center, double mDist);
*/
	virtual double operator()(const boost::numeric::ublas::vector<T>& x,
			const boost::numeric::ublas::vector<T>& center, double mDist){

		dist<double> d = &euclid_dist<double>;
		std::cout << "in bisquare \n";
			return pow(1 - pow(d(x, center), 2.0) / mDist, 2.0);
	}

};
#endif /* WEIGHT_H_ */
