/**
 * file implements member functions
 * for the classes LSTMLayer and LSTMNet
 * defined in lstm.h
 * Author: Charlie Street
 */

#include <utility>
#include <vector>
#include <fstream>
#include <iomanip>

#include "../../include/lstm/lstm.h"
#include "../../include/lstm/auxillary_functions.h"


//****LSTMLayer Functions****

/**
 * implemented from lstm.h
 * randomly initialises weight matrix (by reference)
 * @param mat the matrix to be initialised
 * @param rows the number of rows in the matrix
 * @param cols the number of columns in the matric
 */
void LSTMLayer::initialiseWeightMatrix(MatrixXd &mat, unsigned int rows, unsigned int cols) {
    mat = MatrixXd::Zero(rows,cols);

    //initialise random generator with seed based on timestamp
    std::default_random_engine generator;
    generator.seed((unsigned int)chrono::system_clock::now().time_since_epoch().count());

    //create a normal distribution for initialisation
    std::normal_distribution<double> distribution(0.0,INITIAL_STD_DEV);

    //randomly initialise each matrix item
    for(int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat(i,j) = distribution(generator);
        }
    }
}

/**
 * a slightly different function to initialiseWeightMatrix
 * a different function is used in case I decide to initialise
 * these vector differently at some point
 * @param b the vector to be initialised
 * @param rows the rows in the vector b
 */
void LSTMLayer::initialiseBiasVector(VectorXd &b, unsigned int rows) {
    b = VectorXd::Zero(rows);

    std::default_random_engine generator;
    generator.seed((unsigned int)chrono::system_clock::now().time_since_epoch().count());

    //set up the distribution
    std::normal_distribution<double> distribution(0.0,INITIAL_STD_DEV);

    //initialise each value in the vector
    for(int i = 0; i < rows; i++) {
        b(i,0) = distribution(generator);
    }



}

/**
 * function resets the state of the lstm 'cell'
 * represented by h and C
 */
void LSTMLayer::resetState() {


    //reset all values of h and C to 0.0 (for now)
    for(int i = 0; i < h.rows(); i++) {
        h(i,0) = 0.0;
        C(i,0) = 0.0;
    }
}

/**
 * constructor sets up the state of the lstm layer
 * @param inputSize the size of the input data
 * @param hiddenOutputSize the desired size of the output from the hidden layer
 */
LSTMLayer::LSTMLayer(unsigned int inputSize, unsigned int hiddenOutputSize) {

    resetState(); //reset/initialise the internal state of the lstm layer

    //initialise input weight matrices
    initialiseWeightMatrix(theta_xi,hiddenOutputSize,inputSize);
    initialiseWeightMatrix(theta_xf,hiddenOutputSize,inputSize);
    initialiseWeightMatrix(theta_xo,hiddenOutputSize,inputSize);
    initialiseWeightMatrix(theta_xg,hiddenOutputSize,inputSize);

    //initialise recurrent weight matrices
    initialiseWeightMatrix(theta_hi,hiddenOutputSize,hiddenOutputSize);
    initialiseWeightMatrix(theta_hf,hiddenOutputSize,hiddenOutputSize);
    initialiseWeightMatrix(theta_ho,hiddenOutputSize,hiddenOutputSize);
    initialiseWeightMatrix(theta_hg,hiddenOutputSize,hiddenOutputSize);

    //initialise bias vectors
    initialiseBiasVector(bias_i,hiddenOutputSize);
    initialiseBiasVector(bias_f,hiddenOutputSize);
    initialiseBiasVector(bias_o,hiddenOutputSize);
    initialiseBiasVector(bias_g,hiddenOutputSize);

}

/**
 * implemented from lstm.h
 * function takes a new input to the layer
 * and uses it to update the hidden state of the lstm 'cell'
 * @param x_t the new input into the hidden layer
 * @return the new hidden state h (also stored internally)
 */
