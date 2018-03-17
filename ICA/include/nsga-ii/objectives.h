/**
 * this file contains all of the objective functions for
 * my multi-objective optimisation
 * NOTE: all of my objective functions are being written such that
 * an optimal value is the minimum value
 * having all objectives minimised makes things somewhat more straightforward
 * Author: Charlie Street
 */

#ifndef ICA_OBJECTIVES_H
#define ICA_OBJECTIVES_H

#include "../../include/Eigen/Dense"

using namespace Eigen;

//List of Objectives to Consider:
//1. Statistical Independence TODO: how to deal with decorrelation if necessary?
//2. Centre of Stereo Spectrum
//3. Equal on both sides
//4. Unit vectors? TODO: Come back to if everything wrong (not using initially
//5. Frequency Band Check

/* an objective function to carry out ICA
 * by that I mean we try and maximise statistical independence
 * the function may have to be negated (internally) to make
 * it a minimisation function
 * this function is dependent on the data
 * I will use partial function application to get the desired type
 * This function uses the kurtosis measure for non-gaussianity
 */
double statIndependenceKurtObj(Matrix2d W, const Matrix<double,2,Dynamic> &X);


/* find components that are close to the centre
 * of the stereo spectrum, where a lead guitar is likely to be
 */
double spectrumCentreObj(Matrix2d W);


/* exploit the nature of stereo
 * i.e. th weights for a source
 * in the L and R speaker must sum to 1
 */
double stereoSumObj(Matrix2d W);


/* try and look for a guitar's overtone pattern
 * in the FFT of the separated sources
 * really I only need one guitar source
 * so this may be taken into account
 * due to my minimisation constraint
 * this will likely be represented as the difference
 * from some expected shape
 * again partial function application will come to the rescue
 * for the parameters
 */
double freqShapeObj(Matrix2d W, Matrix<double,2,Dynamic> X);

/* try and look at the skewness of the data (once all values are made positive)
 * some preliminary tests I carried out suggested this may be at least worth trying
 */
double skewnessObj(Matrix2d W, const Matrix<double,2,Dynamic> &X);

#endif //ICA_OBJECTIVES_H
