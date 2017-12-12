/**
 * this file contains a series of correctness tests
 * for the esn architecture
 * no speed considerations are made here, the aim
 * is to check everything functions correctly
 * Author: Charlie Street
 */

#define CATCH_CONFIG_MAIN

#include "../../include/test/catch.hpp"
#include "../../include/esn/esn.h"
#include <cmath>

/**
 * test carried out by checking read(save(w)) = w
 */
TEST_CASE("Check read/write functionality works properly", "[checkpoints]") {

    auto *echo = new ESN(1.0,2.0,3.0,40,5,3,8,nullptr,nullptr); //the function pointers can be ignored here
    echo->saveNetwork();

    MatrixXd inRes = echo->getInRes();
    MatrixXd resRes = echo->getResRes();
    MatrixXd resOut = echo->resOutWeights;

    delete echo;

    auto *echo2 = new ESN("inputReservoirWeights.csv","reservoirReservoirWeights.csv","reservoirOutputWeights.csv",
                        nullptr,nullptr);

    MatrixXd inResRead = echo2->getInRes();
    MatrixXd resResRead = echo2->getResRes();
    MatrixXd resOutRead = echo2->resOutWeights;

    delete echo2;

    //compare the sizes of the matrices before doing any proper checks

    REQUIRE(inRes.cols() == inResRead.cols());
    REQUIRE(inRes.rows() == inResRead.rows());

    REQUIRE(resRes.cols() == resResRead.cols());
    REQUIRE(resRes.rows() == resResRead.rows());

    REQUIRE(resOut.cols() == resOutRead.cols());
    REQUIRE(resOut.rows() == resOutRead.rows());

    //if the sizes match up, we can begin to check for matrix equivalence

    for(int i = 0; i < inRes.rows(); i++) {
        for(int j = 0; j < inRes.cols(); j++) {
            bool test = inRes(i,j) == Approx(inResRead(i,j)).margin(0.0);
            REQUIRE(test);
        }
    }

    for(int i = 0; i < resRes.rows(); i++) {
        for(int j = 0; j < resRes.cols(); j++) {
            bool test = resRes(i,j) == Approx(resResRead(i,j)).margin(0.0);
            REQUIRE(test);
        }
    }

    for(int i = 0; i < resOut.rows(); i++) {
        for(int j = 0; j < resOut.cols(); j++) {
            bool test = resOut(i,j) == Approx(resOutRead(i,j)).margin(0.0);
            REQUIRE(test);
        }
    }


}

/**
 * to be tested here are:
 * checks that setup of matrices is correct
 * weight matrices and reservoir
 */
TEST_CASE("Check network setup has desired effect", "[setup]") {

    double inResWeight = 1.0;
    double resResWeight = 2.0;
    double biResWeight = 3.0;
    int reservoirSize = 40;
    int jumpSize = 5;
    int numInputs = 3;
    int numOutputs = 8;

    auto *echo = new ESN(inResWeight,resResWeight,biResWeight,reservoirSize,jumpSize,
                         numInputs,numOutputs,nullptr,nullptr);

    MatrixXd inResWeights = echo->getInRes();
    MatrixXd resResWeights = echo->getResRes();
    MatrixXd resOutWeights = echo->resOutWeights;
    VectorXd reservoir = echo->getReservoir();

    delete echo; //now I have the matrices I have no need for the object

    //first check the matrix dimensions hold properly

    REQUIRE(inResWeights.rows() == reservoirSize);
    REQUIRE(inResWeights.cols() == numInputs);

    REQUIRE(resResWeights.rows() == reservoirSize);
    REQUIRE(resResWeights.cols() == reservoirSize);

    REQUIRE(resOutWeights.rows() == numOutputs);
    REQUIRE(resOutWeights.cols() == reservoirSize);

    REQUIRE(reservoir.rows() == reservoirSize);
    REQUIRE(reservoir.cols() == 1);

    //now check all of the items in the matrices are as expected

    //check all weights same magnitude, and that approximately 50/50 +/- split
    //the latter check is just printed as it can't be enforced due to nondeterminism

    int plus = 0;
    int minus = 0;

    for (int i = 0; i < inResWeights.rows(); i++) {
        for (int j = 0; j < inResWeights.cols(); j++) {
            bool test = abs(inResWeights(i,j)) == Approx(inResWeight).margin(0.0);
            REQUIRE(test);
            if(inResWeights(i,j) > 0) {
                plus++;
            } else {
                minus++;
            }
        }
    }

    cout << "Plus: " << plus << endl;
    cout << "Minus: " << minus << endl;

    //check weight matrix follows desired structure
    for (int i = 0; i < resResWeights.rows(); i++) {
        for (int j = 0; j < resResWeights.cols(); j++) {
            if (j == ((i+1) % reservoirSize)) { //SCR weights
                bool test = resResWeights(i,j) == Approx(resResWeight).margin(0.0);
                REQUIRE(test);
            } else if ((i % jumpSize == 0) && (j % jumpSize == 0) &&
                    (j == ((i + jumpSize) % reservoirSize) || i == ((j + jumpSize) % reservoirSize)) ) { //bidirectional weights
                bool test = resResWeights(i,j) == Approx(biResWeight).margin(0.0);
                REQUIRE(test);
            } else { //no connection
                bool test = resResWeights(i,j) == Approx(0.0).margin(0.0);
                REQUIRE(test);
            }
        }
    }

    //check weight matrix values fall within expected range
    //this isn't that crucial test, but I'm putting it there for completeness
    for (int i = 0; i < resOutWeights.rows(); i++) {
        for (int j = 0; j < resOutWeights.cols(); j++) {
            bool test = resOutWeights(i,j) >= -1 && resOutWeights(i,j) <= 1;
            REQUIRE(test);
        }
    }

    //check reservoir is initialised correctly
    for (int i = 0; i < reservoir.rows(); i++) {
        bool test = reservoir(i,0) == Approx(0.0).margin(0.0);
        REQUIRE(test);
    }


}

TEST_CASE("Check network update works as expected", "[update]") {

}

TEST_CASE("Check prediction functionality works as expected", "[predict]") {

}