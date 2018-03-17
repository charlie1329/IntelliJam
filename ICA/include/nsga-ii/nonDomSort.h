/**
 * file contains headers for functions/structures related
 * to non-dominated sorting, which helps to find the Pareto-optimal front
 * Author: Charlie Street
 */

#ifndef ICA_NONDOMSORT_H
#define ICA_NONDOMSORT_H

#include <set>
#include <vector>
#include "../../include/nsga-ii/operators.h"
#include "../../include/Eigen/Dense"

using namespace std;
using namespace Eigen;


/* this function checks for domination of one solution to the other
 * note that I am assuming objective functions are
 * all to be maximised or all to be minimised
 */
bool dominates(solution i,solution j, bool(*comp)(double,double));

/* this function finds the (rank 1) non-dominated set of the population
 * again making assumptions that either all objective to be maximised
 * or all to be minimised
 */
set<solution> findParetoFront(set<solution> population, bool(*comp)(double,double));


/* this function finds all non-dominated sets of the population and sorts them by rank
 * again making assumptions that either all objective to be maximised
 * or all to be minimised
 */
vector<set<solution>> nonDominatedSort(set<solution> population, bool(*comp)(double,double), int N);

/* the intermediate function for finding the pareto optimal front
 */
vector<solution> front(vector<solution> P, bool(*comp)(double,double));

#endif //ICA_NONDOMSORT_H
