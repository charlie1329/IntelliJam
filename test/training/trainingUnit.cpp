/**
 * this file carries out unit tests for functionality within the
 * training part of the system functionality
 * Author: Charlie Street
 */

#define CATCH_CONFIG_MAIN
#define FILE_PATH "../test/training/testFile.csv" //for testing file reading functionality

#include "../../include/test/catch.hpp"
#include "../../include/training/fileToEcho.h"

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