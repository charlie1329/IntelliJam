/**
 * this file contains the class structure for an echo state network
 *  after implementation, it will contain all functionality required
 *  of the network for running and training
 *  Author: Charlie Street
 */

#ifndef FYP_ESN_H
#define FYP_ESN_H

#include "../Eigen/Dense"
#include <string>

using namespace Eigen;
using namespace std;

class ESN {
private:

    //Hyper-Parameters
    double inResWeight;
    double resWeight;
    double biResWeight;
    int reservoirSize;
    int jumpSize;

    //size of input/output layers
    int numInputNeurons;
    int numOutputNeurons;

    //reservoir
    VectorXd reservoir;

    //weight matrices
    MatrixXd inResWeights;
    MatrixXd resResWrights;
    MatrixXd resOutWeights;

    //activation functions
    double (*reservoirActivation)(double);
    double (*outputActivation)(double);

    //cost function
    double (*costFunction)(VectorXd groundTruth, VectorXd prediction);

    /**
     * an auxillary function for reading in a weight matrix from a file and
     * formatting it into an eigen matrix
     * @param matrixPath file path to matrix file
     * @return the weight matrix in an eigen-usable format
     */
    MatrixXd readWeightMatrix(string matrixPath);

    /**
     * an auxillary function for writing an eigen matrix to a file
     * so I can load it back in at a later date
     * @param weightMatrix the eigen matrix of weights
     * @return a status code of whether the operation succeeded
     * 1 = success, 0 = failure
     */
    int writeWeightMatrix(MatrixXd weightMatrix);

public:

    /**
     * constructor used for training: set up network manually
     * @param v weight for input/reservoir connections
     * @param r weight for simple cycle reservoir
     * @param a weight for inner cycle in reservoir
     * @param N size of reservoir
     * @param k size of jumps for inner reservoir cycle
     * @param inNeurons number of input neurons
     * @param outNeurons number of output neurons
     * @param rAct reservoir activation function
     * @param oAct output activation function
     * @param cost cost function for network
     */
    ESN(double v, double r, double a, int N, int k, int inNeurons, int outNeurons,
        double (*rAct)(double), double(*oAct)(double),double(*cost)(VectorXd,VectorXd));

    /**
     * constructor for situatuon where network weights already found (i.e. run-time)
     * the difference is that here, the network weights are read in
     * @param inRes file path to input-reservoir weights
     * @param resRes file path to reservoir-reservoir weights
     * @param resOut file path to reservoir-output weights
     * @param inNeurons number of input neurons
     * @param outNeurons number of output neurons
     * @param rAct reservoir activation function
     * @param oAct output activation function
     * @param cost cost function
     */
    ESN(string inRes, string resRes, string resOut, int inNeurons, int outNeurons,
        double (*rAct)(double), double(*oAct)(double), double(*cost)(VectorXd,VectorXd));


    /**
     * takes new input and feeds it into the reservoir
     * @param newInput the new input into the network
     */
    void updateReservoir(VectorXd newInput);

    /**
     * generates a set of predictions for the network
     * @return
     */
    VectorXd predict();

};


#endif //FYP_ESN_H
