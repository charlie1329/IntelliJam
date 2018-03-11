/**
 * file implements functionality specified within keyDetect.h
 * Author: Charlie Street
 */

#include <utility>

#include "../../include/model/keyDetect.h"
#include <iostream>

/**
 * mod function which abides by the rules I want
 * e.g. -3 % 12 = 9
 * @param a an integer
 * @param b an integer
 * @return a % b
 */
int mod(int a, int b) {
    return ((a % b) + b) % b;
}

/**
 * converts an integer value to a key
 * @param keyIndex the index of the key (note starts at 1)
 * @return the string representing the key
 */
string valToKey(int keyIndex) {
    return keys[keyIndex-1];
}

/**
 * converts a key (string) to an integer index
 * @param key the key to be converted
 * @return the integer representation of the key
 */
int keyToVal(string key) {
    for(int i = 0; i < NUM_KEYS; i++) {
        if(key == keys[i]) return i+1;
    }

    return -1;
}

/**
 * function transposes a sequence of notes from one key to another
 * @param sequence the input sequence of notes
 * @param src_key the source key as a string
 * @param dst_key the destination key as a string
 * @return the transposed sequence
 */
vector<int> transpose(vector<int> sequence, string srcKey, string dstKey) {
    int dstVal = keyToVal(std::move(dstKey));
    int srcVal = keyToVal(std::move(srcKey));

    vector<int> transposed;

    //can't go between major and minor keys unfortunately
    if((dstVal > 12) && (srcVal <= 12) || (dstVal <= 12 && srcVal > 12)) {
        return transposed;
    }

    int toShift = dstVal - srcVal;

    //actually carry out the transposing
    for (auto s : sequence) {
        if(s == 0) {
            transposed.push_back(s);
        } else {
            transposed.push_back(mod((s + toShift - 1),12) + 1);
        }
    }

    return transposed;
}

/**
 * for an input vector, the key value for a particular
 * key is calculated
 * @param notesPresent the flat input vector
 * @param keyIndex the key as an integer
 * @return the dot product with the correct key profile
 */
double getPitchKeyValue(VectorXd notesPresent, int keyIndex) {
    int startPoint = keyIndex - 1;

    double pitchVal = 0.0;

    for(int i = startPoint; i < startPoint + NUM_KEYS; i++) {
        pitchVal += (notesPresent(i % NUM_KEYS,0) * majorProfile[i-startPoint]);
    }

    return pitchVal;
}

/**
 * splits a sequence of notes into a series of smaller segments
 * @param sequence the sequence of notes to split
 * @param segmentLength the maximum length of a segment
 * @return the segmented sequence
 */
vector<vector<int>> splitIntoSegments(vector<pair<int,double>> sequence, double segmentLength) {
    vector<vector<int>> segments;
    double segmentTime = 0.0;
    vector<int> currentSegment;

    //loop through the sequence and split
    for (auto currentNote : sequence) {  //note, duration pairs

        //if we reach the end of the segment
        if(segmentTime + currentNote.second > segmentLength) {
            if(currentSegment.empty()) {
                currentSegment.push_back(currentNote.first); //just place in its own segment
                segments.push_back(currentSegment);
                segmentTime = 0.0;
                currentSegment.clear();
            } else {
                segments.push_back(currentSegment);
                segmentTime = currentNote.second;
                currentSegment.clear();
                currentSegment.push_back(currentNote.first);
            }
        } else {
            currentSegment.push_back(currentNote.first);
            segmentTime += currentNote.second;
        }
    }

    //add on the last segment for completion's sake
    if(!currentSegment.empty()) {
        segments.push_back(currentSegment);
    }

    return segments;
}

/**
 * gets the pitch key values for each key, for each segment
 * @param segments the segmented note sequence
 * @return a pitch key vector for each segment
 */
vector<VectorXd> getPitchKeyValues(vector<vector<int>> segments) {
    vector<VectorXd> pitchKeyVectors;

    //calculate for each segment
    for(const auto &segment : segments) {

        //form the input vector
        VectorXd inputVector = VectorXd::Zero(12);
        for(int note: segment) {
            if (note != 0) { //if not silence
                inputVector(note-1,0) = 1;
            }
        }

        //now for each input vector, calculate the pitch key values for each of the 24 keys
        VectorXd pitchKeyVals = VectorXd::Zero(NUM_KEYS);
        for (int key = 1; key <= NUM_KEYS; key++) {
            pitchKeyVals(key-1,0) = getPitchKeyValue(inputVector,key);
        }

        pitchKeyVectors.push_back(pitchKeyVals); //now we have the values for the nth segment
    }

    return pitchKeyVectors;
}

