/* file is used to unit test some functions included in the ICA project
 * ICA on its own is not something that can be unit tested really
 * and the audio reading/writing functions will have to be manually tested by me
 * instead I will focus on the functions within ICAHelpers.h/cpp as they can be trivially tested
 * Author: Charlie Street
 */

#define CATCH_CONFIG_MAIN

#include "../include/catch.hpp"
#include "../include/ICAHelpers.h"
#include "../include/ICA.h"
#include "../include/audioFileHelpers.h" //need it for certain testing
#include <Eigen/Dense>

using namespace Eigen;

/* functions for testing:
 *  from ICAHelpers:
 *      subtractMean
 *      addMean
 *      whitenData
 *      unwhitenData
 *  from ICA:
 *      g
 *      gPrime
 *  from main (but copied here because of multiple mains):
 *      fromBuffers
 */


//function takes audio buffers and writes it into an eigen matrix
//to avoid conflicts later with multiple main files, this function is just copied here
//its fairly isolated so copying it isn't that big an issue
Matrix<double,SPEAKERS,Dynamic> fromBuffers(const audioBuffers &track) {

    Matrix<double,SPEAKERS,Dynamic> X = MatrixXd::Zero(2,track.left.size());

    //write into matrix
    for(unsigned int i = 0; i < track.left.size(); i ++) {
        X(0,i) = track.left.at(i);
        X(1,i) = track.right.at(i);
    }

    return X;
}



TEST_CASE("tests the subtractMean function from ICAHelpers","[subtractMean]") {

    //i only need 1 instance of a test, its mostly to check the referencing is done correctly
    Matrix<double,SPEAKERS,Dynamic> subMean = MatrixXd::Zero(SPEAKERS,5);
    subMean(0,0) = 1.0;
    subMean(0,1) = 2.0;
    subMean(0,2) = 3.0;
    subMean(0,3) = 4.0;
    subMean(0,4) = 5.0;

    subMean(1,0) = 6.0;
    subMean(1,1) = 7.0;
    subMean(1,2) = 8.0;
    subMean(1,3) = 9.0;
    subMean(1,4) = 10.0;

    Matrix<double,SPEAKERS,1> m = subtractMean(subMean);

    double test1 = m(0,0);
    CHECK(test1 == 3.0);

    double test2 = m(1,0);
    CHECK(test2 == 8.0);

    double test3 = subMean(0,0);
    CHECK(test3 == -2.0);

    double test4 = subMean(0,1);
    CHECK(test4 == -1.0);

    double test5 = subMean(0,2);
    CHECK(test5 == 0.0);

    double test6 = subMean(0,3);
    CHECK(test6 == 1.0);

    double test7 = subMean(0,4);
    CHECK(test7 == 2.0);

    double test8 = subMean(1,0);
    CHECK(test8 == -2.0);

    double test9 = subMean(1,1);
    CHECK(test9 == -1.0);

    double test10 = subMean(1,2);
    CHECK(test10 == 0.0);

    double test11 = subMean(1,3);
    CHECK(test11 == 1.0);

    double test12 = subMean(1,4);
    CHECK(test12 == 2.0);

    double test13 = subMean.cols();
    CHECK(test13 == 5.0);
}

TEST_CASE("tests the addMean function from ICAHelpers","[addMean]") {
    Matrix<double,SPEAKERS,Dynamic> A = MatrixXd::Zero(2,2);
    A(0,0) = 0.25;
    A(1,1) = 0.25;

    Matrix<double,SPEAKERS,1> m = MatrixXd::Zero(2,1);
    m(0,0) = 2;
    m(1,0) = 4;

    Matrix<double,Dynamic,Dynamic> s = MatrixXd::Zero(2,3);
    s(0,0) = 1.0;
    s(0,1) = 2.0;
    s(0,2) = 3.0;
    s(1,0) = 4.0;
    s(1,1) = 5.0;
    s(1,2) = 6.0;


    addMean(s,A,m);

    //check A and m have not been changed
    double test1 = A.cols();
    CHECK(test1 == 2.0);

    double test2 = A(0,0);
    CHECK(test2 == 0.25);

    double test3 = A(0,1);
    CHECK(test3 == 0.0);

    double test4 = A(1,0);
    CHECK(test4 == 0.0);

    double test5 = A(1,1);
    CHECK(test5 == 0.25);

    double test6 = m(0,0);
    CHECK(test6 == 2.0);

    double test7 = m(1,0);
    CHECK(test7 == 4.0);

    //check s has been changed appropriately
    double test8 = s.rows();
    CHECK(test8 == 2.0);

    double test9 = s.cols();
    CHECK(test9 == 3.0);

    double test10 = s(0,0);
    CHECK(test10 == 9.0);

    double test11 = s(0,1);
    CHECK(test11 == 10.0);

    double test12 = s(0,2);
    CHECK(test12 == 11.0);

    double test13 = s(1,0);
    CHECK(test13 == 20.0);

    double test14 = s(1,1);
    CHECK(test14 == 21.0);

    double test15 = s(1,2);
    CHECK(test15 == 22.0);

}

