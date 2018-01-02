/**
 * this file carries out unit tests for functionality within the
 * training part of the system functionality
 * Author: Charlie Street
 */

#define CATCH_CONFIG_MAIN
#define FILE_PATH "../test/training/testFile.csv" //for testing file reading functionality
#define TEST_LOG "../test/training/testLog.txt" //for testing checkpoint functionality

#include "../../include/test/catch.hpp"
#include "../../include/training/fileToEcho.h"
#include "../../include/training/checkpoint.h"

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