/**
 * gets the best sums along the paths
 * following this, we just need to backtrack to get the set of keys
 * @param pitchKeyVectors the pitch key values for each key, for each segment
 * @param modulationPenalty the penalty given for switching key
 * @return a vector of pairs listing the sum and the best index
 */
vector<vector<pair<double,int>>> getBestSums(vector<VectorXd> pitchKeyVectors, double modulationPenalty) {
    vector<vector<pair<double,int>>> bestSums;
    vector<pair<double,int>> firstStep;

    //deal with special case of start of path
    for(int i = 0; i < pitchKeyVectors.at(0).rows(); i++) {
        firstStep.emplace_back(pitchKeyVectors.at(0)(i,0),-1);
    }

    bestSums.push_back(firstStep);
    //now deal with the rest of the path segments
    vector<pair<double,int>> bestSumi;
    for(unsigned int i = 1; i < pitchKeyVectors.size(); i++) {

        for(int j = 0; j < pitchKeyVectors.at(i).rows(); j++) {
            double bestJ = -1;
            int bestJIndex = -1;

            //do the dynamic programming bit
            for(int unsigned k = 0; k < bestSums.at(i-1).size(); k++) {
                double newSum = pitchKeyVectors.at(i)(j,0) + bestSums.at(i-1).at(k).first;
                if(j != k) { //add penalty for changing key
                    newSum -= modulationPenalty;
                }

                if(newSum > bestJ) {
                    bestJ = newSum;
                    bestJIndex = k;
                }
            }

            bestSumi.emplace_back(bestJ,bestJIndex);
        }

        bestSums.push_back(bestSumi);
        bestSumi.clear();
    }

    return bestSums;
}

/**
 * function backtracks in order to retrieve the best path
 * @param bestSums likely the return value from getBestSums
 * @return the string of keys for each segment
 */
vector<string> getBestPath(vector<vector<pair<double,int>>> bestSums) {
    vector<int> bestPath;
    double maxLast = -1;
    int maxLastIndex = -1;

    //go through the last part of bestSums and find the starting point (max at the end)
    for(unsigned int i = 0; i < bestSums.at(bestSums.size()-1).size(); i++) {
        double currentSum = bestSums.at(bestSums.size()-1).at(i).first;
        if(currentSum > maxLast) {
            maxLast = currentSum;
            maxLastIndex = i;
        }
    }

    bestPath.push_back(maxLastIndex);

    unsigned int index = 0;
    for (unsigned int i = bestSums.size()-1; i > 0; i--) {
        bestPath.push_back(bestSums.at(i).at(static_cast<unsigned int>(bestPath.at(index))).second);
        index++;
    }

    //now reverse the vector to get the path in the right order
    reverse(bestPath.begin(), bestPath.end());

    vector<string> finalPath;

    //convert all to string for final return value
    for (int i : bestPath) {
        finalPath.push_back(valToKey(i + 1));
    }

    return finalPath;
}

/**
 * function detects the key of a number of segments within sequence
 * @param sequence the original note sequence
 * @param segmentLength the maximum length of a segment
 * @param modulationPenalty the cost incurred by switching key
 * @return a vector with elements like (startPoint, endPoint, key) for each segment
 */
vector<pair<pair<int,int>,string>> detectKey(vector<pair<int,double>> sequence,
                                             double segmentLength, double modulationPenalty) {

    //first split into segments, will find closest to segmentLength
    vector<vector<int>> segments = splitIntoSegments(std::move(sequence),segmentLength);

    //now generate the pitch key values for each segment
    vector<VectorXd> pitchKeyVectors = getPitchKeyValues(segments);

    //then take a dynamic programming approach to find the best solution
    vector<vector<pair<double,int>>> bestSums = getBestSums(pitchKeyVectors, modulationPenalty);

    //now back track and retrieve the best path of keys
    vector<string> bestPath = getBestPath(bestSums);

    //segmentsAndKeys is a vector of (effectively) triples (startPoint, endPoint+1, key)
    vector<pair<pair<int,int>,string>> segmentsAndKeys;
    int currentStartPoint = 0;
    for(unsigned int i = 0; i < segments.size(); i++) {
        int currentEndPoint = currentStartPoint + segments.at(i).size();
        pair<int,int> startEnd(currentStartPoint,currentEndPoint);
        segmentsAndKeys.emplace_back(startEnd,bestPath.at(i));
        currentStartPoint = currentEndPoint;
    }

    return segmentsAndKeys;

}