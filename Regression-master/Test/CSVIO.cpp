/*
 * CSVIO.cpp
 *
 *  Created on: Apr 29, 2014
 *      Author: student
 */


#include <iostream>
#include <vector>
#include "TestSet.h"

CSVIO::CSVIO(string filename, int _nrow) : nrow(_nrow), csvfile(filename){
	try{
		ifs.open(csvfile);
	}catch(...){
		cout << "Error opeing file " + filename + "\n";
	}
}

boost::numeric::ublas::matrix<double> CSVIO::getMatrix(){
	std::vector<std::vector<double> > x = read();
	boost::numeric::ublas::matrix<double> X(x.size(), x[1].size());

	for(int j = 0; j < X.size2(); j++){
		for(int i = 0; i < X.size1(); i++){
			X(i, j) = x[i][j];
		}
	}
	return X;
}

std::vector<std::vector<double> > CSVIO::read(){
	CSVRow row;
	std::vector<string> rowStr;
	std::vector<std::vector<double> > out;
	row.read(ifs);
	rowStr = row.getData();
	std::vector<double> tVec(rowStr.size());
	int rowindex = 0;

	while(!ifs.fail() && !ifs.eof() && (rowindex < nrow)){
		row.read(ifs);
		rowStr = row.getData();

		for(int i = 0; i < rowStr.size(); i++){
			tVec[i] =  std::stof(rowStr.at(i));
		}
		out.push_back(tVec);
		++rowindex;
		rowStr.clear();

	}

	return out;
}


std::vector<double> CSVIO::readColumn(){
	CSVRow row;
	std::vector<string> rowStr;
	std::vector<double> out;
	row.read(ifs);
	rowStr = row.getData();


	do{
		row.read(ifs);
		rowStr = row.getData();

		for(int i = 0; i < rowStr.size(); i++){
			out.push_back(std::stof(rowStr.at(i)));
		}
	}while(!(ifs.fail() && ifs.eof()));

	return out;
}


