/**
 * The purpose of this file is to examine the speed of execution of the ESN
 * The most efficiency critical function is the update function of the ESN
 * But I will also test the predict function for completeness
 * Author: Charlie Street
 */

#include <iostream>
#include <chrono> //I want execution timers!!!
#include "../../include/esn/esn.h"

/**
 * carry out the tests
 * @param argc we all know this by now...
 * @param argv ^^^
 * @return 0
 */
int main(int argc, char **argv) {

    //the time of set up doesn't bother me
    //in practice it will happen once at the start of the system
    auto *echo = new ESN(0.5,0.7,0.3,200,10,10,8,nullptr,nullptr);

    //create some arbitrary input vector of correct length
    //randomly initialise between -1 and 1
    //for these experiments, the same vector is being input each time
    //hopefully this doesn't cause any issues
    //in practice, this will be audio information
    VectorXd inputVec = MatrixXd::Random(10,1);

    //note that dynamically sized matrices may cause some slight speed decreases (I think)
    //if this happens to be the case, then once the network is optimised, I can bake in these sizes
    //such that memory is allocated at run-time

    //carry out 200 updates
    //if i used FFT input, this is larger than the sample rate

    auto start = chrono::high_resolution_clock::now(); //start timer
    for(int i = 0; i < 200; i++) {
        echo->updateReservoir(0.5);
    }
    auto finish = chrono::high_resolution_clock::now();

    //calculate elapsed time
    chrono::duration<double> elapsed = finish - start;
    cout << "Elapsed Time For Updates: " << elapsed.count() << " (s)" << endl;

    //make a prediction based on the network

    start = chrono::high_resolution_clock::now();
    echo->predict();
    finish = chrono::high_resolution_clock::now();

    elapsed = finish - start;
    cout << "Elapsed Time For Prediction: " << elapsed.count() << " (s)" << endl;

    delete echo; //finished with the object now

    return 0;
}