VectorXd LSTMLayer::update(VectorXd x_t) {


    //input gate calculations
    VectorXd i_t = (theta_xi * x_t) + (theta_hi * h) + bias_i;
    i_t = applyToAll(sigmoid,i_t);

    //forget gate calculations
    VectorXd f_t = (theta_xf * x_t) + (theta_hf * h) + bias_f;
    f_t = applyToAll(sigmoid,f_t);

    //output gate calculations
    VectorXd o_t = (theta_xo * x_t) + (theta_ho * h) + bias_o;
    o_t = applyToAll(sigmoid,o_t);

    //g calculations
    VectorXd g_t = (theta_xg * x_t) + (theta_hg * h) + bias_g;
    g_t = applyToAll(tanh,g_t);

    //update cell state C value
    C = f_t.cwiseProduct(C) + i_t.cwiseProduct(g_t);

    //update hidden output state
    h = o_t.cwiseProduct(applyToAll(tanh,C));

    return h;

}

//****LSTMNet Functions****

/**
 * implemented from lstm.h
 * function initialises a weight matrix appropriately
 * used randomness better than Eigen's default
 * @param mat the matrix to be initialised (by reference)
 * @param rows the rows of the matrix
 * @param cols the columns of the matrix
 */
void LSTMNet::initialiseWeightMatrix(MatrixXd &mat, int rows, int cols) {

    //allocate space for the matrix
    mat = MatrixXd::Zero(rows,cols);

    //initialise random generator with a timestamped seed
    default_random_engine generator;
    generator.seed((unsigned int)chrono::system_clock::now().time_since_epoch().count());

    //create the normal distribution used to initialise the matrix
    normal_distribution<double> distribution(0.0,INITIAL_STD_DEV);

    //actually place the values in the matrix
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            mat(i,j) = distribution(generator);
        }
    }
}

/**
 * implemented from lstm.h
 * function takes a file path (to a csv file)
 * and reads the matrix in that file into an Eigen matrix
 * @param path the file path
 * @return the matrix represented in that file
 */
MatrixXd LSTMNet::readWeightMatrix(string path) {
    vector<vector<double>> temp;

    ifstream matFile(path);
    string item;

    vector<double> currentVector;

    //read item by item
    while(getline(matFile,item,',')) {

        if(item == "\n") { //exit condition, item only has a newline
            temp.push_back(currentVector);
            currentVector.clear();
            break;
        }

        if(item.find('\n') != string::npos) { //if we reach a new line
            temp.push_back(currentVector);
            currentVector.clear();
        }

        double currentVal = stod(item,nullptr);
        currentVector.push_back(currentVal);
    }

    //now put into an eigen matrix

    MatrixXd newMat = MatrixXd::Random(temp.size(), temp.at(0).size());

    for(unsigned int i = 0; i < temp.size(); i++) {
        for(unsigned int j = 0; j < temp.at(0).size(); j++) {
            newMat(i,j) = temp.at(i).at(j);
        }
    }

    return newMat;
}

/**
 * implemented from lstm.h
 * file takes an Eigen matrix and writes it to file
 * @param weightMatrix the matrix to be written
 * @param path the path to the write the matrix too
 */
void LSTMNet::writeWeightMatrix(MatrixXd weightMatrix, string path) {

    //first, open the file
    ofstream csvFile;
    csvFile.open(path);

    for(int i = 0; i < weightMatrix.rows(); i++) {
        for(int j = 0; j < weightMatrix.cols(); j++) { //we want to retain the level of precision

            if(!csvFile.is_open()) return; //a small level of error checking
            csvFile << setprecision(numeric_limits<double>::digits10 + 1) << weightMatrix(i,j);

            if(!csvFile.is_open()) return;
            csvFile << ",";
        }

        if(!csvFile.is_open()) return;
        csvFile << "\n";
    }

    csvFile.close();
}


/**
 * implemented from lstm.h
 * constructor sets up network of appropriate size
 * @param inputSize the size of the input data
 * @param hiddenSize the size of the lstm layer output (i.e. h)
 * @param outputSize the size of the output data (likely the same as the inputSize)
 * @param outputFun the output function of the network
 * @param costFun the cost function used when training the network
 */
LSTMNet::LSTMNet(unsigned int inputSize, unsigned int hiddenSize, unsigned int outputSize,
                 VectorXd(*out)(VectorXd), double(*cost)(VectorXd,VectorXd)) {

    //initialise functions
    outputFun = out;
    costFun = cost;

    //set up the lstm layer
    lstmLayer = std::make_shared<LSTMLayer>(inputSize,hiddenSize);

    //initialise the output weight matrix
    initialiseWeightMatrix(outputWeights,outputSize,hiddenSize);
}

