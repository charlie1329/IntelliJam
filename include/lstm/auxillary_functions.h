/**
 * file contains prototypes for functions
 * which may be useful in and around the use of the lstm
 * Author: Charlie Street
 */

#ifndef FYP_AUXILLARY_FUNCTIONS_H
#define FYP_AUXILLARY_FUNCTIONS_H

#include <cmath>
#include "../Eigen/Dense"

using namespace Eigen;

//note bounds for use in the system
#define LOWEST_NOTE 24.0
#define HIGHEST_NOTE 79.0

/**
 * takes a note in the range LOWEST-HIGHEST
 * and compresses it to be in the range [0,1]
 * @param note the note to compress
 * @return the compressed note between 0 and 1
 */
double compressNote(double note);

/**
 * takes a compressed note in the range [0,1]
 * and converts it back into a note in the range LOWEST-HIGHEST
 * @param compressedNote the compressed note in the range [0,1]
 * @return the decompressed note
 */
unsigned int decompressNote(double compressedNote);

/**
 * function acts as a sigmoid activation function
 * @param x the input
 * @return the sigmoid activation of x
 */
double sigmoid(double x);

/**
 * function applies a function to all elements of a vector
 * @param activation the activation function to apply
 * @param vec the vector to be modified
 * @return the modified vector
 */
VectorXd applyToAll(double(*activation)(double),VectorXd vec);

/**
 * function calculates element wise product of two Eigen vectors
 * @param x the first vector
 * @param y the second vector
 * @return the element wise product
 */
VectorXd elementWiseProduct(VectorXd x, VectorXd y);

#endif //FYP_AUXILLARY_FUNCTIONS_H