TEST_CASE("tests the whitenData function from ICAHelpers","[whitenData]") {
    Matrix<double,SPEAKERS,Dynamic> X = MatrixXd::Zero(2,5);
    X(0,0) = -2.0;
    X(0,1) = -1.0;
    X(0,2) = 0.0;
    X(0,3) = 1.0;
    X(0,4) = 2.0;
    X(1,0) = 0.0;
    X(1,1) = -1.0;
    X(1,2) = -2.0;
    X(1,3) = 1.0;
    X(1,4) = 2.0;

    shared_ptr<EigenDecomp> eigen = whitenData(X);

    //all results calculated using Matlab
    Matrix<double,SPEAKERS,SPEAKERS> eigenVecs = eigen->eigenVectors;
    Matrix<double,SPEAKERS,SPEAKERS> eigenVals = eigen->eigenValueMatRoot;

    //biggest eigen values first
    double test1 = eigenVecs(0,0);
    CHECK(test1 == Approx(0.7071));

    double test2 = eigenVecs(0,1);
    CHECK(test2 == Approx(-0.7071));

    double test3 = eigenVecs(1,0);
    CHECK(test3 == Approx(0.7071));

    double test4 = eigenVecs(1,1);
    CHECK(test4 == Approx(0.7071));

    double test5 = eigenVals(0,0);
    CHECK(test5 == Approx(0.5));

    double test6 = eigenVals(0,1);
    CHECK(test6 == 0.0);

    double test7 = eigenVals(1,0);
    CHECK(test7 == 0.0);

    double test8 = eigenVals(1,1);
    CHECK(test8 == Approx(1.0));

    double test9 = X.rows();
    CHECK(test9 == 2.0);

    double test10 = X.cols();
    CHECK(test10 == 5.0);

    double test11 = X(0,0);
    CHECK(test11 == Approx(-1.5));

    double test12 = X(0,1);
    CHECK(test12 == Approx(-0.5));

    double test13 = X(0,2);
    CHECK(test13 == Approx(0.5));

    double test14 = X(0,3);
    CHECK(test14 == Approx(0.5));

    double test15 = X(0,4);
    CHECK(test15 == Approx(1.0));

    double test16 = X(1,0);
    CHECK(test16 == Approx(0.5));

    double test17 = X(1,1);
    CHECK(test17 == Approx(-0.5));

    double test18 = X(1,2);
    CHECK(test18 == Approx(-1.5));

    double test19 = X(1,3);
    CHECK(test19 == Approx(0.5));

    double test20 = X(1,4);
    CHECK(test20 == Approx(1.0));


}

TEST_CASE("tests the unwhitenData function from ICAHelpers","[unwhitenData]") {

    Matrix<double,SPEAKERS,Dynamic> X = MatrixXd::Zero(2,5);
    X(0,0) = -2.0;
    X(0,1) = -1.0;
    X(0,2) = 0.0;
    X(0,3) = 1.0;
    X(0,4) = 2.0;
    X(1,0) = 0.0;
    X(1,1) = -1.0;
    X(1,2) = -2.0;
    X(1,3) = 1.0;
    X(1,4) = 2.0;

    shared_ptr<EigenDecomp> eigen = whitenData(X);

    //test is to whiten and unwhiten the data and see if we end up with the original data back
    unwhitenData(X,*eigen);

    double test1 = X.rows();
    CHECK(test1 == 2.0);

    double test2 = X.cols();
    CHECK(test2 == 5.0);

    double test3 = X(0,0);
    CHECK(test3 == Approx(-2.0));

    double test4 = X(0,1);
    CHECK(test4 == Approx(-1.0));

    double test5 = X(0,2);
    CHECK(test5 == Approx(0.0));

    double test6 = X(0,3);
    CHECK(test6 == Approx(1.0));

    double test7 = X(0,4);
    CHECK(test7 == Approx(2.0));

    double test8 = X(1,0);
    CHECK(test8 == Approx(0.0));

    double test9 = X(1,1);
    CHECK(test9 == Approx(-1.0));

    double test10 = X(1,2);
    CHECK(test10 == Approx(-2.0));

    double test11 = X(1,3);
    CHECK(test11 == Approx(1.0));

    double test12 = X(1,4);
    CHECK(test12 == Approx(2.0));
}

