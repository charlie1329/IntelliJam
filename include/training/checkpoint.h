/**
 * file contains function prototypes
 * for having checkpoints during the
 * echo state network training
 * Author: Charlie Street
 */

#ifndef FYP_CHECKPOINT_H
#define FYP_CHECKPOINT_H

#include <vector>
#include <string>

using namespace std;

/**
 * file examines log file to find which combinations of hyper parameters
 * have already been used during training
 * @param hyperCombos a vector of vectors, each sub vector contains a set of hyper parameters
 * @param logFile the file containing all the training logs
 * @return the same vector, but each sub vector is paired with a bool. True means the parameters have been used
 */
vector<pair<vector<double>,bool>> findCompleted(vector<vector<double>> hyperCombos, string logFile);

#endif //FYP_CHECKPOINT_H
