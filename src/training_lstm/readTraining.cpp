/**
 * file implements functions with prototypes
 * in readTraining.h
 * Author: Charlie Street
 */

#include "../../include/training_lstm/readTraining.h"
#include "../../include/lstm/auxillary_functions.h"

/**
 * implemented from readTraining.h
 * function reads in from file containing training data and stores it
 * appropriately for training
 * @param path the path where the training data is stored
 * @return a vector of vectors of pairs representing the training data
 */
training_set_t readTrainingSet(string path) {

    training_set_t trainingSet;

    ifstream csvFile(path);

    vector<VectorXd> currentVector;

    string currentLine;

    //get each line in turn
    while(getline(csvFile,currentLine)) {
        if(currentLine.at(0) == '*') { //starting a new sample so store old one and clear
            if(!currentVector.empty()) {
                trainingSet.push_back(currentVector);
                currentVector.clear();
            }
        } else {
            //i'm assuming a well formed file here
            unsigned int comma = 0;
            comma = currentLine.find(',');

            //get the note from the line
            string note = currentLine.substr(0,comma);

            //erase note and first comma out of string
            currentLine.erase(0,comma+1);

            comma = currentLine.find(',');

            //get the duration from the line
            string duration = currentLine.substr(0,comma);

            //TODO: Decide what to do with silence in compression scheme
            VectorXd sample(2);
            sample(0,0) = compressNote(stod(note,nullptr));
            sample(1,0) = stod(duration,nullptr);

            currentVector.push_back(sample);
        }
    }

    if(!currentVector.empty()) trainingSet.push_back(currentVector); // push back last sample if needed

    return trainingSet;
}