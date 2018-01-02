/**
 * file implements function prototypes from checkpoint.h
 * Author: Charlie Street
 */

#include "../../include/training/checkpoint.h"
#include <fstream>
#include <iostream>
#include <string>

/**
 * function implemented from checkpoint.h
 * @param hyperCombos each sub-vector contains a set of hyper parameters
 * @param logFile the file containing all logging information
 * @return a tagged version of the hyper-parameter vector specifying if that combination has already been used
 */
vector<pair<vector<double>,bool>> findCompleted(vector<vector<double>> hyperCombos, string logFile) {

    //start by extracting from the log file
    ifstream log(logFile);

    vector<vector<double>> inFile;

    int lineNo = 0;

    string currentLine;

    //go through each line of the file and pick out the important information
    while(getline(log,currentLine)) {

        if((lineNo % 2) == 0) {
            vector<double> currentCombo;

            int delimNo = 0;
            unsigned int currentPos;
            while((currentPos = currentLine.find(' '))) {
                if((delimNo % 2) != 0) {
                    currentCombo.push_back(stod(currentLine.substr(0,currentPos)));
                }
                currentLine.erase(0,currentPos + 1); //erase that part of the string and keep going
                delimNo++;
            }

            inFile.push_back(currentCombo);
        }

        lineNo++;
    }

    log.close();

    vector<pair<vector<double>,bool>> taggedCombos;

    //now go about checking things
    for (auto &hyperCombo : hyperCombos) {
        bool isFound = false;

        for(auto &completed: inFile) {
            int numSame = 0;
            for(unsigned int i = 0; i < hyperCombo.size(); i++) {
                if(hyperCombo.at(i) == completed.at(i)){
                    numSame++;
                }   else {
                    break;
                }
            }

            if(numSame == hyperCombo.size()) isFound = true;
        }

        taggedCombos.emplace_back(hyperCombo,isFound);
    }

    return taggedCombos;
}