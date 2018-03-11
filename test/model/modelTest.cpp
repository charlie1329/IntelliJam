/**
 * file tests the functionality within the model directory
 * Author: Charlie Street
 */

#define CATCH_CONFIG_MAIN

#include "../../include/test/catch.hpp"
#include "../../include/model/keyDetect.h"
#include "../../include/model/fpm.h"
#include <iostream>
/**
 * tests that transposition is carried out properly
 */
TEST_CASE("Tests the transposing function", "[Transpose]") {
    vector<int> GPhrase = {11,3,6,10,6,3,11,0};
    vector<int> CPhrase = transpose(GPhrase,"G","C");
    REQUIRE(CPhrase.size() == 8);
    CHECK(CPhrase.at(0) == 4);
    CHECK(CPhrase.at(1) == 8);
    CHECK(CPhrase.at(2) == 11);
    CHECK(CPhrase.at(3) == 3);
    CHECK(CPhrase.at(4) == 11);
    CHECK(CPhrase.at(5) == 8);
    CHECK(CPhrase.at(6) == 4);
    CHECK(CPhrase.at(7) == 0);

    vector<int> GBack = transpose(CPhrase,"C","G");
    REQUIRE(GBack.size() == 8);
    CHECK(GBack.at(0) == 11);
    CHECK(GBack.at(1) == 3);
    CHECK(GBack.at(2) == 6);
    CHECK(GBack.at(3) == 10);
    CHECK(GBack.at(4) == 6);
    CHECK(GBack.at(5) == 3);
    CHECK(GBack.at(6) == 11);
    CHECK(GBack.at(7) == 0);
}

