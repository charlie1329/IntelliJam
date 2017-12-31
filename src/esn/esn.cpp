/**
 * this file implements all functionality for the echo state network components
 * abstracted away from specific activation functions/topologies etc.
 * Author: Charlie Street
 */

#include <utility>
#include <random>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include "../../include/esn/esn.h"

//either random or zero initial values
//in network states seem to be reasonable
#define INITIAL_RESERVOIR_VALUE 0.0

/**
 * implementing constructor from esn.h
 * @param inRes file path to input-reservoir weights
 * @param resRes file path to reservoir-reservoir weights
 * @param resOut file path to reservoir-output weights
 * @param oAct output activation function
 * @param cost cost function for the network
 */
ESN::ESN(string inRes, string resRes, string resOut,
         double(*oAct)(double), double(*cost)(VectorXd,VectorXd)) {

    //read in the weight matrices
    inResWeights = readWeightMatrix(move(inRes));
    resResWeights = readWeightMatrix(move(resRes));
    resOutWeights = readWeightMatrix(move(resOut));

    //set all hyper parameters to -1, we won't be needing them
    //this value at least tells us not to use them
    inResWeight = -1;
    resWeight = -1;
    biResWeight = -1;
    reservoirSize = -1;
    jumpSize = -1;
    numInputNeurons = -1;
    numOutputNeurons = -1;

    //initialise the reservoir
    //resResWeights is a square matrix, using rows or cols is fine
    reservoir = MatrixXd::Constant(resResWeights.cols(),1, INITIAL_RESERVOIR_VALUE);

    //initialise activation functions
    outputActivation = oAct;
    costFunction = cost;
}

/**
 * implemented constructor from esn.h
 * @param v the input-reservoir weight magnitude
 * @param r the simple cycle reservoir weight
 * @param a the bidirectional cycle weight
 * @param N the size of the reservoir
 * @param k the size of the jump in the bidirectional cycle
 * @param inNeurons number of input neurons
 * @param outNeurons number of output neurons
 * @param oAct the output activation function
 * @param cost the cost function for the network
 */
ESN::ESN(double v, double r, double a, int N, int k, int inNeurons, int outNeurons,
         double(*oAct)(double), double(*cost)(VectorXd,VectorXd)){

    //firstly initialise hyper-parameters
    inResWeight = v;
    resWeight = r;
    biResWeight = a;
    reservoirSize = N;
    jumpSize = k;

    numInputNeurons = inNeurons;
    numOutputNeurons = outNeurons;

    //initialise the function pointers
    outputActivation = oAct;
    costFunction = cost;

    //now initialise the reservoir
    reservoir = MatrixXd::Constant(reservoirSize,1,INITIAL_RESERVOIR_VALUE);

    //set-up the weight matrices

    inResWeights = MatrixXd::Constant(reservoirSize,numInputNeurons,inResWeight);
    //set +/- signs on these values with an average pseudo-random number generator
    default_random_engine gen;
    bernoulli_distribution dis(0.5); //make a 'heads or tails' choice from bernoulli distribution with p = 0.5

    //efficiency isn't really an issue here, this is a one off operation on the start of a training cycle.
    for (int i = 0; i < inResWeights.rows(); i++) {
        for (int j = 0; j < inResWeights.cols(); j++) {
            inResWeights(i,j) *= ((dis(gen)) ? -1 : 1); //either multiply by -1 or 1 depending on random draw
        }
    }

    //initially set to all zeroes, and then add the sparse connections in
    resResWeights = MatrixXd::Zero(reservoirSize,reservoirSize);
    //first cycle
    for(int i = 0; i < reservoirSize; i++) {
        resResWeights(i,(i+1)%reservoirSize) = resWeight;
    }

    //second cycle: these connections are bidirectional
    //TODO: Check conditions on jump size to get something that actually manages to cycle properly!
    for(int i = 0; i < reservoirSize; i+= jumpSize) {
        resResWeights(i,(i+jumpSize)%reservoirSize) = biResWeight;
        resResWeights((i+jumpSize)%reservoirSize,i) = biResWeight;
    }


    //TODO: Check Random Initialisation is some reasonable range
    resOutWeights = MatrixXd::Random(numOutputNeurons,reservoirSize);

}

/**
 * reads csv file into eigen matrix
 * @param matrixPath the path where the matrix is stored
 * @return the new eigen matrix
 */
