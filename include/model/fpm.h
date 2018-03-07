/**
 * Header file for all fpm prediction behaviour
 * (and importantly the class encapsulating the model)
 * Author: Charlie Street
 */

#ifndef FYP_FPM_H
#define FYP_FPM_H

#include "keyDetect.h"

/**
 * class representing the combined fpm model
 * stores all matrices, and all other internal state
 */
class FPM {

private:
    /**
     * applies the mapping for the chaos representation conversion
     * @param t_i the corner on the D dimensional hypercube
     * @param k the `shrinking' parameter
     * @param x the point on the D dimensional hypercube
     * @return the mapped point
     */
    VectorXd applyMap(VectorXd t_i, double k, VectorXd x);

    /**
     * forms the chaos block representation for a sequence of notes
     * @param t all corners of the hypercube
     * @param k the `shrinking' parameter
     * @param S the sequence of notes
     */
    VectorXd toChaosRep(MatrixXd t, double k, vector<int> S);

    /**
     * calculates euclidean distance between two vectors
     * @param x a vector
     * @param y a vector
     * @return the euclidean distance
     */
    double euclideanDistance(VectorXd x, VectorXd y);

    /**
     * function finds the closest codebook vector
     * to point x, and returns the index in B of
     * the closest codebook vector
     * @param x the data point x
     * @param B a matrix consisting of all the codebook vectors
     * @return the index in B of the closest codebook vector
     */
    int findClosestCodebook(VectorXd x, MatrixXd B);

    /**
     * predicts the next note in the sequence s
     * @param s the sequence of notes
     * @param B the matrix of codebook vectors
     * @param N the matrix of distributions
     * @param t the matrix of D dimensional hypercube parameters
     * @param k the `shrinking' parameter
     * @return the next note in the sequence
     */
    int predictNextNote(vector<int> s, MatrixXd B, MatrixXd N, MatrixXd t, double k);

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
    int predictNextDir(vector<int> s, MatrixXd B, MatrixXd N, MatrixXd t, double k, int upInterval);

    /**
     * reads in a matrix from a csv file
     * @param filePath the path to the matrix file
     * @param rows the number of rows
     * @param cols the number of columns
     * @return the read-in matrix
     */
    MatrixXd readInMat(string filePath, int rows, int cols);

    //FIELDS
    MatrixXd BNote;
    MatrixXd NNote;
    MatrixXd tNote;
    double kNote;
    double TNote;

    MatrixXd BDir;
    MatrixXd NDir;
    MatrixXd tDir;
    double kDir;
    double TDir; //used differently to TNote

    //internal state of prediction
    vector<int> absSequence;
    vector<pair<int,double>> noteSequence;
    vector<int> dirSequence;
    int previousNote;

public:

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
    FPM(string BNotePath, string NNotePath, string tNotePath, double kNote, double TNote,
        string BDirPath, string NDirPath, string tDirPath, double kDir, double TDir);

    /**
     * adds a new note found to the queue
     * @param note the note (24-79)
     * @param duration the duration of said note
     */
    void queueNote(int note, double duration);

    /**
     * will generate a suitable response sequence
     * to that which has been queued up
     * the output will be of length of the queued notes
     * @return a matrix of notes and duration
     */
    MatrixXd combinedPredict();

    /**
     * member function wipes the arrays for the next round of prediction etc.
     */
    void clearState();

};

#endif //FYP_FPM_H
