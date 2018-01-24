/**
 * file contains a multitude of output functions
 * that could be used by the echo state network
 * Author: Charlie Street
 */

#ifndef FYP_ESN_OUTPUTS_H
#define FYP_ESN_OUTPUTS_H

/**
 * just a wrapper for C++'s default rounding function
 * plus something to keep within output bounds of guitar
 * @param value the unrounded value
 * @return the rounded,bounded value
 */
double roundValInBound(double value);

#endif //FYP_ESN_OUTPUTS_H
