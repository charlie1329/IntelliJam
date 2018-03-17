/*
 * File to implement functions specified in audioFileHelpers.h
 * Author: Charlie Street
 */

#include <utility>
#include "../include/audioFileHelpers.h"

#define SAMPLE_RATE 44100 //44.1kHz, the standard sample rate

/* implementation of function in audioFileHelpers.h
 * will read in (probably) wav files into left/right buffer
 */
shared_ptr<audioBuffers> readInFromFile(string path) {

    auto *file_info = new SF_INFO;
    SNDFILE *wavFile = sf_open(path.c_str(), SFM_READ, file_info); //open the audio file

    if (file_info->channels != 2) { //i need stereo here, not mono
        return nullptr;
    }

    auto *allSound = new double[file_info->channels * file_info->frames]; //will temporarily store all the audio data
    sf_count_t framesRead = sf_readf_double(wavFile, allSound, file_info->frames); //read in data
    sf_count_t totalItems = file_info->channels * framesRead;

    vector<double> wavLeft; //my two audio channels
    vector<double> wavRight;

    //now to separate the left and right channels
    for(sf_count_t i = 0; i < totalItems; i++) {
        if(i % 2 == 0) { //even gives left channel
            wavLeft.push_back(allSound[i]);
        } else { //odd gives right channel
            wavRight.push_back(allSound[i]);
        }
    }

    delete file_info; //delete any temporarily stored variables on the heap
    delete allSound;

    sf_close(wavFile); //good practice to close the file!

    return make_shared<audioBuffers>(wavLeft, wavRight);


}

/* implementation of function from audioFileHelpers.h
 * Takes buffers of audio and writes them into a playable wav file
 * each buffer is a different file to be written
 * outputPath should not contain a file extension
 * a small amount of advice for this function came from:
 * https://stackoverflow.com/questions/22530515/writing-to-a-wav-file-using-libsndfile
 */
void writeToFile(string outputPathNoExtension, vector<vector<double>> s) {

    SF_INFO fileInfo{};
    fileInfo.channels = 1; //each component is mono
    fileInfo.samplerate = SAMPLE_RATE;
    fileInfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16; //writing to a wav file with 16 bits per sample

    //for each independent component
    for(unsigned int i = 0; i < s.size(); i++) {

        //first set up the output file
        string outputPath = outputPathNoExtension + to_string(i) + ".wav";
        SNDFILE *output = sf_open(outputPath.c_str(), SFM_WRITE, &fileInfo);

        //now write the vectors into a double array
        auto buf = new double[s.at(i).size()];

        for(unsigned int j = 0; j < s.at(i).size(); j++) {
            //buf[j] = s.at(i).at(j);
            double current = s.at(i).at(j);
            if(current > 1.0) { //clipping to be safe
                buf[j] = 1.0;
            } else if (current < -1.0) {
                buf[j] = -1.0;
            } else {
                buf[j] = current;
            }
        }

        //now write buffer to file
        sf_write_double(output,buf,s.at(i).size());
        sf_write_sync(output); //force the write through (don't stay in an OS buffer)

        //close the file
        sf_close(output);
    }

}

/* constructor for audioBuffers struct which allows
 * the passing in of the two members/fields of the struct
 */
audioBuffers::audioBuffers(vector<double> newLeft,
                           vector<double> newRight) : left(std::move(newLeft)),
                                                      right(std::move(newRight)) {}


//function takes audio buffers and writes it into an eigen matrix
Matrix<double,2,Dynamic> fromBuffers(const audioBuffers &track) {

    Matrix<double,2,Dynamic> X = MatrixXd::Zero(2,track.left.size());

    //write into matrix
    for(unsigned int i = 0; i < track.left.size(); i ++) {
        X(0,i) = track.left.at(i);
        X(1,i) = track.right.at(i);
    }

    return X;
}