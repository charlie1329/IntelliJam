/**
 * file implements function prototypes from fileToEcho.h
 * Author: Charlie Street
 */

#include "../../include/training/fileToEcho.h"
#include "../../include/libsndfile/sndfile.h"
#include <fstream>
#include <iostream>
#include <boost/thread.hpp>
#include <utility>

//prototype for thread worker function
void trainingReaderWorker(ESN echo, vector<pair<string,VectorXd>> files,
                          unsigned int sampleJump, const shared_ptr<boost::mutex> &lock,
                          const shared_ptr<training_set_t> &trainingSet);


/**
 * implemented from fileToEcho.h
 * converts wav file to esn reservoir state
 * @param echo the echo state network
 * @param filePath the path of the file of the training sample
 * @param sampleJump represents an artificial decrease in sample rate
 * @return the resulting reservoir state
 */
VectorXd wavToEcho(ESN echo, const string &filePath, unsigned int sampleJump) {

    echo.resetReservoir(); //reset the reservoir to its intitial state

    //read in from wav file
    auto *file_info = new SF_INFO;
    SNDFILE *wavFile = sf_open(filePath.c_str(),SFM_READ,file_info);

    //i only want to work with mono files here
    if(file_info->channels != 1) {
        throw "Non-mono file used!";
    }

    auto *allSamples = new double[file_info->frames];
    sf_count_t framesRead = sf_readf_double(wavFile, allSamples, file_info->frames);

    //loop through and feed into echo state network
    for(sf_count_t i = 0; i < framesRead; i++) {
        if(i % sampleJump == 0) echo.updateReservoir(allSamples[i]);
    }

    //close file
    int err = sf_close(wavFile);
    if(err != 0) { //don't need to throw an exception here, just a warning
        cout << "Warning, file: " << filePath
             << " produced an error on closing, error code: "
             << to_string(err) << endl;
    }

    //deallocate anything heap allocated
    delete file_info;
    delete allSamples;

    //return the new reservoir state
    return echo.getReservoir();

}

/**
 * implemented from fileToEcho.h
 * brings whole training set into memory
 * in the form of reservoir states of the echo state network
 * @param echo the echo state network being used right now
 * @param samplesAndOuts the config file containing file paths and ground truth values
 * @param sampleJump an artificial decrease in sample rate
 * @return the training set in the form of a vector of pairs
 */
shared_ptr<training_set_t> formTrainingSet(shared_ptr<ESN> echo, string samplesAndOuts, unsigned int sampleJump) {

    //where the training set will be located
    shared_ptr<training_set_t> trainingSet = std::make_shared<training_set_t>();

    //mutex lock for access to the training set
    shared_ptr<boost::mutex> lock = std::make_shared<boost::mutex>();

    //read in from csv file the file names and the ground truth values
    vector<pair<string,VectorXd>> namesAndTruth = readTrainingFile(std::move(samplesAndOuts),
                                                                   (echo->resOutWeights).rows());

    //get indexes to split at
    int stopFirst = namesAndTruth.size()/4;
    int stopSecond = namesAndTruth.size()/2;
    int stopThird = 3 * namesAndTruth.size() / 4;

    vector<pair<string,VectorXd>> fstVec(namesAndTruth.begin(), namesAndTruth.begin() + stopFirst);
    vector<pair<string,VectorXd>> sndVec(namesAndTruth.begin() + stopFirst, namesAndTruth.begin() + stopSecond);
    vector<pair<string,VectorXd>> thirdVec(namesAndTruth.begin() + stopSecond, namesAndTruth.begin() + stopThird);
    vector<pair<string,VectorXd>> fourthVec(namesAndTruth.begin() + stopThird, namesAndTruth.end());

    //start up threads
    //TODO: Check copies of ESN are made properly!!!
    boost::thread firstQuarter(trainingReaderWorker, *echo, fstVec, sampleJump, lock, trainingSet);
    boost::thread secondQuarter(trainingReaderWorker, *echo, sndVec, sampleJump, lock, trainingSet);
    boost::thread thirdQuarter(trainingReaderWorker, *echo, thirdVec, sampleJump, lock, trainingSet);
    boost::thread fourthQuarter(trainingReaderWorker, *echo, fourthVec, sampleJump, lock, trainingSet);

    //wait till threads finish
    firstQuarter.join();
    secondQuarter.join();
    thirdQuarter.join();
    fourthQuarter.join();

    return trainingSet;

}

/**
 * processes a segment of the training files in order to speed
 * up the time taken to generate the training reservoirs
 * @param echo the Echo State Network
 * @param files the files to process, and the associated ground truth
 * @param sampleJump artificial decrease in sample rate
 * @param lock to lock access to the training set
 * @param trainingSet the in progress training set
 */
void trainingReaderWorker(ESN echo, vector<pair<string,VectorXd>> files,
                          unsigned int sampleJump, const shared_ptr<boost::mutex> &lock,
                          const shared_ptr<training_set_t> &trainingSet) {

    for (auto currentFile : files) {
        VectorXd currentReservoir = wavToEcho(echo, currentFile.first, sampleJump);
        cout << "Finished reading in: " << currentFile.first << endl;

        //push into the training set
        lock->lock();
        trainingSet->emplace_back(currentReservoir, currentFile.second);
        lock->unlock();
    }
}

/**
 * function takes a csv file where each line is of the form
 * <name>,<note1>,<note2>,...,<noteN><NEWLINE>
 * and formats it into an easy to work with format
 * @param trainingFile the file containing all the necessary information
 * @param numOut how many ground truth values are there per sample
 * @return everything put nicely into a vector
 */
vector<pair<string,VectorXd>> readTrainingFile(const string &trainingFile, int numOut) {

    vector<pair<string,VectorXd>> namesAndTruth;

    ifstream csvFile(trainingFile);

    string currentLine;

    //get each line in turn
    while(getline(csvFile,currentLine)) {

        unsigned int currentPos = 0;
        unsigned int itemsFound = 0;

        //template for splitting string by delimiter found at:
        //https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c/14266139
        string token;
        string newFile;
        VectorXd newGT = MatrixXd::Zero(numOut,1);

        //loop through the current line
        while((currentPos = currentLine.find(',')) != std::string::npos && itemsFound < numOut) {

            token = currentLine.substr(0,currentPos);
            if(itemsFound == 0) {
                newFile = token;
            } else {
                newGT(itemsFound-1,0) = stod(token, nullptr);

            }

            itemsFound++;
            currentLine.erase(0, currentPos + 1); //+1 for ','
        }

        //if bad line in csv file found!
        if(itemsFound != numOut) {
            csvFile.close();
            throw "Invalid line when reading csv";
        }

        newGT(itemsFound-1,0) = stod(currentLine,nullptr); //account for last remaining item without comma

        namesAndTruth.emplace_back(newFile,newGT);
    }

    csvFile.close();

    return namesAndTruth;

}