/*
 * This file implements the helper functions
 * as specified in ICA Helpers.h
 * Author: Charlie Street
 */

#include "../include/ICAHelpers.h"
#include <Eigen/Eigenvalues>
#include <utility>
#include <iostream>

/* function implemented from ICAHelpers.h
 * mean is subtracted from matrix; the mean vector is returned
 */
Matrix<double,SPEAKERS,1> subtractMean(Matrix<double,SPEAKERS,Dynamic> &X) {

    //create the mean vector
    Vector2d m = X.rowwise().mean();

    //subtract mean from each column in our data
    X.colwise() -= m;

    //return the mean vector back for use later
    return m;
}

/* function implemented from ICAHelpers.h
 * adds the mean back to the source realisations
 * (since we assume treat it as a random variable)
 */
void addMean(Matrix<double,Dynamic,Dynamic> &s, Matrix<double,SPEAKERS,Dynamic> A, Matrix<double,SPEAKERS,1> m) {
    s = s.colwise() + (A.inverse() * m) ;
}


/* function implemented from ICAHelpers.h
 * data is whitened so that our data is uncorrelated and has unit variance
 * put shortly, this means the covariance matrix = I = the identity matrix
 * this involves finding the eigenvectors and then projecting our data into the new space
 */
shared_ptr<EigenDecomp> whitenData(Matrix<double,SPEAKERS,Dynamic> &X) {

    //calculate covariance matrix of X - the n-1 makes less difference when we have 1000s of samples
    //n-1 gives us an unbiased estimate
    Matrix<double,SPEAKERS,SPEAKERS> cov = (X * X.transpose()) / ((double)X.cols()-1);

    //because covariance matrix is adjoint, everything is a little easier:
    //calculate eigenvectors and eigenvalues of covariance matrix
    SelfAdjointEigenSolver<Matrix<double,SPEAKERS,SPEAKERS>> eigen(cov);

    //i want most significant eigen vector first
    Matrix<double,SPEAKERS,SPEAKERS> eigenVecs = eigen.eigenvectors();
    Matrix<double,SPEAKERS,1> tmp = eigenVecs.col(0);
    eigenVecs.col(0) = eigenVecs.col(1);
    eigenVecs.col(1) = tmp;


    //now get the eigenvalue matrix in the correct format (following the literature)
    const Matrix<double,SPEAKERS,1> &eigenVals = eigen.eigenvalues();
    Matrix<double,SPEAKERS,1> eigValSwitched = MatrixXd::Zero(SPEAKERS,1);
    eigValSwitched(0,0) = eigenVals(1,0);
    eigValSwitched(1,0) = eigenVals(0,0);
    Matrix <double,SPEAKERS,1> DVec = eigValSwitched.cwiseSqrt().cwiseInverse(); //do before I add zeroes everywhere

    Matrix<double,SPEAKERS,SPEAKERS> D = DVec.asDiagonal();


    //whiten the data
    X = eigenVecs * D * eigenVecs.transpose() * X;

    return make_shared<EigenDecomp>(eigenVecs,D);
}

/* implements function from ICAHelpers.h
 * brings whitened data back to normal so to speak
 */
void unwhitenData(Matrix<double,SPEAKERS,Dynamic> &A, EigenDecomp eigen) {
    A = eigen.eigenVectors *
        eigen.eigenValueMatRoot.inverse() *
        eigen.eigenVectors.transpose() * A;

}


//general constructor for EigenDecomp struct
EigenDecomp::EigenDecomp(Matrix<double,SPEAKERS,SPEAKERS> newVecs,
                         Matrix<double,SPEAKERS,SPEAKERS> newVals) : eigenVectors(std::move(newVecs)),
                                                              eigenValueMatRoot(std::move(newVals)) {}