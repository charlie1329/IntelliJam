/*
 * The main file for carrying out ICA
 * it collates everything together in the project
 * and runs everything
 * Author: Charlie Street
 */

#include <iostream>
#include "../include/audioFileHelpers.h"
#include "../include/ICAHelpers.h"
#include "../include/ICA.h"

#define COMPONENTS 2 //how many independent components do we want to find?
#define DAMPENING 0.2 //things seem to be outputting very loudly, I am going to try and suppress that

//function to do the work bringing everything together
void runICA(string inputPath, string outputPathNoExtension);
Matrix<double,SPEAKERS,Dynamic> fromBuffers(const audioBuffers &track);

int main() {
    runICA("../../../Documents/fyp/extraction_tests/original_audio/metal.wav", "../../../Documents/fyp/extraction_tests/ICA/metal");
    return 0;
}

/* carry out the following set of steps
 * read data from wav/flac file
 * read into an eigen matrix
 * subtract mean from data
 * whiten data
 * run ICA
 * unwhiten the results
 * add the mean back on
 * write back into a wav file
 */
void runICA(string inputPath,string outputPathNoExtension) {

    //read from wav file
    shared_ptr<audioBuffers> stereoRead = readInFromFile(std::move(inputPath));
    //read into an eigen matrix
    Matrix<double,SPEAKERS,Dynamic> X = fromBuffers(*stereoRead);
    Matrix<double,SPEAKERS,Dynamic> XOriginal = X; //need that for later

    //subtract the mean from the data
    Matrix<double,SPEAKERS,1> m = subtractMean(X);
    cout << "subbed mean\n" << endl;
    //whiten the data
    shared_ptr<EigenDecomp> eigen = whitenData(X);
    cout << "whitened data\n" << endl;
    //run ICA
    auto AMinusOne = fastICA(X,COMPONENTS);
    cout << "completed ICA\n" << endl;
    Matrix<double,SPEAKERS,Dynamic> A = AMinusOne.inverse();

    //un-whiten the data
    unwhitenData(A, *eigen);
    cout << "unwhitened data\n" << endl;

    Matrix<double,Dynamic,Dynamic> s = A.inverse() * XOriginal;

    //add the mean back on
    addMean(s,A,m);
    cout << "added back mean" << endl;

    //convert to std vectors for ease of use
    vector<vector<double>> sVec(2);

    for(int i = 0; i < s.cols(); i++) {
        sVec.at(0).push_back(s(0,i) * DAMPENING);
        sVec.at(1).push_back(s(1,i) * DAMPENING);
    }
    cout << "about to write" << endl;
    //write back to a new wav file
    writeToFile(std::move(outputPathNoExtension),sVec);
}


