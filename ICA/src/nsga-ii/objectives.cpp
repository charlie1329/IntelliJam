/**
 * file implements functions provided in objectives.h
 * Author: Charlie Street
 */

#include "../../include/nsga-ii/objectives.h"
#include "../../include/nsga-ii/hyperParameters.h"

#include <stdlib.h>


/** this objective functions makes
 * a particular source's weights sum to 1
 * over the L/R speaker
 * derivations/formulation made by myself
 * @param W the current weight matrix
 * @return the distance from the ideal solution
 */
double stereoSumObj(Matrix2d W) {
    Matrix2d A = W.inverse();
    return abs(A(0,0) + A(1,0) - 1.0) + abs(A(0,1) + A(1,1) - 1.0);
}

/** this objective function tries to bring W
 * (or the resulting sources s) into the centre of the
 * stereo spectrum, which is where I am likely to find a lead
 * guitar
 * @param W the current W matrix in ICA
 * @return a double representing the total element-wise difference from the optimum
 * optimum is [[z/(x(z-y)) -y/(x(z-y))]; [-1/(z-y) 1(z-y)]]
 * where x, y, z are from A = W^-1 [[x y]; [x z]]
 * we want each value of the first column of A to be 0.5
 */
double spectrumCentreObj(Matrix2d W) {
    Matrix2d A = W.inverse();
    return abs(A(0,0) - 0.5) + abs(A(1,0) - 0.5);
}

/** this function measures the statistical independence
 * of the source signals via non-gaussianity
 * we want to maximise the non-gaussianity
 * of our source signals which correlates to independence
 * we turn this into a minimisation using a constant
 * @param W the solution matrix W
 * @param X the (whitened) data
 * @return the kurtosis objective value
 */
double statIndependenceKurtObj(Matrix2d W, const Matrix<double,2,Dynamic> &X) {
    Matrix<double,2,Dynamic> s = (W * X).unaryExpr([](double x){return pow(x,4);});
    //this may require speed-up if too slow!
    Matrix<double,2,1> mean = s.rowwise().mean();
    return INDEPENDENCE_MAX - abs(mean(0,0)-3.0) - abs(mean(1,0)-3);

}

/** this function aims to find the skewness of the sources
 * we want to maximise the positive skewness, however, this
 * will be a minimisation function so it will be subtracted from a constant
 * @param W the intermediate inverse mixing matrix
 * @param X the original (whitened) data
 * @return the skewness subtracted from a suitably large constant
 */
double skewnessObj(Matrix2d W, const Matrix<double,2,Dynamic> &X) {
    Matrix<double,2,Dynamic> s = (W * X).unaryExpr([](double x){return abs(x);});
    Matrix<double,2,1> mean = s.rowwise().mean();

    double sd1 = 0.0;
    double sd2 = 0.0;
    for(int i = 0; i < s.cols(); i++) {
        sd1 += pow(s(0,i) - mean(0,0),2);
        sd2 += pow(s(1,i) - mean(1,0),2);
    }

    sd1 = sqrt(sd1/(double)s.cols());
    sd2 = sqrt(sd2/(double)s.cols());

    Matrix<double,1,Dynamic> z1 = s.row(0).unaryExpr([=](double x){return (x - mean(0,0))/sd1;});
    Matrix<double,1,Dynamic> z2 = s.row(1).unaryExpr([=](double x){return (x - mean(1,0))/sd2;});

    Matrix<double,1,1> g1 = z1.rowwise().mean();
    Matrix<double,1,1> g2 = z2.rowwise().mean();

    double bias = sqrt(s.cols() * (s.cols() - 1))/(s.cols()-2);

    return INDEPENDENCE_MAX - (bias * g1(0,0) + bias * g2(0,0));


    //TODO: FIX!
}