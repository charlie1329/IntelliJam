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

TEST_CASE("Check network setup has desired effect", "[setup]") {

}

TEST_CASE("Check network update works as expected", "[update]") {

}

TEST_CASE("Check prediction functionality works as expected", "[predict]") {

}