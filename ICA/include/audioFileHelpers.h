/*
 * header file for my audio file helper functions
 * these are used throughout the ica implementation for reading in my sound files etc.
 * Author: Charlie Street
 */

#ifndef ICA_AUDIOFILEHELPERS_H
#define ICA_AUDIOFILEHELPERS_H

#include <sndfile.h> //necessary for all audio reading functions
#include <string> //need for file paths
#include <memory> //want to use shared pointers
#include <utility>
#include <vector> //probably better to use than c-style arrays
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

struct audioBuffers {
    vector<double> left;
    vector<double> right;
    audioBuffers(vector<double> newLeft, vector<double> newRight);
};

//reads from a wav file and fills left/right buffer
shared_ptr<audioBuffers> readInFromFile(string path);


//takes (probably mono) track and writes to file
void writeToFile(string outputPath, vector<vector<double>> s);

//audio buffers into a matrix
Matrix<double,2,Dynamic> fromBuffers(const audioBuffers &track);

#endif //ICA_AUDIOFILEHELPERS_H