TEST_CASE("Tests the Key Detection functionality", "[Key Detection]") {
    vector<pair<int,double>> sequence;
    sequence.emplace_back(4,0.371519);
    sequence.emplace_back(6,0.371519);
    sequence.emplace_back(8,0.18576);
    sequence.emplace_back(11,0.18576);
    sequence.emplace_back(1,0.18576);
    sequence.emplace_back(4,0.371519);
    sequence.emplace_back(6,0.18576);
    sequence.emplace_back(5,0.18576);

    vector<vector<int>> segments = splitIntoSegments(sequence,2.0);
    REQUIRE(segments.size() == 2);
    CHECK(segments.at(0).at(0) == 4);
    CHECK(segments.at(0).at(1) == 6);
    CHECK(segments.at(0).at(2) == 8);
    CHECK(segments.at(0).at(3) == 11);
    CHECK(segments.at(0).at(4) == 1);
    CHECK(segments.at(0).at(5) == 4);
    CHECK(segments.at(0).at(6) == 6);
    CHECK(segments.at(1).at(0) == 5);

    VectorXd inputVector = VectorXd::Zero(12);
    inputVector(0,0) = 1;
    inputVector(3,0) = 1;
    inputVector(5,0) = 1;
    inputVector(7,0) = 1;
    inputVector(10,0) = 1;
    double result = getPitchKeyValue(inputVector,1);
    CHECK(result == Approx(17.0));
    result = getPitchKeyValue(inputVector,2);
    CHECK(result == Approx(17.5));

    vector<VectorXd> pitchKeyVectors = getPitchKeyValues(segments);
    REQUIRE(pitchKeyVectors.size() == 2);
    REQUIRE(pitchKeyVectors.at(0).rows() == 12);
    REQUIRE(pitchKeyVectors.at(0).cols() == 1);
    REQUIRE(pitchKeyVectors.at(1).rows() == 12);
    REQUIRE(pitchKeyVectors.at(1).cols() == 1);

    CHECK(pitchKeyVectors.at(0)(0,0) == Approx(17.0));
    CHECK(pitchKeyVectors.at(0)(1,0) == Approx(17.5));
    CHECK(pitchKeyVectors.at(0)(2,0) == Approx(11.5));
    CHECK(pitchKeyVectors.at(0)(3,0) == Approx(21.0));
    CHECK(pitchKeyVectors.at(0)(4,0) == Approx(12.0));
    CHECK(pitchKeyVectors.at(0)(5,0) == Approx(18.5));
    CHECK(pitchKeyVectors.at(0)(6,0) == Approx(16.0));
    CHECK(pitchKeyVectors.at(0)(7,0) == Approx(14.5));
    CHECK(pitchKeyVectors.at(0)(8,0) == Approx(20.0));
    CHECK(pitchKeyVectors.at(0)(9,0) == Approx(9.5));
    CHECK(pitchKeyVectors.at(0)(10,0) == Approx(20.5));
    CHECK(pitchKeyVectors.at(0)(11,0) == Approx(14.5));

    CHECK(pitchKeyVectors.at(1)(0,0) == Approx(4.5));
    CHECK(pitchKeyVectors.at(1)(1,0) == Approx(2.0));
    CHECK(pitchKeyVectors.at(1)(2,0) == Approx(3.5));
    CHECK(pitchKeyVectors.at(1)(3,0) == Approx(2.0));
    CHECK(pitchKeyVectors.at(1)(4,0) == Approx(5.0));
    CHECK(pitchKeyVectors.at(1)(5,0) == Approx(4.0));
    CHECK(pitchKeyVectors.at(1)(6,0) == Approx(1.5));
    CHECK(pitchKeyVectors.at(1)(7,0) == Approx(3.5));
    CHECK(pitchKeyVectors.at(1)(8,0) == Approx(2.0));
    CHECK(pitchKeyVectors.at(1)(9,0) == Approx(4.5));
    CHECK(pitchKeyVectors.at(1)(10,0) == Approx(2.0));
    CHECK(pitchKeyVectors.at(1)(11,0) == Approx(4.0));

    vector<vector<pair<double,int>>> bestSums = getBestSums(pitchKeyVectors,2.0);
    REQUIRE(bestSums.size() == 2);
    REQUIRE(bestSums.at(0).size() == 12);
    REQUIRE(bestSums.at(1).size() == 12);
    CHECK(bestSums.at(0).at(0).first == Approx(17.0));
    CHECK(bestSums.at(0).at(0).second == -1);
    CHECK(bestSums.at(0).at(1).first == Approx(17.5));
    CHECK(bestSums.at(0).at(1).second == -1);
    CHECK(bestSums.at(0).at(2).first == Approx(11.5));
    CHECK(bestSums.at(0).at(2).second == -1);
    CHECK(bestSums.at(0).at(3).first == Approx(21.0));
    CHECK(bestSums.at(0).at(3).second == -1);
    CHECK(bestSums.at(0).at(4).first == Approx(12.0));
    CHECK(bestSums.at(0).at(4).second == -1);
    CHECK(bestSums.at(0).at(5).first == Approx(18.5));
    CHECK(bestSums.at(0).at(5).second == -1);
    CHECK(bestSums.at(0).at(6).first == Approx(16.0));
    CHECK(bestSums.at(0).at(6).second == -1);
    CHECK(bestSums.at(0).at(7).first == Approx(14.5));
    CHECK(bestSums.at(0).at(7).second == -1);
    CHECK(bestSums.at(0).at(8).first == Approx(20.0));
    CHECK(bestSums.at(0).at(8).second == -1);
    CHECK(bestSums.at(0).at(9).first == Approx(9.5));
    CHECK(bestSums.at(0).at(9).second == -1);
    CHECK(bestSums.at(0).at(10).first == Approx(20.5));
    CHECK(bestSums.at(0).at(10).second == -1);
    CHECK(bestSums.at(0).at(11).first == Approx(14.5));
    CHECK(bestSums.at(0).at(11).second == -1);

    CHECK(bestSums.at(1).at(0).first == Approx(23.5));
    CHECK(bestSums.at(1).at(0).second == 3);
    CHECK(bestSums.at(1).at(1).first == Approx(21.0));
    CHECK(bestSums.at(1).at(1).second == 3);
    CHECK(bestSums.at(1).at(2).first == Approx(22.5));
    CHECK(bestSums.at(1).at(2).second == 3);
    CHECK(bestSums.at(1).at(3).first == Approx(23.0));
    CHECK(bestSums.at(1).at(3).second == 3);
    CHECK(bestSums.at(1).at(4).first == Approx(24.0));
    CHECK(bestSums.at(1).at(4).second == 3);
    CHECK(bestSums.at(1).at(5).first == Approx(23.0));
    CHECK(bestSums.at(1).at(5).second == 3);
    CHECK(bestSums.at(1).at(6).first == Approx(20.5));
    CHECK(bestSums.at(1).at(6).second == 3);
    CHECK(bestSums.at(1).at(7).first == Approx(22.5));
    CHECK(bestSums.at(1).at(7).second == 3);
    CHECK(bestSums.at(1).at(8).first == Approx(22.0));
    CHECK(bestSums.at(1).at(8).second == 8);
    CHECK(bestSums.at(1).at(9).first == Approx(23.5));
    CHECK(bestSums.at(1).at(9).second == 3);
    CHECK(bestSums.at(1).at(10).first == Approx(22.5));
    CHECK(bestSums.at(1).at(10).second == 10);
    CHECK(bestSums.at(1).at(11).first == Approx(23.0));
    CHECK(bestSums.at(1).at(11).second == 3);

    vector<string> bestPath = getBestPath(bestSums);
    REQUIRE(bestPath.size() == 2);
    CHECK(bestPath.at(0) == "C");
    CHECK(bestPath.at(1) == "C#");

    vector<pair<pair<int,int>,string>> segmentsAndKeys = detectKey(sequence,2.0,2.0);
    REQUIRE(segmentsAndKeys.size() == 2);
    CHECK(segmentsAndKeys.at(0).first.first == 0);
    CHECK(segmentsAndKeys.at(0).first.second == 7);
    CHECK(segmentsAndKeys.at(0).second == "C");
    CHECK(segmentsAndKeys.at(1).first.first == 7);
    CHECK(segmentsAndKeys.at(1).first.second == 8);
    CHECK(segmentsAndKeys.at(1).second == "C#");

    vector<pair<int,double>> sequence2;
    sequence2.emplace_back(4,0.5);
    sequence2.emplace_back(8,0.5);
    sequence2.emplace_back(11,0.5);
    sequence2.emplace_back(3,0.5);
    sequence2.emplace_back(11,0.5);
    sequence2.emplace_back(11,0.5);
    sequence2.emplace_back(11,0.5);
    sequence2.emplace_back(8,0.5);
    sequence2.emplace_back(5,0.5);
    sequence2.emplace_back(10,0.5);
    sequence2.emplace_back(8,1);
    sequence2.emplace_back(4,0.5);
    sequence2.emplace_back(3,0.5);
    sequence2.emplace_back(8,0.5);
    sequence2.emplace_back(11,0.5);
    vector<pair<pair<int,int>,string>> segmentsAndKeys2 = detectKey(sequence2,2.0,2.0);
    REQUIRE(segmentsAndKeys2.size() == 4);
    CHECK(segmentsAndKeys2.at(0).first.first == 0);
    CHECK(segmentsAndKeys2.at(0).first.second == 4);
    CHECK(segmentsAndKeys2.at(0).second == "C");
    CHECK(segmentsAndKeys2.at(1).first.first == 4);
    CHECK(segmentsAndKeys2.at(1).first.second == 8);
    CHECK(segmentsAndKeys2.at(1).second == "C");
    CHECK(segmentsAndKeys2.at(2).first.first == 8);
    CHECK(segmentsAndKeys2.at(2).first.second == 11);
    CHECK(segmentsAndKeys2.at(2).second == "A");
    CHECK(segmentsAndKeys2.at(3).first.first == 11);
    CHECK(segmentsAndKeys2.at(3).first.second == 15);
    CHECK(segmentsAndKeys2.at(3).second == "C");

    vector<pair<int,double>> sequence3;
    sequence3.emplace_back(4,0.5);
    sequence3.emplace_back(8,0.5);
    sequence3.emplace_back(11,0.5);
    sequence3.emplace_back(3,0.5);
    vector<pair<pair<int,int>,string>> segmentsAndKeys3 = detectKey(sequence3,2.0,6.0);
    REQUIRE(segmentsAndKeys3.size() == 1);
    CHECK(segmentsAndKeys3.at(0).first.first == 0);
    CHECK(segmentsAndKeys3.at(0).first.second == 4);
    CHECK(segmentsAndKeys3.at(0).second == "C");

}