MatrixXd ESN::readWeightMatrix(string matrixPath) {
    vector<vector<double>> temp;

    ifstream matFile(matrixPath);
    string item;

    vector<double> currentVector;

    //read item by item
    while(getline(matFile,item,',')) {

        if(!item.compare("\n")) { //exit condition, item only has a newline
            temp.push_back(currentVector);
            currentVector.clear();
            break;
        }

        if (item.find('\n') != string::npos) { //if we reach a new line
            temp.push_back(currentVector);
            currentVector.clear();
        }


        double currentVal = stod(item,nullptr);
        currentVector.push_back(currentVal);


    }

    //now put into an eigen matrix

    MatrixXd newMat = MatrixXd::Random(temp.size(),temp.at(0).size());

    for (int i = 0; i < temp.size(); i++) {
        for (int j = 0; j < temp.at(0).size(); j++) {
            newMat(i,j) = temp.at(i).at(j);
        }
    }

    return newMat;


}

/**
 * write a matrix to csv file
 * @param path the file path to store matrix at
 * @param weightMatrix the weight matrix to save
 * @return an error code for whether write was successful
 */
int ESN::writeWeightMatrix(string path, MatrixXd weightMatrix) {
    //first, open the file
    ofstream csvFile;
    csvFile.open(path);
    for (int i = 0; i < weightMatrix.rows(); i++) {
        for (int j = 0; j < weightMatrix.cols(); j++) { //we want to retain the level of precision
            if(!csvFile.is_open()) return 0; //a small level of error checking
            csvFile << setprecision(numeric_limits<double>::digits10 + 1) << weightMatrix(i,j);

            if(!csvFile.is_open()) return 0;
            csvFile << ",";
        }
        if(!csvFile.is_open()) return 0;
        csvFile << "\n";
    }

    csvFile.close();

    return 1;

}

/**
 * saves the ESN weight matrices to file
 */
void ESN::saveNetwork(){

    int write1 = writeWeightMatrix("inputReservoirWeights.csv", inResWeights);
    int write2 = writeWeightMatrix("reservoirReservoirWeights.csv", resResWeights);
    int write3 = writeWeightMatrix("reservoirOutputWeights.csv", resOutWeights);

    //why not do a little error checking to prevent something really bad happening...
    if(!(write1 && write2 && write3)) {
        cout << "Error writing matrices to file. To avoid total loss, here are the matrices:" << endl;
        cout << "Input-Reservoir weights:" << endl;
        cout << inResWeights << endl;
        cout << "Reservoir-Reservoir weights:" << endl;
        cout << resResWeights << endl;
        cout << "Reservoir-Output weights:" << endl;
        cout << resOutWeights << endl;
    }
}

/**
 * updates ESN reservoir on the arrival of new data
 * @param newInput the new input to be fed into the network
 */
void ESN::updateReservoir(VectorXd newInput) {
    //TODO: There is no way this is going to be efficient enough
    //TODO: Carry out speed tests here, try doing it around 200 times a second and see what happens
    reservoir = tanh(((inResWeights * newInput) + (resResWeights * reservoir)).array());
}

/**
 * updates ESN reservoir on the arrival of new (1D) data
 * @param newInput the new input to be fed into the network
 */
void ESN::updateReservoir(double newInput) {
    reservoir = tanh(((inResWeights * newInput) + (resResWeights * reservoir)).array());
}

/**
 * generates a new set of outputs from the echo state network
 * @return the new set of outputs from the readout network
 */
VectorXd ESN::predict() {
    //TODO: Complete - EFFICIENCY CRITICAL
    //TODO: Stop assuming linear output function
    return resOutWeights * reservoir;
}

/**
 * resets the reservoir to its initial state, which here
 * is all the initial reservoir value
 */
void ESN::resetReservoir() {
    reservoir = MatrixXd::Constant(resResWeights.cols(),1, INITIAL_RESERVOIR_VALUE);
}


//Get Functions

/**
 * implemented from esn.h
 * @return input-reservoir weights
 */
MatrixXd ESN::getInRes() {
    return inResWeights;
}

/**
 * implemented from esn.h
 * @return reservoir-reservoir weights
 */
MatrixXd ESN::getResRes() {
    return resResWeights;
}


/**
 * implemented from esn.h
 * @return current reservoir activations
 */
VectorXd ESN::getReservoir() {
    return reservoir;
}
