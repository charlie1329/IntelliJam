/**
 * This file contains structure/function definitions for
 * functionality of bringing the training set into a nicer format
 * than the wav files they will be represented as
 * Author: Charlie Street
 */
#ifndef FYP_FILETOECHO_H
#define FYP_FILETOECHO_H

#include "../Eigen/Dense"
#include "../esn/esn.h"
#include <string>
#include <memory>
#include <vector>

using namespace std;
using namespace Eigen;

typedef pair<VectorXd,VectorXd> training_sample_t;
typedef vector<training_sample_t> training_set_t;


/**
 * function takes a (mono) wav file, inputs into an echo state network
 * and returns the resulting reservoir state
 * @param echo a pointer to the echo state network
 * @param filePath the path of the wav file in question
 * @param sampleJump represents an artificial decrease in sample rate
 * @return the resulting reservoir state
 */
VectorXd wavToEcho(ESN echo, const string &filePath, unsigned int sampleJump);

/**
 * function takes an entire directory of wav files and uses it to construct entire training set
 * @param echo the echo state network to construct reservoir states from
 * @param samplesAndOuts a file containing a list of all training files as well as the ground truth values
 * @param sampleJump represents an artificial decrease in sample rate
 * @return the entire training set
 */
shared_ptr<training_set_t> formTrainingSet(shared_ptr<ESN> echo, string samplesAndOuts, unsigned int sampleJump);

/**
 * this function reads from a csv file
 * and takes the file paths of input samples and ground truth values
 * and then outputs them in a convenient manner
 * @param trainingFile the file to be read from
 * @param numOut the number of outputs per sample
 * @return the data of the csv file, appropriately formatted
 */
vector<pair<string,VectorXd>> readTrainingFile(const string &trainingFile, int numOut);

#endif //FYP_FILETOECHO_H
