/**
 * file implements class functionality found within fpm.h
 * Author: Charlie Street
 */

#include <utility>
#include <cmath>
#include <limits>
#include <random>
#include <chrono>
#include "../../include/model/fpm.h"

/**
 * applies the mapping for the chaos representation conversion
 * @param t_i the corner on the D dimensional hypercube
 * @param k the `shrinking' parameter
 * @param x the point on the D dimensional hypercube
 * @return the mapped point
 */
VectorXd FPM::applyMap(VectorXd t_i, double k, VectorXd x) {
    return (k * x) + ((1.0 - k) * t_i);
}


/**
  * forms the chaos block representation for a sequence of notes
  * @param t all corners of the hypercube
  * @param k the `shrinking' parameter
  * @param S the sequence of notes
  */
VectorXd FPM::toChaosRep(MatrixXd t, double k, vector<int> S) {
    int D = t.rows();

    //initialise to centre of hypercube
    VectorXd x = MatrixXd::Constant(D,1,0.5);

    //now do the conversion to CBR
    for(auto note : S) {
       x = applyMap(t.col(note),k,x);
    }

    return x;
}

/**
 * calculates euclidean distance between two vectors
 * @param x a vector
 * @param y a vector
 * @return the euclidean distance
 */
double FPM::euclideanDistance(VectorXd x, VectorXd y) {
    double distance = 0.0;
    for(int i = 0; i < x.rows(); i++) {
        distance += pow((x(i,0)-y(i,0)),2);
    }
    return distance;
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
    double minDistance = std::numeric_limits<double>::infinity(); //set to inifinitely high value
    int minIndex = -1; //nothing found yet

    //loop through the codebook vectors
    for(int i = 0; i < B.cols(); i++) {
        double newDistance = euclideanDistance(x,B.col(i));
        if(newDistance < minDistance) {
            minDistance = newDistance;
            minIndex = i;
        }
    }

    //return the index of the closest codebook vector
    return minIndex;
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

    //bring sequence to chaos representation
    VectorXd x = toChaosRep(std::move(t),k,std::move(s));

    //now find the closest codebook vector
    int i = findClosestCodebook(x,std::move(B));
    RowVectorXd distribution = N.row(i);
    auto totalSamples = (int)distribution.sum();

    //set up a random generator
    default_random_engine gen;
    gen.seed((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> rng(1,totalSamples);

    //do some random sampling to generate the note
    int randNo = rng(gen);
    int total = 0;
    for(int j = 0; j < distribution.cols(); j++) {
        total += distribution(0,j);
        if(randNo <= total) return j;
    }
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

    //read in note parameters
    BNote = readInMat(std::move(BNotePath),4,6);
    NNote = readInMat(std::move(NNotePath),6,13);
    tNote = readInMat(std::move(tNotePath),4,13);
    this->kNote = kNote;
    this->TNote = TNote;

    //read in direction parameters
    BDir = readInMat(std::move(BDirPath),1,5);
    NDir = readInMat(std::move(NDirPath),5,2);
    tDir = readInMat(std::move(tDirPath),1,2);
    this->kDir = kDir;
    this->TDir = TDir;

    //scale the NNote matrix by the temperature parameter
    NNote = Eigen::pow(NNote.array(),1.0/TNote);


}


/**
 * adds a new note found to the queue
 * @param note the note (24-79)
 * @param duration the duration of said note
 */
void FPM::queueNote(int note, double duration) {
    absSequence.push_back(note); //just the note
    if(note == 0) {
        noteSequence.emplace_back(note,duration);
        return; // nothing to add to direction sequence
    } else {
        noteSequence.emplace_back((note % 12) + 1,duration);
    }

    if(previousNote != 1 && previousNote != note) {
        if(note > previousNote) {
            dirSequence.push_back(1);
        } else {
            dirSequence.push_back(0);
        }

        previousNote = note;
    }

    if(previousNote == -1) previousNote = note;
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
    //clear all vectors
    absSequence.clear();
    noteSequence.clear();
    dirSequence.clear();
    //reinitialise
    previousNote = -1;
}