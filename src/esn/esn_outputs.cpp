/**
 * this file implements functions defined in esn_outputs.h
 * Author: Charlie Street
 */

#include "../../include/esn/esn_outputs.h"
#include <cmath>

/**
 * this is fairly naive at the moment
 * wrapper for rounding function plus some bounds
 * @param value unrounded value
 * @return the rounded value
 */
double roundValInBound(double value) {
    double rounded =  round(value);
    if(rounded < 24) return 24;
    if(rounded > 79) return 79;
    return rounded;
}