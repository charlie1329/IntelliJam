/**
 * header file for key detection functionality
 * Ported from Python code in fractal/keyDetection.py
 * Author: Charlie Street
 */

#ifndef FYP_KEYDETECT_H
#define FYP_KEYDETECT_H

#include <string>
#include <vector>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

const string keys[] =  {"A","A#","B","C","C#","D","D#","E","F","F#","G","G#","Am","A#m","Bm","Cm","C#m","Dm","D#m","Em","Fm","F#m","Gm","G#m"};
#define NUM_KEYS 12 // only considering major keys (drawback in the system)
#define MODULATION_PENALTY 2.0
#define SEGMENT_LENGTH 2.0
const double majorProfile[] = {5.0, 2.0, 3.5, 2.0, 4.5, 4.0, 2.0, 4.5, 2.0, 3.5, 1.5, 4.0};
const double minorProfile[] = {5.0, 2.0, 3.5, 4.5, 2.0, 4.0, 2.0, 4.5, 3.5, 2.0, 2.5, 4.0};

/**
 * mod function which abides by the rules I want
 * e.g. -3 % 12 = 9
 * @param a an integer
 * @param b an integer
 * @return a % b
 */
int mod(int a, int b);

/**
 * converts an integer value to a key
 * @param keyIndex the index of the key (note starts at 1)
 * @return the string representing the key
 */
string valToKey(int keyIndex);

/**
 * converts a key (string) to an integer index
 * @param key the key to be converted
 * @return the integer representation of the key
 */
int keyToVal(string key);

/**
 * function transposes a sequence of notes from one key to another
 * @param sequence the input sequence of notes
 * @param src_key the source key as a string
 * @param dst_key the destination key as a string
 * @return the transposed sequence
 */
vector<int> transpose(vector<int> sequence, string srcKey, string dstKey);

/**
 * for an input vector, the key value for a particular
 * key is calculated
 * @param notesPresent the flat input vector
 * @param keyIndex the key as an integer
 * @return the dot product with the correct key profile
 */
double getPitchKeyValue(VectorXd notesPresent, int keyIndex);

/**
 * splits a sequence of notes into a series of smaller segments
 * @param sequence the sequence of notes to split
 * @param segmentLength the maximum length of a segment
 * @return the segmented sequence
 */
vector<vector<int>> splitIntoSegments(vector<pair<int,double>> sequence, double segmentLength);

/**
 * gets the pitch key values for each key, for each segment
 * @param segments the segmented note sequence
 * @return a pitch key vector for each segment
 */
vector<VectorXd> getPitchKeyValues(vector<vector<int>> segments);

/**
 * gets the best sums along the paths
 * following this, we just need to backtrack to get the set of keys
 * @param pitchKeyVectors the pitch key values for each key, for each segment
 * @param modulationPenalty the penalty given for switching key
 * @return a vector of pairs listing the sum and the best index
 */
vector<vector<pair<double,int>>> getBestSums(vector<VectorXd> pitchKeyVectors, double modulationPenalty);

/**
 * function backtracks in order to retrieve the best path
 * @param bestSums likely the return value from getBestSums
 * @return the string of keys for each segment
 */
vector<string> getBestPath(vector<vector<pair<double,int>>> bestSums);

/**
 * function detects the key of a number of segments within sequence
 * @param sequence the original note sequence
 * @param segmentLength the maximum length of a segment
 * @param modulationPenalty the cost incurred by switching key
 * @return a vector with elements like (startPoint, endPoint, key) for each segment
 */
vector<pair<pair<int,int>,string>> detectKey(vector<pair<int,double>> sequence,
                                             double segmentLength, double modulationPenalty);

#endif //FYP_KEYDETECT_H
