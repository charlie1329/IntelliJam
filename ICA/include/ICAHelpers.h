/*
 * This file contains definitions/prototypes for all of the helper
 * and pre-processing steps useful for carrying out Independent Component Analysis
 * Author: Charlie Street
 */

#ifndef ICA_ICAHELPERS_H
#define ICA_ICAHELPERS_H

#define SPEAKERS 2 //the number of 'microphones' we have in our ICA
//SPEAKERS is the dimensionality of our data

#include <Eigen/Dense>
#include <memory>
#include <utility>

using namespace std;
using namespace Eigen;

//struct for storing information about eigenvector decomposition
struct EigenDecomp {
    Matrix<double,SPEAKERS,SPEAKERS> eigenVectors;
    Matrix<double,SPEAKERS,SPEAKERS> eigenValueMatRoot;
    EigenDecomp(Matrix<double,SPEAKERS,SPEAKERS> newVecs, Matrix<double,SPEAKERS,SPEAKERS> newVals);
};

/* naming conventions
 * X is a matrix of our mixtures of the sources for all time steps (frames)
 * A is the matrix of weights for the sources
 * S is the matrix with our source values at all times
 * m is the mean vector of X
 * if X/S appear as lower case then it means it is a vector for time slice t
 */


//retrieves mean vector from X and returns it (m)
Matrix<double,SPEAKERS,1> subtractMean(Matrix<double,SPEAKERS,Dynamic> &X);


//add back mean after obtaining sources
void addMean(Matrix<double,Dynamic,Dynamic> &s, Matrix<double,SPEAKERS,Dynamic> A, Matrix<double,SPEAKERS,1> m);

/* whitens the data in X, by this I mean we find the eigen vectors and values and
 * project the data into the space such that the covariance matrix = I = the identity matrix
 * the eigenvectors are then returned from the function
 * assumes data is already centred
 */
shared_ptr<EigenDecomp> whitenData(Matrix<double,SPEAKERS,Dynamic> &X);

/* essentially the opposite of what whiten does
 * it turns it back into the original data format
 * it does it to matrix A~ rather than X
 */
void unwhitenData(Matrix<double,SPEAKERS,Dynamic> &A, EigenDecomp eigen);

#endif //ICA_ICAHELPERS_H