TEST_CASE("tests the g function from ICA","[g]") {
    Matrix<double,1,Dynamic> vec = MatrixXd::Zero(1,5);

    vec(0,0) = 1.0;
    vec(0,1) = 2.0;
    vec(0,2) = 3.0;
    vec(0,3) = 4.0;
    vec(0,4) = 5.0;


    Matrix<double,1,Dynamic> result = g(vec);

    //checking original vector hasn't changed
    double test1 = vec.cols();
    CHECK(test1 == 5.0);

    double test2 = vec(0,0);
    CHECK(test2 == 1.0);

    double test3 = vec(0,1);
    CHECK(test3 == 2.0);

    double test4 = vec(0,2);
    CHECK(test4 == 3.0);

    double test5 = vec(0,3);
    CHECK(test5 == 4.0);

    double test6 = vec(0,4);
    CHECK(test6 == 5.0);

    double test7 = result.cols();
    CHECK(test7 == 5.0);

    //done by hand with a calculator

    double test8 = result(0,0);
    CHECK(test8 == Approx(0.6065306597));

    double test9 = result(0,1);
    CHECK(test9 == Approx(0.2706705665));

    double test10 = result(0,2);
    CHECK(test10 == Approx(0.03332698961));

    double test11 = result(0,3);
    CHECK(test11 == Approx(0.001341850512));

    double test12 = result(0,4);
    CHECK(test12 == Approx(0.00001863326586));

}

TEST_CASE("tests the gPrime function from ICA","[gPrime]") {
    Matrix<double,1,Dynamic> vec = MatrixXd::Zero(1,5);

    vec(0,0) = 1.0;
    vec(0,1) = 2.0;
    vec(0,2) = 3.0;
    vec(0,3) = 4.0;
    vec(0,4) = 5.0;

    Matrix<double,1,Dynamic> result = gPrime(vec);

    //checking original vector hasn't changed
    double test1 = vec.cols();
    CHECK(test1 == 5.0);

    double test2 = vec(0,0);
    CHECK(test2 == 1.0);

    double test3 = vec(0,1);
    CHECK(test3 == 2.0);

    double test4 = vec(0,2);
    CHECK(test4 == 3.0);

    double test5 = vec(0,3);
    CHECK(test5 == 4.0);

    double test6 = vec(0,4);
    CHECK(test6 == 5.0);

    double test7 = result.cols();
    CHECK(test7 == 5.0);

    //done by hand with a calculator

    double test8 = result(0,0);
    CHECK(test8 == Approx(0.0));

    double test9 = result(0,1);
    CHECK(test9 == Approx(-0.4060058497));

    double test10 = result(0,2);
    CHECK(test10 == Approx(-0.08887197231));

    double test11 = result(0,3);
    CHECK(test11 == Approx(-0.005031939419));

    double test12 = result(0,4);
    CHECK(test12 == Approx(-0.00008943967613));

}


TEST_CASE("tests the fromBuffers function from main","[fromBuffers]") {
    vector<double> left = {1.0, 2.0, 3.0, 4.0, 5.0};
    vector<double> right = {6.0, 7.0, 8.0, 9.0, 10.0};

    audioBuffers track(left, right);

    Matrix<double,SPEAKERS,Dynamic> fromBuf = fromBuffers(track);

    double test1 = fromBuf.cols();
    CHECK(test1 == 5.0);

    double test2 = fromBuf(0,0);
    CHECK(test2 == 1.0);

    double test3 = fromBuf(0,1);
    CHECK(test3 == 2.0);

    double test4 = fromBuf(0,2);
    CHECK(test4 == 3.0);

    double test5 = fromBuf(0,3);
    CHECK(test5 == 4.0);

    double test6 = fromBuf(0,4);
    CHECK(test6 == 5.0);

    double test7 = fromBuf(1,0);
    CHECK(test7 == 6.0);

    double test8 = fromBuf(1,1);
    CHECK(test8 == 7.0);

    double test9 = fromBuf(1,2);
    CHECK(test9 == 8.0);

    double test10 = fromBuf(1,3);
    CHECK(test10 == 9.0);

    double test11 = fromBuf(1,4);
    CHECK(test11 == 10.0);
}