/**
 * This file contains function prototypes
 * for functionality related to the training of the echo state network
 * Author: Charlie Street
 */

#ifndef FYP_TRAINNETWORK_H
#define FYP_TRAINNETWORK_H

#define FOLDS 10 //90/10 split training to validation set
#define REPEATS 10 //if we can easily manage this...

#include "fileToEcho.h"
#include <boost/thread.hpp>

/**
 * function trains an echo state network on the provided training set
 * @param echo the echo state network
 * @param trainingSet the list of training samples
 * @param epochs how many iterations to train for?
 */
void trainNetwork(shared_ptr<ESN> echo, training_set_t trainingSet, unsigned int epochs);

/**
 * calculates the total error on a particular validation set
 * @param echo the echo state network
 * @param validationSet the test samples
 * @return the total average error on the validation set
 */
double getError(shared_ptr<ESN> echo, training_set_t validationSet);


/**
 * calculates the average k-fold error for a particular set
 * of network hyper parameters
 * @param repeats the number of repeats to carry out
 * @param folds the number of folds over the input samples
 * @param trainingFile the file where file names and ground truth values can be found
 * @param logFile the file to log all results
 * @param lock the lock for access to the log file
 * @param v weight for input/reservoir connections
 * @param r weight for simple cycle reservoir
 * @param a weight for inner cycle in reservoir
 * @param N size of reservoir
 * @param k size of jumps for inner reservoir cycle
 * @param inNeurons number of input neurons
 * @param outNeurons number of output neurons
 * @param epochs training iterations
 */
void kfoldWithRepeats(unsigned int repeats, unsigned int folds, string trainingFile,
                      string logFile, shared_ptr<boost::mutex> lock, double v, double r,
                      double a, int N, int k, int inNeurons, int outNeurons, unsigned int epochs);

#endif //FYP_TRAINNETWORK_H
