/**
 * this file contains the key
 * definitions/prototypes for running the nsga algorithm
 * Author: Charlie Street
 */

#ifndef ICA_NSGA_II_H
#define ICA_NSGA_II_H

#include "../../include/nsga-ii/operators.h"
#include "../../include/Eigen/Dense"

#include <set>
#include <vector>

using namespace std;
using namespace Eigen;

//need a random population
void randomPopulation(set<solution> &population, unsigned int N);

set<solution> runNSGA(void (*randGen)(set<solution>&, unsigned int), unsigned int generations,
             void(*cross)(Matrix2d,Matrix2d,Matrix2d&,Matrix2d&,double), void(*mutate)(Matrix2d&,double),
                      vector<function<double(Matrix2d)>> objFuns,
                bool(*comp)(double, double), unsigned int N, double niche, double sigma);




//function prototypes

void calculateObjectives(set<solution> &population, vector<function<double(Matrix2d)>> objFuns);

vector<pair<double,double>> getMinMaxValues(set<solution> P);

void generateOffspring(set<solution> P, set<solution> &Q,
                       void(*cross)(Matrix2d,Matrix2d,Matrix2d&,Matrix2d&,double), void(*mutate)(Matrix2d&,double),
                       double niche, double sigma);


#endif //ICA_NSGA_II_H
