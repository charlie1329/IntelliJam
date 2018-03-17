/**
 * header file containing definitions/structures
 * for the genetic algorithm operators
 * Author: Charlie Street
 */

#ifndef ICA_OPERATORS_H
#define ICA_OPERATORS_H

#include <vector>
#include "../Eigen/Dense"

using namespace std;
using namespace Eigen;

//structure which allows some book-keeping
//this should allow things to run more smoothly
struct solution {
    Matrix2d str;
    int front;
    double crowdDistance;
    vector<double> objFunValues;

    //just return true, I don't care about ordering
    bool operator<(const solution &rhs) const {
        return true;
    }
};


//carries out the tournament selection operator
//based on rank and crowding distance

bool tournamentSelection(solution i, solution j);

//carries out a crowding sort on the data
//this includes setting the crowding distances
//hence the pass by reference
void crowdingSort(vector<solution> &F, vector<pair<double,double>> minMax, bool(*comp)(double,double));

//carries out a crossover
//i.e. generates children
void crossover(Matrix<double,2,2> mum, Matrix<double,2,2> dad,
               Matrix<double,2,2> &boy, Matrix<double,2,2> &girl, double niche);

//carries out a mutation on a solution
//uses a normally distributed mutation method
void mutate(Matrix<double,2,2> &sol, double sigma);

#endif //ICA_OPERATORS_H
