/*
 * file contains definitions for ICA.h
 * Author: Charlie Street
 */

#ifndef ICA_ICA_H
#define ICA_ICA_H

#define MAX_ITERATIONS 50 //max number of iterations around the algorithm

#include <Eigen/Dense>
#include "ICAHelpers.h"

using namespace Eigen;

//function g from the literature
Matrix<double,1,Dynamic> g(Matrix<double,1,Dynamic> wTX);

//the derivative of g, applied coefficient wise
Matrix<double,1,Dynamic> gPrime(Matrix<double,1,Dynamic> wTX);

//the function that carries out the iterative ica algorithm
Matrix<double,Dynamic,Dynamic> fastICA(Matrix<double,SPEAKERS,Dynamic> X, int components);

#endif //ICA_ICA_H
