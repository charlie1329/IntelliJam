/**
 * file implements function prototypes
 * from errorCalculation.h
 * Author: Charlie Street
 */

#include "../../include/training/errorCalculation.h"

/**
 * implemented from errorCalculation.h
 * function calculates total error on lstm network
 * over a particular set of samples
 * @param lstm the network being tested
 * @param samples the samples to be used for testing
 * @return the average error over all samples
 */
double getError(shared_ptr<LSTMNet> lstm, training_set_t samples) {

    double error = 0.0;

    for(unsigned int i = 0; i < samples.size(); i++) {

        lstm->lstmLayer->resetState(); //reset internal state of network

        for(unsigned int j = 0; j < samples.at(i).size()-1; j++) {

            VectorXd prediction = lstm->predict(samples.at(i).at(j));
            VectorXd gt = samples.at(i).at(j+1);

            error += lstm->costFun(gt,prediction);
        }

    }

    return error/samples.size();
}