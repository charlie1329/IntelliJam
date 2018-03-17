/*
 * file checks the functions in audioFileHelpers.h/cpp work correctly
 * Author: Charlie Street
 */

#include "../include/audioFileHelpers.h"

int main() {

    shared_ptr<audioBuffers> track = readInFromFile("06 Permanating.wav");

    vector<vector<double>> buffers;

    buffers.push_back(track->left);
    buffers.push_back(track->right);

    writeToFile("separated",buffers);

    return 0;
}