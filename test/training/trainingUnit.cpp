/**
 * file contains unit tests
 * for the lstm version of training
 * Author: Charlie Street
 */

#define CATCH_CONFIG_MAIN

#include "../../include/test/catch.hpp"
#include "../../include/training/readTraining.h"

#define TRAINING_FILE "../test/training/testTraining.csv"

/**
 * test case tests the functionality of reading in from a training data file
 */
TEST_CASE("Test functionality to read in the training data file","[readTraining]") {

    training_set_t trainingSet = readTrainingSet(TRAINING_FILE);

    REQUIRE(trainingSet.size() == 3);
    REQUIRE(trainingSet.at(0).size() == 4);
    REQUIRE(trainingSet.at(1).size() == 4);
    REQUIRE(trainingSet.at(2).size() == 7);


    double duration = 1.1;

    //loop round and check values
    for (auto &i : trainingSet) {
        for(unsigned int j = 0; j < i.size(); j++) {
            if(j % 2 == 0) {
                CHECK(i.at(j)(0,0) == Approx(0.0));
            } else {
                CHECK(i.at(j)(0,0) == Approx(1.0));
            }

            CHECK(i.at(j)(1,0) == Approx(duration));
            duration += 0.1;
        }
    }

    CHECK(duration == Approx(2.6));
}