/**
 * file is used to calculate the error of
 * the echo state network in different situations
 */

#include "../../include/esn/esn_outputs.h"
#include "../../include/esn/esn.h"
#include "../../include/training/fileToEcho.h"
#include "../../include/training/trainNetwork.h"
#include "../../include/esn/esn_costs.h"

#include <iostream>
#include <fstream>

#define IN_RES "trained_weights/inputReservoirWeightsRidge.csv"
#define RES_RES "trained_weights/reservoirReservoirWeightsRidge.csv"
#define RES_OUT "trained_weights/reservoirOutputWeightsRidge.csv"

#define MINI_TRAINING "../test/training/testTraining.csv"


void manuallyCheckSamples() {
    shared_ptr<ESN> echo = std::make_shared<ESN>(IN_RES,RES_RES,RES_OUT,roundValInBound,lse);
    std::cout << "Initialised Echo State Network" << std::endl;

    //read in the considerably smaller test set
    shared_ptr<training_set_t> testSet = formTrainingSet(echo,MINI_TRAINING,10);

    std::cout << "Finished reading in test set" << std::endl;

    ofstream myFile;
    myFile.open("predictions.txt");

    for(int i = 0; i < testSet->size(); i++) {
        echo->setReservoir(testSet->at(i).first);
        VectorXd prediction = echo->predict();
        myFile << "Prediction: " << i << "\n";
        myFile << prediction << "\n";
        myFile << "Ground Truth: " << i << "\n";
        myFile << testSet->at(i).second << "\n";
    }

    myFile << "Finished checking samples" << "\n";
    myFile.close();
}


/**
 * main function sets everything up
 * @return error code
 */
int main() {
    /*
    //constructor values taken from Rodan & Tino's paper
    //output and cost function not needed (currently) during training
    shared_ptr<ESN> echo = std::make_shared<ESN>(IN_RES,RES_RES,RES_OUT,roundValInBound,lse);
    std::cout << "Initialised Echo State Network" << std::endl;

    //read in the training set
    shared_ptr<training_set_t> trainingSet = formTrainingSet(echo,"D:/trainingData.csv",10);
    std::cout << "Finished reading in training set" << std::endl;

    double totalError = getError(echo,*trainingSet);

    std::cout << "Finished Calculating Error" << std::endl;

    //write error to log file
    ofstream writeErrorFile;
    writeErrorFile.open("error.txt");
    writeErrorFile << "Total Average Training Set Error is: " << totalError << "\n";
    writeErrorFile.close();
    */

    manuallyCheckSamples();
    return 0;
}

