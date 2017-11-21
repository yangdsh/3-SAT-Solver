# 3-SAT_Solver
This is a combined solver for 3-SAT problem, written for class *"Algorithm Design and Analysis"*. 
We imported up to five open source 3-SAT solvers and trained a solver selector by Bayes model.
## Requirement
To run our combined solver, please install open source 3-SAT solvers *cryptominisat, minisat* and *glucose* in corresponding folders in advance. 
You may also install *rsat* and *lingeling* for more functions.
## Build
```makefile
make
cd naive_Bayes-master
make
```
## Run
```makefile
./input                 //run all 3-SAT solvers on 8 thousand test samples to get their running time
./phase                 //analyze the feature of each test sample
./data_processor.out                      //feed the feature data and running time data to the bayes classifier. 
./naive_Bayes-master/naive_bayes.out      //train the bayes classifier and test its performance
```
