/**
 * this file carries out unit tests for functionality within the
 * training part of the system functionality
 * this also includes the testing of the musical cost function
 * Author: Charlie Street
 */

#define CATCH_CONFIG_MAIN
#define FILE_PATH "../test/training_old/testFile.csv" //for testing file reading functionality
#define TEST_LOG "../test/training_old/testLog.txt" //for testing checkpoint functionality
#define TRAINING_SAMPLE "../test/training_old/testTraining.csv" //for testing formTrainingSet functionality

#include "../../include/test/catch.hpp"
#include "../../include/training_old/fileToEcho.h"
#include "../../include/training_old/checkpoint.h"
#include "../../include/training_old/trainNetwork.h"
#include "../../include/esn/esn_costs.h"
#include "../../include/training_old/simulated_annealing.h"

//TODO: Test rest of functionality!!!

/**
 * tests the functionality of the function readTrainingFile
 * by using a sample file found in the directory alongside this test file
 */
TEST_CASE("Tests the readTrainingFile functionality","[readTrainingFile]") {
    vector<pair<string,VectorXd>> namesAndTruth = readTrainingFile(FILE_PATH,8);

    REQUIRE(namesAndTruth.size() == 3);

    pair<string,VectorXd> firstItem = namesAndTruth.at(0);
    pair<string,VectorXd> secondItem = namesAndTruth.at(1);
    pair<string,VectorXd> thirdItem = namesAndTruth.at(2);

    //check file names came through correctly
    CHECK(firstItem.first == "fileNo1.wav");
    CHECK(secondItem.first == "fileNo2.wav");
    CHECK(thirdItem.first == "fileNo3.wav");

    for(unsigned int i = 0; i < 8; i++) {

        CHECK(((int)firstItem.second(i,0)) == (i + 1));
        CHECK(((int)secondItem.second(i,0)) == (i + 11));
        CHECK(((int)thirdItem.second(i,0)) == (i + 21));
    }
}

/**
 * tests the functionality of the checkpoint system
 * by using a sample file found in the directory alongside this test file
 */
TEST_CASE("Tests checkpoint functionality","[checkpoint]") {
    vector<vector<double>> combos;

    vector<double> combo1;
    combo1.push_back(0.05);
    combo1.push_back(0.06);
    combo1.push_back(0.07);
    combo1.push_back(200);
    combo1.push_back(30);
    combo1.push_back(50);

    vector<double> combo2;
    combo2.push_back(0.1);
    combo2.push_back(2.0);
    combo2.push_back(3.5);
    combo2.push_back(100);
    combo2.push_back(2);
    combo2.push_back(50);

    vector<double> combo3;
    combo3.push_back(0.05);
    combo3.push_back(0.07);
    combo3.push_back(0.07);
    combo3.push_back(200);
    combo3.push_back(30);
    combo3.push_back(50);

    vector<double> combo4;
    combo4.push_back(0.06);
    combo4.push_back(0.07);
    combo4.push_back(0.08);
    combo4.push_back(250);
    combo4.push_back(45);
    combo4.push_back(60);

    vector<double> combo5;
    combo5.push_back(0.2);
    combo5.push_back(0.2);
    combo5.push_back(0.2);
    combo5.push_back(1000);
    combo5.push_back(7);
    combo5.push_back(60);

    vector<double> combo6;
    combo6.push_back(0.07);
    combo6.push_back(0.08);
    combo6.push_back(0.09);
    combo6.push_back(300);
    combo6.push_back(50);
    combo6.push_back(70);


    combos.push_back(combo1);
    combos.push_back(combo2);
    combos.push_back(combo3);
    combos.push_back(combo4);
    combos.push_back(combo5);
    combos.push_back(combo6);

    vector<pair<vector<double>,bool>> taggedCombos = findCompleted(combos,TEST_LOG);

    //check vector correct length
    REQUIRE(taggedCombos.size() == 6);

    unsigned int i = 0;
    //check combos remain unchanged
    for(auto &combo: taggedCombos) {
        REQUIRE(combos.at(i).size() == 6);
        REQUIRE(combo.first.size() == 6);
        for(int j = 0; j < combos.at(i).size(); j++) {
            CHECK(combos.at(i).at(j) == Approx(combo.first.at(j)));
        }
        i++;
    }

    vector<bool> completeVals = {true, false, false, true, false, true};

    //check bool set correctly
    for(unsigned int i = 0; i < taggedCombos.size(); i++) {
        CHECK(taggedCombos.at(i).second == completeVals.at(i));
    }


}

