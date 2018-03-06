/**
 * header file contains function prototypes for error
 * calculation functions
 * Author: Charlie Street
 */
#ifndef FYP_ERRORCALCULATION_H
#define FYP_ERRORCALCULATION_H

#include "../lstm/lstm.h"
#include "readTraining.h"

/**
 * function calculates total error on lstm network
 * over a particular set of samples
 * @param lstm the network being tested
 * @param samples the samples to be used for testing
 * @return the average error over all samples
 */
double getError(shared_ptr<LSTMNet> lstm, training_set_t samples);

#endif //FYP_ERRORCALCULATION_H