/**
 * alternative constructor reads in the weight matrices over initialising them
 * @param inputSize the size of the network input
 * @param hiddenSize the size of the hidden output
 * @param out the output function for the network
 * @param cost the cost function to be used during training
 * @param filePrefix the prefix for all weight matrices
 */
LSTMNet::LSTMNet(unsigned int inputSize, unsigned int hiddenSize, VectorXd(*out)(VectorXd),
                 double(*cost)(VectorXd,VectorXd), string filePrefix) {

    //initialise functions
    outputFun = out;
    costFun = cost;

    //set up the lstm layer
    lstmLayer = std::make_shared<LSTMLayer>(inputSize,hiddenSize);

    //now read in all the matrices
    lstmLayer->theta_xi = readWeightMatrix(filePrefix + "theta_xi.csv");
    lstmLayer->theta_xf = readWeightMatrix(filePrefix + "theta_xf.csv");
    lstmLayer->theta_xo = readWeightMatrix(filePrefix + "theta_xo.csv");
    lstmLayer->theta_xg = readWeightMatrix(filePrefix + "theta_xg.csv");

    lstmLayer->bias_i = readWeightMatrix(filePrefix + "bias_i.csv");
    lstmLayer->bias_f = readWeightMatrix(filePrefix + "bias_f.csv");
    lstmLayer->bias_o = readWeightMatrix(filePrefix + "bias_o.csv");
    lstmLayer->bias_g = readWeightMatrix(filePrefix + "bias_g.csv");

    lstmLayer->theta_hi = readWeightMatrix(filePrefix + "theta_hi.csv");
    lstmLayer->theta_hf = readWeightMatrix(filePrefix + "theta_hf.csv");
    lstmLayer->theta_ho = readWeightMatrix(filePrefix + "theta_ho.csv");
    lstmLayer->theta_hg = readWeightMatrix(filePrefix + "theta_hg.csv");

    outputWeights = readWeightMatrix(filePrefix + "outputWeights.csv");

}


/**
 * implemented from lstm.h
 * function saves all network weights for future use
 */
void LSTMNet::saveNetwork() {
    string filePrefix = "lstmWeightMatrix_";

    //whole function is really just a load of calls to writeWeightMatrix

    writeWeightMatrix(lstmLayer->theta_xi,filePrefix + "theta_xi.csv");
    writeWeightMatrix(lstmLayer->theta_xf,filePrefix + "theta_xf.csv");
    writeWeightMatrix(lstmLayer->theta_xo,filePrefix + "theta_xo.csv");
    writeWeightMatrix(lstmLayer->theta_xg,filePrefix + "theta_xg.csv");

    writeWeightMatrix(lstmLayer->bias_i,filePrefix + "bias_i.csv");
    writeWeightMatrix(lstmLayer->bias_f,filePrefix + "bias_f.csv");
    writeWeightMatrix(lstmLayer->bias_o,filePrefix + "bias_o.csv");
    writeWeightMatrix(lstmLayer->bias_g,filePrefix + "bias_g.csv");

    writeWeightMatrix(lstmLayer->theta_hi,filePrefix + "theta_hi.csv");
    writeWeightMatrix(lstmLayer->theta_hf,filePrefix + "theta_hf.csv");
    writeWeightMatrix(lstmLayer->theta_ho,filePrefix + "theta_ho.csv");
    writeWeightMatrix(lstmLayer->theta_hg,filePrefix + "theta_hg.csv");

    writeWeightMatrix(outputWeights,filePrefix + "outputWeights.csv");
}

/**
 * function takes a new input into the network
 * (could be something just output from the network in practice),
 * feeds it through, and then generates the output
 * @param x_t the newly arrived input data
 * @return the new output of the network
 */
VectorXd LSTMNet::predict(VectorXd x_t) {
    VectorXd output =  outputWeights * lstmLayer->update(std::move(x_t));

    //if an output function provided, then use it!
    if(outputFun != nullptr) output = outputFun(output);

    return output;
}