/**
 * test case takes a simple test problem and checks the ridge
 * regression algorithm works fine with it
 */
TEST_CASE("Tests functionality of ridge regression","[ridge]") {

    //set up the echo state network
    shared_ptr<ESN> echo = std::make_shared<ESN>(1.0,0.9,0.4,2,13,1,1,nullptr,nullptr);

    VectorXd sample1(2);
    VectorXd sample2(2);
    VectorXd gt1(1);
    VectorXd gt2(1);

    sample1(0,0) = 1;
    sample1(1,0) = 0;

    sample2(0,0) = 0;
    sample2(1,0) = 1;

    gt1(0,0) = 1;

    gt2(0,0) = 0;

    training_set_t trainingSet;

    trainingSet.emplace_back(sample1,gt1);
    trainingSet.emplace_back(sample2,gt2);

    trainNetwork(echo,trainingSet,0);

    CHECK(echo->resOutWeights(0,0) == Approx(0.99502));
    CHECK(echo->resOutWeights(0,1) == Approx(0.0));
}

/**
 * test that formTrainingSet performs as intended
 */
TEST_CASE("Tests formTrainingSet works correctly","[formTrainingSet]") {

    //initialise the echo state network
    shared_ptr<ESN> echo = std::make_shared<ESN>(1.0,0.9,0.4,200,13,1,8,nullptr,nullptr);

    //I don't want the state of the reservoir to have changed, this is of crucial importance
    VectorXd initialReservoir = echo->getReservoir();

    //form the training set
    shared_ptr<training_set_t> trainingSet = formTrainingSet(echo,TRAINING_SAMPLE,10);

    //check no duplication of training items
    REQUIRE(trainingSet->size() == 4);

    //check reservoir state not changed (as copies sent to each worker)
    REQUIRE(initialReservoir.isApprox(echo->getReservoir()));

    //now check each new reservoir does show some difference to the initial one or nothing useful is done
    for (auto &i : *trainingSet) {
        CHECK(i.first.rows() == 200);
        CHECK(!initialReservoir.isApprox(i.first));
    }

    //also check each reservoir is different to each other
    for(int i = 0; i < trainingSet->size(); i++) {
        for(int j = 0; j < trainingSet->size(); j++) {
            if(i != j) {
                CHECK(!(trainingSet->at(i)).first.isApprox((trainingSet->at(j)).first));
            }
        }
    }

    //now find the correct order of the vectors in the training set and check they are all correct.
    bool firstFound = false;
    bool secondFound = false;
    bool thirdFound = false;
    bool fourthFound = false;

    for(auto &i : *trainingSet) {

        VectorXd gt = i.second;
        REQUIRE(gt.rows() == 8);

        if(gt(0,0) == 79 && !firstFound) {
            CHECK(gt(0,0) == 79);
            CHECK(gt(1,0) == 72);
            CHECK(gt(2,0) == 79);
            CHECK(gt(3,0) == 60);
            CHECK(gt(4,0) == 60);
            CHECK(gt(5,0) == 60);
            CHECK(gt(6,0) == 48);
            CHECK(gt(7,0) == 49);
            firstFound = true;
        } else if(gt(0,0) == 56 && !secondFound) {
            CHECK(gt(0,0) == 56);
            CHECK(gt(1,0) == 47);
            CHECK(gt(2,0) == 51);
            CHECK(gt(3,0) == 53);
            CHECK(gt(4,0) == 56);
            CHECK(gt(5,0) == 56);
            CHECK(gt(6,0) == 58);
            CHECK(gt(7,0) == 56);
            secondFound = true;
        } else if(gt(0,0) == 47 && !thirdFound) {
            CHECK(gt(0,0) == 47);
            CHECK(gt(1,0) == 53);
            CHECK(gt(2,0) == 55);
            CHECK(gt(3,0) == 47);
            CHECK(gt(4,0) == 46);
            CHECK(gt(5,0) == 47);
            CHECK(gt(6,0) == 46);
            CHECK(gt(7,0) == 47);
            thirdFound = true;
        } else if(gt(0,0) == 55 && !fourthFound) {
            CHECK(gt(0,0) == 55);
            CHECK(gt(1,0) == 47);
            CHECK(gt(2,0) == 46);
            CHECK(gt(3,0) == 47);
            CHECK(gt(4,0) == 47);
            CHECK(gt(5,0) == 48);
            CHECK(gt(6,0) == 55);
            CHECK(gt(7,0) == 47);
            fourthFound = true;
        }
    }

    //check all 4 training items were successfully included in the training set
    bool allFound = firstFound && secondFound && thirdFound && fourthFound;
    REQUIRE(allFound);
}

