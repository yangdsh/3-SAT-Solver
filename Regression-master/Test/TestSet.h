/*
 * TestSet.h
 *
 *  Created on: Apr 30, 2014
 *      Author: student
 */

#ifndef TESTSET_H_
#define TESTSET_H_

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>


#include "../Weight.h"
#include "../Dist.h"
#include "../Regression.h"


using namespace std;

class TestSet{
public:
	TestSet(){};
	std::vector<bool> validateResult(const std::vector<std::vector<double> >&,  const std::vector<std::vector<double> >& , const double);
	std::vector<bool> test(int, int, int, string);
};

class CSVIO{
public:
	CSVIO(string fileName, int nrow);
	std::vector<std::vector<double> > read();
	std::vector<double> readColumn();
	boost::numeric::ublas::matrix<double> getMatrix();

	ifstream ifs;
	string csvfile;
	int nrow;

};


class CSVRow
{
	public:
		std::string const& operator[](std::size_t index) const {
			return m_data[index];
		}
		std::size_t size() const {
			return m_data.size();
		}
		void read(std::istream& stream) {
			std::string         line;
			stream.good() ? (void) std::getline(stream, line) : throw -2;

			std::stringstream  lineStream(line);
			std::string         cell;

			m_data.clear();

			while(std::getline(lineStream,cell,',')){
				m_data.push_back(cell);
			}
		}

		std::vector<string> getData(){
			return this->m_data;
		}
	private:
		std::vector<std::string>    m_data;
};





#endif /* TESTSET_H_ */
