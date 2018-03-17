/*
 * this file implements the functions from ICA.h
 * i.e. it implements everything necessary for ICA
 * Author: Charlie Street
 */


#include "../include/ICA.h"
#include <cmath>
#include <iostream>

/* implemented from ICA.h
 * g(u) = ue^(-(u^2)/2)
 */
Matrix<double,1,Dynamic> g(Matrix<double,1,Dynamic> wTX) {

    //Matrix<double,1,Dynamic> cpy = std::move(wTX);

    //matrix access = (row,column)
    for(int i = 0; i < wTX.cols(); i++) {
        double u = wTX(0,i);
        wTX(0,i) = u * exp(-(pow(u,2))/2.0);
    }

    return wTX;
}

/* implemented from ICA.h
 * g'(u) = (1 - u^2)e^(-(u^2)/2)
 */
Matrix<double,1,Dynamic> gPrime(Matrix<double,1,Dynamic> wTX) {
    //Matrix<double,1,Dynamic> cpy = std::move(wTX);

    for(int i = 0; i < wTX.cols(); i++) {
        double u = wTX(0,i);
        double sq = pow(u,2);
        wTX(0,i) = (1 - sq) * exp(-sq/2.0);
    }

    return wTX;
}


/* implemented from ICA.h
 * function implements the fastICA algorithm
 * assumes data is centered and whitened
 * returns the centred and whitened W = A^-1
 * pseudo code can be found here: https://en.wikipedia.org/wiki/FastICA
 */
Matrix<double,Dynamic,Dynamic> fastICA(Matrix<double,SPEAKERS,Dynamic> X, int components) {
    Matrix<double,SPEAKERS,Dynamic> W = MatrixXd::Random(SPEAKERS,components); //randomly initialise vectors

    Matrix<double,SPEAKERS,1> decorrelate;
    decorrelate << 0,0;

    Matrix<double,1,Dynamic> wTX;

    double M = X.cols();

    // carry out the iterative part of the algorithm
    for(int p = 0; p < components; p++) {
        int iterations = 0;
        Matrix<double,SPEAKERS,1> wpOld;
        do {
            std::cout << "iteration: " + std::to_string(iterations) << endl;
            cout << wpOld << endl;
            wpOld = W.col(p);

            wTX = W.col(p).transpose() * X;

            W.col(p) = ((X * g(wTX).transpose()) /M);
            W.col(p) -= (gPrime(wTX).mean() * wpOld); //needs to be wpOld because of the previous statement

            //do de-corrleation to stop convergence to same thing
            decorrelate(0,0) = 0.0; //reset
            decorrelate(1,0) = 0.0;

            for(int j = 0; j < p; j++) {
                decorrelate += (W.col(p).dot(W.col(j)) * W.col(j));
            }

            W.col(p) -= decorrelate;

            //make this particular column a unit vector
            //the norm being used here is the square root of the dot product of the vector with itself
            W.col(p) /= W.col(p).norm();

            //iteration check just in case of rounding errors etc.
            //however I am still giving myself some small area of error to encourage convergence
            cout << W.col(p) << endl;
            iterations++;
        } while(iterations <= MAX_ITERATIONS && !W.col(p).isApprox(wpOld,0) && (W.col(p) + wpOld).norm() > 1e-6);
    }

    return W;
}