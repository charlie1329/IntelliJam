/**
 * file implements cost functions from esn_costs.h
 * Author: Charlie Street
 */

#include "../../include/esn/esn_costs.h"
/**
 * implemented from esn_costs.h
 * @param gt the ground truth vector
 * @param prediction the prediction vector
 * @return the average lse between the two vectors
 */
double lse(VectorXd gt, VectorXd prediction) {
    double sumError = 0.0;

    for(int i = 0; i < gt.rows(); i++) {
        if(prediction(i,0) < 24 || prediction(i,0) > 79) {
            sumError += 6000;
        }
        sumError += pow(gt(i,0) - prediction(i,0),2);
    }

    return sumError/gt.rows();
}

/**
 * a smarter cost function using domain information about music
 * to calculate the cost between two predictions
 * @param gt the ground truth vector
 * @param prediction the prediction vector
 * @return the sum of the interval cost between the two vectors
 */
double intervalCost(VectorXd gt, VectorXd prediction) {

    double error = 0.0;
    for(int i = 0; i < gt.rows(); i++) {
        auto newGt = static_cast<int>(gt(i, 0));
        auto newPred = static_cast<int>(prediction(i,0));

        if(newPred > 79 || newPred < 24) {
            error += 200; //high penalty
            continue;
        }

        if(newGt > newPred) {
            int noteDiff = (newGt % 12) - (newPred % 12);
            if(noteDiff < 0) noteDiff += 12;
            error += intervalArr[noteDiff] + 2*((newGt - newPred)/12);
        } else {
            int noteDiff = (newPred % 12) - (newGt % 12);
            if(noteDiff < 0) noteDiff += 12;
            error += intervalArr[noteDiff] + 2*((newPred - newGt)/12);
        }
    }

    return error;

}