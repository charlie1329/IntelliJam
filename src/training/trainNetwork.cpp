/**
 * implements prototypes etc. from trainNetwork.h
 * Author: Charlie Street
 */

#include <utility>
#include <random>
#include <chrono>
#include <fstream>
#include <iostream>


#include "../../include/training/trainNetwork.h"
#include "../../include/runtime/init_close.h"

/**
 * carry out ridge regression to find the reservoir-output weights
 * @param echo the echo state network
 * @param trainingSet the training set
 * @param epochs the number of training epochs (not used here)
 */
void trainNetwork(shared_ptr<ESN> echo, training_set_t trainingSet, unsigned int epochs) {

    //epochs isn't needed for ridge regression
    (void)epochs; //stop those pesky CLion warnings

    int reservoirSize = echo->getResRes().cols();

    //regularisation setup
    double lambda = 0.1; //TODO: Properly Set

    MatrixXd regMat = lambda * MatrixXd::Identity(reservoirSize,reservoirSize);

    //TODO: Test this copying functionality
    //set up matrices X (input samples as rows) and t (ground truth vectors as rows)
    MatrixXd X = MatrixXd::Zero(trainingSet.size(),reservoirSize);
    MatrixXd t = MatrixXd::Zero(trainingSet.size(),echo->resOutWeights.rows());

    std::cout << "Starting copying of training set" << std::endl;

    //for each training sample
    for(int row = 0; row < trainingSet.size(); row++) {
        training_sample_t currentSample = trainingSet.at(static_cast<unsigned int>(row));

        //copying into X
        for(int col = 0; col < reservoirSize; col++) {
            X(row,col) = currentSample.first(col,0);
        }

        //copying into t
        for(int col = 0; col < currentSample.second.rows(); col++) {
            t(row,col) = currentSample.second(col,0);
        }
    }

    std::cout << "Copied training set into X and t" << std::endl;

    //actually learn the weights
    MatrixXd beforeInv = (X.transpose() * X + regMat);
    MatrixXd beforeTranspose = beforeInv.inverse() * X.transpose() * t;

    echo->resOutWeights = beforeTranspose.transpose();

    std::cout << "Finished Ridge Regression" << std::endl;


}

/**
 * implemented from trainNetwork.h
 * calcualtes total average error on a particular validation set
 * @param echo the echo state network
 * @param validationSet the test samples
 * @return the total average error on the validation set
 */
double getError(shared_ptr<ESN> echo, training_set_t validationSet) {

    //TODO: Actually fill in!!!!
    return 0.0;
}

/**
 * implemented from trainNetwork.h
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
 * @param epochs number of training iterations to carry out
 */
void kfoldWithRepeats(unsigned int repeats, unsigned int folds, string trainingFile,
                      string logFile, shared_ptr<boost::mutex> lock, double v, double r,
                      double a, int N, int k, int inNeurons, int outNeurons, unsigned int epochs) {

    //TODO: Fill in nullptr once I've finished code
    //Set up the Echo State Network
    shared_ptr<ESN> echo = std::make_shared<ESN>(v,r,a,N,k,inNeurons,outNeurons,nullptr,nullptr);

    //form the training set
    shared_ptr<training_set_t> trainingSet = formTrainingSet(echo, std::move(trainingFile),SAMPLE_JUMP);

    //variable for the final output value
    double totalError = 0;


    //get fold size
    int gapPerFold = trainingSet->size() / folds;

    //loop over each repeat
    for(int repeat = 0; repeat < repeats; repeat++) {

        //average error throughout this repeat
        double repeatError = 0;

        //randomly shuffle about the training set to reduce any statistical bias
        long long int seed = chrono::high_resolution_clock::now().time_since_epoch().count();
        shuffle(trainingSet->begin(),trainingSet->end(),default_random_engine(seed));

        //loop over each fold
        for(unsigned int fold = 0; fold < folds; fold++) {

            //variables for current training and validation set
            unsigned int endVal = (fold+1) * gapPerFold;
            unsigned int endWithSize = (endVal <= trainingSet->size()) ? endVal : trainingSet->size();

            auto start = trainingSet->begin() + (fold * gapPerFold);
            auto finish = trainingSet->begin() + endWithSize;
            //TODO: TEST!!!
            training_set_t currentTraining(trainingSet->begin(), trainingSet->end());
            currentTraining.erase(start,finish); //erase validation set from training set

            training_set_t currentValidation(start,finish);

            //TODO: Make sure these functions reset reservoir appropriately
            trainNetwork(echo,currentTraining,epochs); //train the network
            repeatError += getError(echo,currentValidation);
        }

        repeatError /= ((double)folds); //average k-fold error
        totalError += repeatError; //add average for this repeat to total error
    }

    totalError /= ((double)repeats); //average error over repeats


    //produce the string prior to locking
    string toWrite = "v: " + to_string(v) + " r: " + to_string(r) + " a: " + to_string(a)
                     + " N: " + to_string(N) + " k: " + to_string(k) + " epochs: " + to_string(epochs)
                     + " \n Total average " + to_string(folds) + "-fold error after "
                     + to_string(repeats) + " repeats is: " + to_string(totalError) + " \n";

    //lock and then write to file
    lock->lock();
    ofstream log(logFile,ofstream::out|ofstream::app); //TODO: Check appends correctly
    log << toWrite;
    log.close();
    lock->unlock();

}