/**
 * tests as much as possible the fpm code
 */
TEST_CASE("Tests the prediction/fpm code", "[fpm]") {

    FPM fpm("runtime/matrices/BNote.csv","runtime/matrices/NNote.csv","runtime/matrices/tNote.csv",0.5,0.4,
            "runtime/matrices/BDir.csv","runtime/matrices/NDir.csv","runtime/matrices/tDir.csv",0.5,1.9);

    MatrixXd BNote = fpm.getBNote();
    MatrixXd NNote = fpm.getNNote();
    MatrixXd tNote = fpm.gettNote();
    MatrixXd BDir = fpm.getBDir();
    MatrixXd NDir = fpm.getNDir();
    MatrixXd tDir = fpm.gettDir();

    //testing also by visual inspection
    REQUIRE(BNote.rows() == 4);
    REQUIRE(BNote.cols() == 6);
    REQUIRE(NNote.rows() == 6);
    REQUIRE(NNote.cols() == 13);
    REQUIRE(tNote.rows() == 4);
    REQUIRE(tNote.cols() == 13);
    REQUIRE(BDir.rows() == 1);
    REQUIRE(BDir.cols() == 5);
    REQUIRE(NDir.rows() == 5);
    REQUIRE(NDir.cols() == 2);
    REQUIRE(tDir.rows() == 1);
    REQUIRE(tDir.cols() == 2);


    cout << "BNote" << endl; //CORRECT
    cout << BNote << endl;
    cout << "NNote" << endl; //CORRECT
    cout << NNote << endl;
    cout << "tNote" << endl; //CORRECT
    cout << tNote << endl;
    cout << "BDir" << endl; //CORRECT
    cout << BDir << endl;
    cout << "NDir" << endl; //CORRECT
    cout << NDir << endl;
    cout << "tDir" << endl; //CORRECT
    cout << tDir << endl;

    vector<pair<int,double>> sequence;
    sequence.emplace_back(51,0.371519);
    sequence.emplace_back(53,0.371519);
    sequence.emplace_back(55,0.18576);
    sequence.emplace_back(34,0.18576);
    sequence.emplace_back(48,0.18576);
    sequence.emplace_back(51,0.371519);
    sequence.emplace_back(53,0.18576);
    sequence.emplace_back(52,0.18576);

    for (auto p : sequence) {
        fpm.queueNote(p.first,p.second);
    }

    vector<int> absSequence = fpm.getAbsQueue();
    vector<pair<int,double>> noteSequence = fpm.getNoteQueue();
    vector<int> dirSequence = fpm.getDirQueue();

    REQUIRE(absSequence.size() == 8);
    REQUIRE(noteSequence.size() == 8);
    REQUIRE(dirSequence.size() == 7);

    CHECK(absSequence.at(0) == 51);
    CHECK(absSequence.at(1) == 53);
    CHECK(absSequence.at(2) == 55);
    CHECK(absSequence.at(3) == 34);
    CHECK(absSequence.at(4) == 48);
    CHECK(absSequence.at(5) == 51);
    CHECK(absSequence.at(6) == 53);
    CHECK(absSequence.at(7) == 52);

    CHECK(noteSequence.at(0).first == 4);
    CHECK(noteSequence.at(0).second == Approx(0.371519));
    CHECK(noteSequence.at(1).first == 6);
    CHECK(noteSequence.at(1).second == Approx(0.371519));
    CHECK(noteSequence.at(2).first == 8);
    CHECK(noteSequence.at(2).second == Approx(0.18576));
    CHECK(noteSequence.at(3).first == 11);
    CHECK(noteSequence.at(3).second == Approx(0.18576));
    CHECK(noteSequence.at(4).first == 1);
    CHECK(noteSequence.at(4).second == Approx(0.18576));
    CHECK(noteSequence.at(5).first == 4);
    CHECK(noteSequence.at(5).second == Approx(0.371519));
    CHECK(noteSequence.at(6).first == 6);
    CHECK(noteSequence.at(6).second == Approx(0.18576));
    CHECK(noteSequence.at(7).first == 5);
    CHECK(noteSequence.at(7).second == Approx(0.18576));

    CHECK(dirSequence.at(0) == 1);
    CHECK(dirSequence.at(1) == 1);
    CHECK(dirSequence.at(2) == 0);
    CHECK(dirSequence.at(3) == 1);
    CHECK(dirSequence.at(4) == 1);
    CHECK(dirSequence.at(5) == 1);
    CHECK(dirSequence.at(6) == 0);



    MatrixXd prediction = fpm.combinedPredict();
    //toChaosRep correct
    //prediction functions correct
    //find closest codebook vector correct.

    absSequence = fpm.getAbsQueue();
    noteSequence = fpm.getNoteQueue();
    dirSequence = fpm.getDirQueue();
    REQUIRE(absSequence.empty());
    REQUIRE(noteSequence.empty());
    REQUIRE(dirSequence.empty());


}