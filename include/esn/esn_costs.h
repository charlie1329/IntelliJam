/**
 * this file contains different cost functions
 * used for the echo state network training
 * Author: Charlie Street
 */

#ifndef FYP_ESN_COSTS_H
#define FYP_ESN_COSTS_H

#include "../Eigen/Dense"

using namespace Eigen;

const int intervalArr[12] = {0,9,7,5,4,2,11,1,6,3,8,10};

/**
 * least squares error cost function
 * @param gt ground truth vector
 * @param prediction prediction vector
 * @return the average lse
 */
double lse(VectorXd gt, VectorXd prediction);

/**
 * a smarter cost function that looks at the
 * interval between notes to generate a cost
 * calculates sum, not average
 * @param gt ground truth vector
 * @param prediction
 * @return the total interval cost between the two vectors
 */
double intervalCost(VectorXd gt, VectorXd prediction);

#endif //FYP_ESN_COSTS_H
