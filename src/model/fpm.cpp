/**
 * file implements class functionality found within fpm.h
 * Author: Charlie Street
 */

#include "../../include/model/fpm.h"

/**
 * applies the mapping for the chaos representation conversion
 * @param t_i the corner on the D dimensional hypercube
 * @param k the `shrinking' parameter
 * @param x the point on the D dimensional hypercube
 * @return the mapped point
 */
VectorXd FPM::applyMap(VectorXd t_i, double k, VectorXd x) {
    //TODO: Fill In!
}


/**
  * forms the chaos block representation for a sequence of notes
  * @param t all corners of the hypercube
  * @param k the `shrinking' parameter
  * @param S the sequence of notes
  */
VectorXd FPM::toChaosRep(MatrixXd t, double k, vector<int> S) {
    //TODO: Fill In!
}

/**
 * calculates euclidean distance between two vectors
 * @param x a vector
 * @param y a vector
 * @return the euclidean distance
 */
double FPM::euclideanDistance(VectorXd x, VectorXd y) {
    //TODO: Fill In!
}

/**
 * function finds the closest codebook vector
 * to point x, and returns the index in B of
 * the closest codebook vector
 * @param x the data point x
 * @param B a matrix consisting of all the codebook vectors
 * @return the index in B of the closest codebook vector
 */
int FPM::findClosestCodebook(VectorXd x, MatrixXd B) {
    //TODO: Fill In!
}

/**
 * predicts the next note in the sequence s
 * @param s the sequence of notes
 * @param B the matrix of codebook vectors
 * @param N the matrix of distributions
 * @param t the matrix of D dimensional hypercube parameters
 * @param k the `shrinking' parameter
 * @return the next note in the sequence
 */
int FPM::predictNextNote(vector<int> s, MatrixXd B, MatrixXd N, MatrixXd t, double k) {
    //TODO: Fill In!
}

/**
 * predicts the motion of the next note
 * @param s a sequence of directions (+/-)
 * @param B the matrix of codebook vectors
 * @param N the matrix of distributions
 * @param t the matric of D dimensional hypercube corners
 * @param k the `shrinking' parameter
 * @param upInterval the upwards interval
 * @return the next direction in the sequence
 */
int FPM::predictNextDir(vector<int> s, MatrixXd B, MatrixXd N, MatrixXd t, double k, int upInterval) {
    //TODO: Fill In!
}

/**
 * reads in a matrix from a csv file
 * @param filePath the path to the matrix file
 * @param rows the number of rows
 * @param cols the number of columns
 * @return the read-in matrix
 */
MatrixXd FPM::readInMat(string filePath, int rows, int cols) {
    //TODO: Fill In!
}



/**
 * constructor sets up the model
 * @param BNotePath path to BNote matrix
 * @param NNotePath path to NNote matrix
 * @param tNotePath path to tNote matrix
 * @param kNote k parameter for note prediction
 * @param TNote temperature for tilting NNote
 * @param BDirPath path to BDir matrix
 * @param NDirPath path to NDir matrix
 * @param tDirPath path to tDir matrix
 * @param kDir k parameter for direction prediction
 * @param TDir temperature for large interval penalty
 */
FPM::FPM(string BNotePath, string NNotePath, string tNotePath, double kNote, double TNote,
        string BDirPath, string NDirPath, string tDirPath, double kDir, double TDir) {
    //TODO: Fill In!
}


/**
 * adds a new note found to the queue
 * @param note the note (24-79)
 * @param duration the duration of said note
 */
void FPM::queueNote(int note, double duration) {
    //TODO: Fill In!
}

/**
 * will generate a suitable response sequence
 * to that which has been queued up
 * the output will be of length of the queued notes
 * @return a matrix of notes and duration
 */
MatrixXd FPM::combinedPredict() {
    //TODO: Fill In!
}

/**
 * member function wipes the arrays for the next round of prediction etc.
 */
void FPM::clearState() {
    //TODO: Fill In!
}