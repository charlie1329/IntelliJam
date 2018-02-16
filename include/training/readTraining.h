/**
 * file contains prototypes and definitions
 * for the functionality of reading in the training set from memory
 * Author: Charlie Street
 */

#ifndef FYP_READTRAINING_H
#define FYP_READTRAINING_H

#include <fstream>
#include <vector>
#include <string>
#include "../Eigen/Dense"

using namespace Eigen;

using namespace std;

//a training set is represented as a list of samples
//each sample is a list of pairs, where each pair represents a note
//and a duration
typedef vector<vector<VectorXd>> training_set_t;

/**
 * function takes a file containing training data
 * and formats it appropriately
 * @param trainingPath the location of the training data file
 * @return a vector of vectors of pairs representing the training set
 */
training_set_t readTrainingSet(string trainingPath);

#endif //FYP_READTRAINING_H
