/**
 * this file contains two different classes
 * related to the construction of an lstm (Long Short Term Memory)
 * network. One class is for a generic lstm hidden layer
 * and the other is for constructing an entire network
 * Author: Charlie Street
 */

#ifndef FYP_LSTM_H
#define FYP_LSTM_H

//the standard deviation of the normal
//distribution all weight matrices are sampled from
//value taken from "LSTM: A Search Space Odyssey"
#define INITIAL_STD_DEV 0.1

#include "../Eigen/Dense"
#include <random>
#include <chrono>
#include <memory>
#include <string>

using namespace Eigen;
using namespace std;

/**
 * class represents an lstm hidden layer
 */
class LSTMLayer {

private:
    VectorXd h; //the hidden state, or the output of the lstm layer
    VectorXd C; //the cell state (represents the memory)

    /**
     * used to randomly initialise a weight matrix
     * separate function written to improve upon
     * Eigen's default random initialisation
     * @param mat the matrix to initialise (by reference)
     * @param rows the rows of the matrix
     * @param cols the columns of the matrix
     */
    void initialiseWeightMatrix(MatrixXd &mat, unsigned int rows, unsigned int cols);

    /**
     * used to initialise one of the bias vectors
     * separate function because Eigen's is terrible
     * @param b the vector to be initialised (by reference)
     * @param rows the rows in the vector (vector so single column)
     */
    void initialiseBiasVector(VectorXd &b, unsigned int rows);

public:

    //any component to be trained has to be kept public

    //input weight matrices (to be trained)
    MatrixXd theta_xi; //weights from network input to input gate
    MatrixXd theta_xf; //weights from network input to forget gate
    MatrixXd theta_xo; //weights from network input to output gate
    MatrixXd theta_xg; //weights from network input to g (used in C calculation)

    //recurrent weight matrices (to be trained)
    MatrixXd theta_hi; //recurrent weights relative to input gate
    MatrixXd theta_hf; //recurrent weights relative to forget gate
    MatrixXd theta_ho; //recurrent weights relative to output gate
    MatrixXd theta_hg; //recurrent weights relative to g (used in C calculation)

    //bias vectors (trained)
    VectorXd bias_i; //bias vector for input gate
    VectorXd bias_f; //bias vector for forget gate
    VectorXd bias_o; //bias vector for output gate
    VectorXd bias_g; //bias vector for g calculation

    /**
     * used to reinitialise the lstm layer whenever
     * required (e.g. after one round of improvisation)
     */
    void resetState();

    /**
     * constructor sets up the state of the lstm layer
     * @param inputSize the size of the input x
     * @param hiddenOutputSize the size of the output from the hidden layer, i.e. the size of h
     */
    LSTMLayer(unsigned int inputSize, unsigned int hiddenOutputSize);


    /**
     * function takes a new input to the layer
     * and uses it to update the hidden state of the lstm 'cell'
     * @param x_t the new input into the hidden layer
     * @return the new hidden state h (also stored internally)
     */
    VectorXd update(VectorXd x_t);

};

/**
 * class uses LSTMLayers to form an entire network,
 * with an output layer to perform arbitrary tasks
 */
class LSTMNet {

private:

    /**
     * function initialises a weight matrix appropriately
     * used randomness better than Eigen's default
     * @param mat the matrix to be initialised (by reference)
     * @param rows the rows of the matrix
     * @param cols the columns of the matrix
     */
    void initialiseWeightMatrix(MatrixXd &mat, int rows, int cols);

    /**
     * function takes a file path
     * and reads the matrix in that file into an Eigen matrix
     * @param path the file path
     * @return the matrix represented in that file
     */
    MatrixXd readWeightMatrix(string path);

    /**
     * file takes an Eigen matrix and writes it to file
     * @param weightMatrix the matrix to be written
     * @param path the path to the write the matrix too
     */
    void writeWeightMatrix(MatrixXd weightMatrix, string path);

    /**
     * takes the raw output from the network
     * and applies an operation element-wise to it
     * @return the network output with the function applied
     */
    VectorXd (*outputFun) (VectorXd);

public:

    //any trained component is best left public

    //the hidden layer of the network
    shared_ptr<LSTMLayer> lstmLayer;

    //weights to the output layer
    MatrixXd outputWeights;

    /**
     * given a ground truth and prediction
     * calculate the cost between the two
     * @return the cost between the truth and network prediction
     */
    double (*costFun) (VectorXd,VectorXd);

    /**
     * constructor sets up network of appropriate size
     * @param inputSize the size of the input data
     * @param hiddenSize the size of the lstm layer output (i.e. h)
     * @param outputSize the size of the output data
     * @param outputFun the output function of the network
     * @param costFun the cost function used when training the network
     */
    LSTMNet(unsigned int inputSize, unsigned int hiddenSize, unsigned int outputSize,
            VectorXd(*outputFun)(VectorXd), double(*costFun)(VectorXd,VectorXd));

    /**
     * alternative constructor reads in the weight matrices over initialising them
     * @param inputSize the size of the network input
     * @param hiddenSize the size of the hidden output
     * @param outputFun the output function for the network
     * @param costFun the cost function to be used during training
     * @param filePrefix the prefix for all weight matrices
     */
    LSTMNet(unsigned int inputSize, unsigned int hiddenSize, VectorXd(*outputFun)(VectorXd),
            double(*costFun)(VectorXd,VectorXd), string filePrefix);

    /**
     * function saves all network weights for future use
     */
    void saveNetwork();

    /**
     * function takes a new input into the network
     * (could be something just output from the network in practice),
     * feeds it through, and then generates the output
     * @param x_t the newly arrived input data
     * @return the new output of the network
     */
    VectorXd predict(VectorXd x_t);

};

#endif //FYP_LSTM_H
