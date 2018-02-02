/**
 * file implements functions with prototypes
 * in auxillary_functions.h
 * Author: Charlie Street
 */

#include "../../include/lstm/auxillary_functions.h"

/**
 * implemented from auxillary_functions.h
 * function acts as a sigmoid activation function
 * @param x the input
 * @return the sigmoid activation of x
 */
double sigmoid(double x) {
    return 1.0/(1.0 + exp(-x));
}

/**
 * implemented from auxillary_functions.h
 * takes a note in the range LOWEST-HIGHEST
 * and compresses it to be in the range [0,1]
 * @param note the note to compress
 * @return the compressed note between 0 and 1
 */
double compressNote(double note) {
    return (note - LOWEST_NOTE)/(HIGHEST_NOTE - LOWEST_NOTE);
}

/**
 * implemented from auxillary_functions.h
 * takes a compressed note in the range [0,1]
 * and converts it back into a note in the range LOWEST-HIGHEST
 * @param compressedNote the compressed note in the range [0,1]
 * @return the decompressed note
 */
unsigned int decompressNote(double compressedNote) {
    return static_cast<unsigned int>((HIGHEST_NOTE - LOWEST_NOTE) * compressedNote + LOWEST_NOTE);
}

/**
 * implemented from auxillary_functions.h
 * function applies a function to all elements of a vector
 * @param activation the activation function to apply
 * @param vec the vector to be modified
 * @return the modified vector
 */
VectorXd applyToAll(double(*activation)(double),VectorXd vec) {
    for(int i = 0; i < vec.rows(); i++) {
        vec(i,0) = activation(vec(i,0));
    }

    return vec;
}

/**
 * function calculates element wise product of two Eigen vectors
 * @param x the first vector
 * @param y the second vector
 * @return the element wise product
 */
VectorXd elementWiseProduct(VectorXd x, VectorXd y) {
    for(int i = 0; i < x.rows(); i++) {
        x(i,0) *= y(i,0);
    }

    return x; //x was a copy, so all should be fine
}