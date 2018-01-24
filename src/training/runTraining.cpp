/**
 * file runs the training/optimisation procedure
 * Author: Charlie Street
 */

#include "../../include/training/checkpoint.h"
#include "../../include/training/hyperParameters.h"
#include "../../include/training/trainNetwork.h"

#include <iostream>

//prototype for combination generation
vector<vector<double>> generateCombos();

//prototype for optimisation procedure
int optimiseNetwork();

//prototype for thread worker function
void optimiseWorker(vector<pair<vector<double>,bool>> combos, const string &trainingFile,
                    const string &logFile, const shared_ptr<boost::mutex> &lock);

//prototype for single run of training algorithm
//with ridge regression
int singleTrainingRunRidge();

/**
 * function starts the optimisation procedure
 * @return an exit code
 */
int main() {
    //return optimiseNetwork();
    return singleTrainingRunRidge();
}

/**
 * generates all possible combinations of hyperparameters
 * and stores them in vectors
 * @return all combinations in sub vectors
 */
vector<vector<double>> generateCombos() {

    vector<vector<double>> combos;

    //apparently using doubles in loop increment is not a good idea,
    //so I've switched things around a bit
    double vMax = (IN_RES_HIGH - IN_RES_LOW)/IN_RES_STEP;
    double rMax = (RES_R_HIGH - RES_R_LOW)/RES_R_STEP;
    double aMax = (RES_A_HIGH - RES_A_LOW)/RES_A_STEP;
    double NMax = (RES_SIZE_HIGH - RES_SIZE_LOW)/RES_SIZE_STEP;
    double kMax = (RES_JUMP_HIGH - RES_JUMP_LOW)/RES_JUMP_STEP;
    double epochMax = (EPOCH_HIGH - EPOCH_LOW)/EPOCH_STEP;

    //basically just a big nested loop
    for(int v = 0; v <= vMax; v++) {
        for(int r = 0; r <= rMax; r++) {
            for(int a = 0; a <= aMax; a++) {
                for(int N = 0; N <= NMax; N++) {
                    for(int k = 0; k <= kMax; k++) {
                        for(int epoch = 0; epoch <= epochMax; epoch++) {

                            double vVal = IN_RES_LOW + (v * IN_RES_STEP);
                            double rVal = RES_R_LOW + (r * RES_R_STEP);
                            double aVal = RES_A_LOW + (a * RES_A_STEP);
                            double NVal = RES_SIZE_LOW + (N * RES_SIZE_STEP);
                            double kVal = RES_JUMP_LOW + (k * RES_JUMP_STEP);
                            double epochVal = EPOCH_LOW + (epoch * EPOCH_STEP);

                            //push back the current combination
                            vector<double> currentCombo;
                            currentCombo.push_back(vVal);
                            currentCombo.push_back(rVal);
                            currentCombo.push_back(aVal);
                            currentCombo.push_back(NVal);
                            currentCombo.push_back(kVal);
                            currentCombo.push_back(epochVal);

                            combos.push_back(currentCombo);
                        }
                    }
                }
            }
        }
    }
}

/**
 * worker function for threads used to optimise the ESN
 * @param combos the hyperparameter combinations this thread has been tasked with
 * @param trainingFile the file containing all information for the training data
 * @param logFile where to store all information about training results
 * @param lock the lock for access to the log file
 */
void optimiseWorker(vector<pair<vector<double>,bool>> combos, const string &trainingFile,
                    const string &logFile, const shared_ptr<boost::mutex> &lock) {

    //loop through combos and train with those combinations
    for (auto &combo : combos) {
        if(!combo.second) { //if not already completed
            vector<double> currentCombo = combo.first;
            kfoldWithRepeats(REPEATS,FOLDS,trainingFile,logFile,lock,currentCombo.at(0),
                             currentCombo.at(1),currentCombo.at(2),(int)currentCombo.at(3),
                             (int)currentCombo.at(4),IN_NEURONS,OUT_NEURONS,(unsigned int)currentCombo.at(5));
        }
    }
}


/**
 * starts up the process of optimising the ESN
 * @return an exit code
 */
int optimiseNetwork() {

    shared_ptr<boost::mutex> lock = std::make_shared<boost::mutex>(); //make shared lock

    vector<pair<vector<double>,bool>> combos = findCompleted(generateCombos(),LOG_FILE);

    //separation points in vector
    unsigned int endOfFirst = combos.size()/4;
    unsigned int endOfSecond = combos.size()/2;
    unsigned int endOfThird = 3*combos.size()/4;

    //TODO: CHECK THIS LINES UP HERE AND IN FILETOECHO FILE!
    //create these separated vectors for each worker thread
    vector<pair<vector<double>,bool>> fstVec(combos.begin(), combos.begin() + endOfFirst);
    vector<pair<vector<double>,bool>> sndVec(combos.begin() + endOfFirst, combos.begin() + endOfSecond);
    vector<pair<vector<double>,bool>> thirdVec(combos.begin() + endOfSecond, combos.begin() + endOfThird);
    vector<pair<vector<double>,bool>> fourthVec(combos.begin() + endOfThird, combos.end());

    boost::thread workerOne(optimiseWorker, fstVec, TRAINING_FILE, LOG_FILE, lock);
    boost::thread workerTwo(optimiseWorker, sndVec, TRAINING_FILE, LOG_FILE, lock);
    boost::thread workerThree(optimiseWorker, thirdVec, TRAINING_FILE, LOG_FILE, lock);
    boost::thread workerFour(optimiseWorker, fourthVec, TRAINING_FILE, LOG_FILE, lock);

    workerOne.join();
    workerTwo.join();
    workerThree.join();
    workerFour.join();

    return 0; //everything fine
}

/**
 * function carries out a single training run of the ESN
 * and then stores the weights for future use
 * @return an exit code (probably just 0)
 */
int singleTrainingRunRidge() {

    //constructor values taken from Rodan & Tino's paper
    //output and cost function not needed (currently) during training
    shared_ptr<ESN> echo = std::make_shared<ESN>(1.0,0.9,0.4,200,13,1,8,nullptr,nullptr);
    std::cout << "Initialised Echo State Network" << std::endl;

    //read in the training set
    shared_ptr<training_set_t> trainingSet = formTrainingSet(echo,"D:/trainingData.csv",10);
    std::cout << "Finished reading in training set" << std::endl;

    //train the network
    //no epochs currently Needed
    trainNetwork(echo,*trainingSet,0);

    //now just write out the weight matrices for future use
    echo->saveNetwork();

    return 0;
}