/**
 * this test case covers the testing of the interval cost function to test things are correct
 */
TEST_CASE("Tests functionality of intervalCost","[intervalCost]") {

    VectorXd gt(1);
    VectorXd pred(1);

    gt(0,0) = 39;
    pred(0,0) = 24;
    CHECK(intervalCost(gt,pred) == Approx(6));

    gt(0,0) = 27;
    pred(0,0) = 24;
    CHECK(intervalCost(gt,pred) == Approx(5));

    gt(0,0) = 27;
    pred(0,0) = 36;
    CHECK(intervalCost(gt,pred) == Approx(3));

    gt(0,0) = 27;
    pred(0,0) = 48;
    CHECK(intervalCost(gt,pred) == Approx(4));

    gt(0,0) = 27;
    pred(0,0) = 60;
    CHECK(intervalCost(gt,pred) == Approx(5));

    gt(0,0) = 24;
    pred(0,0) = 27;
    CHECK(intervalCost(gt,pred) == Approx(5));

    gt(0,0) = 24;
    pred(0,0) = 39;
    CHECK(intervalCost(gt,pred) == Approx(6));

    gt(0,0) = 36;
    pred(0,0) = 27;
    CHECK(intervalCost(gt,pred) == Approx(3));

    gt(0,0) = 48;
    pred(0,0) = 27;
    CHECK(intervalCost(gt,pred) == Approx(4));

    gt(0,0) = 37;
    pred(0,0) = 25;
    CHECK(intervalCost(gt,pred) == Approx(1));

    gt(0,0) = 25;
    pred(0,0) = 27;
    CHECK(intervalCost(gt,pred) == Approx(7));

    gt(0,0) = 25;
    pred(0,0) = 39;
    CHECK(intervalCost(gt,pred) == Approx(8));

    gt(0,0) = 37;
    pred(0,0) = 27;
    CHECK(intervalCost(gt,pred) == Approx(8));

    gt(0,0) = 49;
    pred(0,0) = 27;
    CHECK(intervalCost(gt,pred) == Approx(9));

    gt(0,0) = 25;
    pred(0,0) = 37;
    CHECK(intervalCost(gt,pred) == Approx(1));

    gt(0,0) = 73;
    pred(0,0) = 42;
    CHECK(intervalCost(gt,pred) == Approx(3));

}

/**
 * test case examines certain functions of the simulated annealing part of the code base
 */
TEST_CASE("Tests simulated annealing functions","[simulatedAnnealing]") {

    //test temperature schedule function
    CHECK(temperatureSchedule(100001) == Approx(0.0));
    CHECK(temperatureSchedule(0) == Approx(1.0));
    CHECK(temperatureSchedule(1) == Approx(0.9999));
    CHECK(temperatureSchedule(10) == Approx(0.99900044988));


    //test neighbourhood generation function
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(-SIGMA,SIGMA);

    MatrixXd initMat = MatrixXd::Random(8,200);

    //MatrixXd newMat = generateNeighbour(initMat,generator,distribution,0);

    //CHECK(initMat.isApprox(newMat)); //T was 0 so shouldn't have any affect

    MatrixXd newMat2 = generateNeighbour(initMat,generator,distribution);
    CHECK(!initMat.isApprox(newMat2)); //should actually be changed this time

    MatrixXd newMat3 = generateNeighbour(initMat,generator,distribution);
    CHECK(!newMat3.isApprox(newMat2));

}