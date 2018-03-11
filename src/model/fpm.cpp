/**
 * file implements class functionality found within fpm.h
 * Author: Charlie Street
 */

#include <utility>
#include <cmath>
#include <limits>
#include <chrono>
#include <fstream>
#include "../../include/model/fpm.h"
#include <iostream>

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
       x = applyMap(t.col(note),k,x); //TODO: Test this works properly!
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

    double maxVal = distribution.maxCoeff();

    distribution /= maxVal;

    double totalSamples = 0;
    for(int j = 0; j < distribution.cols(); j++) {
        totalSamples += distribution(0,j);
    }

    //set up a random generator
    uniform_real_distribution<double> rng(0,totalSamples);

    //do some random sampling to generate the note
    double randNo = rng(gen);

    double total = 0;
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
    //bring sequence to chaos representation
    VectorXd x = toChaosRep(std::move(t),k, std::move(s));

    //now find the closest codebook vector
    int i = findClosestCodebook(x, std::move(B));

    //apply interval based cost
    RowVectorXd distribution = N.row(i);
    if(upInterval > 6 && upInterval != 12) {
        distribution(0,1) = round(distribution(0,1) * exp(-(upInterval-6.0)/TDir));
    } else if(upInterval < 6 && upInterval != 0){
        distribution(0,0) = round(distribution(0,0) * exp(-(6.0-upInterval)/TDir));
    }

    auto totalSamples = (int)distribution.sum();

    //set up a random generator
    //default_random_engine gen;
   // gen.seed((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> rng(1,totalSamples);

    //actually generate a random number and use that to determine value
    int randNo = rng(gen);
    if(randNo <= distribution(0,0)) return 0;

    return 1;

}

/**
 * reads in a matrix from a csv file
 * @param filePath the path to the matrix file
 * @param rows the number of rows
 * @param cols the number of columns
 * @return the read-in matrix
 */
MatrixXd FPM::readInMat(string filePath, int rows, int cols) {
    vector<vector<double>> temp; //temporary storage for matrix values

    ifstream matFile(filePath);
    string item;

    vector<double> currentVector;

    //read item by item
    while(getline(matFile,item,',')) {

        if (!item.compare("\n")) { //exit condition, item only has a newline
            temp.push_back(currentVector);
            currentVector.clear();
            break;
        }

        if (item.find('\n') != string::npos) { //if we reach a new line
            temp.push_back(currentVector);
            currentVector.clear();
        }


        double currentVal = stod(item, nullptr);
        currentVector.push_back(currentVal);

    }

    //now write into the matrix
    MatrixXd mat = MatrixXd::Zero(rows,cols);

    for(unsigned int i = 0; i < rows; i++) {
        for(unsigned int j = 0; j < cols; j++) {
            mat(i,j) = temp.at(i).at(j);
        }
    }

    return mat;
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

    //initialise previousNote
    previousNote = -1;

    //seed random generator
    gen.seed((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());

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

    if(previousNote != -1 && previousNote != note) {
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

    //due to timer, we may end on 0, and we don't want this
    if(absSequence.at(absSequence.size()-1) == 0) {
        absSequence.pop_back();
        noteSequence.pop_back();
        //due to way things are dealt with with direction, we don't need to pop from here
    }

    //do the key detection on the users played phrase
    vector<pair<pair<int,int>,string>> segmentsAndKeys = detectKey(noteSequence,SEGMENT_LENGTH,MODULATION_PENALTY);
    vector<int> transposed; //will store transposed phrase
    vector<int> noDuration; // a temporary copy


    //copy notes into no Duration
    for (auto currentPair : noteSequence) {
        noDuration.push_back(currentPair.first);
    }

    //do all the transposing to C
    for (const auto &segment : segmentsAndKeys) {
        vector<int> slice(noDuration.begin()+segment.first.first,noDuration.begin()+segment.first.second);
        vector<int> currentSegment = transpose(slice,segment.second,"C");
        transposed.insert(transposed.end(),currentSegment.begin(),currentSegment.end());
    }


    string endKey = segmentsAndKeys.at(segmentsAndKeys.size()-1).second; //get the key to transpose back to

    //now generate the note sequence
    int outputLen = absSequence.size();
    int startPointNote = transposed.size();
    for(int i = 0; i < outputLen; i++) { //generate a sequence equal in size to that which the user played
        transposed.push_back(predictNextNote(transposed,BNote,NNote,tNote,kNote));
    }

    //now transpose back to the original key
    transposed = transpose(transposed,"C",endKey);
    vector<int> predictedSequence(transposed.begin()+startPointNote,transposed.end());

    //now generate the direction sequence to generate the end sequence
    int startPointOut = absSequence.size();
    int prevNote = absSequence.at(absSequence.size()-1);
    for(unsigned int i = 0; i < outputLen; i++) {
        int upInterval = mod((mod(predictedSequence.at(i) - 1,12) - mod(prevNote,12)),12);
        int newDirection = predictNextDir(dirSequence,BDir,NDir,tDir,kDir,upInterval);

        if(predictedSequence.at(i) == 0) { //silence
            absSequence.push_back(0);
            dirSequence.push_back(newDirection);
        } else if(mod(predictedSequence.at(i)-1,12) == mod(prevNote,12)) { //same note
            //try again
            int newNote = prevNote;
            int secondDraw = predictNextDir(dirSequence,BDir,NDir,tDir,kDir,upInterval);
            dirSequence.push_back(newDirection);

            if(secondDraw == 1 && newDirection == 1) { //if both draws are the same, move in that direction
                newNote += 12;
                if(newNote > 79) newNote = prevNote;
            } else if(secondDraw == 0 && newDirection == 0) {
                newNote -= 12;
                if(newNote < 24) newNote = prevNote;
            }

            absSequence.push_back(newNote);
            prevNote = newNote;
        } else { //standard case
            dirSequence.push_back(newDirection);
            int predictedMod = mod((predictedSequence.at(i)),12);
            int previousMod = mod(prevNote,12);
            int newNote;

            if(newDirection == 1) {
                newNote = prevNote + mod(predictedMod - previousMod,12);
            } else {
                newNote = prevNote - mod(previousMod - predictedMod,12);
            }

            if(newNote < 24) newNote += 12;
            if(newNote > 79) newNote -= 12;

            absSequence.push_back(newNote);
            prevNote = newNote;
        }
    }

    //put into form for return value
    //TODO: Rhythmic modifications here!
    MatrixXd returnPhrase = MatrixXd::Zero(outputLen,2);
    for(unsigned int i = 0; i < outputLen; i++) {
        returnPhrase(i,0) = absSequence.at(startPointOut + i);
        returnPhrase(i,1) = noteSequence.at(i).second;
    }

    cout << "FINAL MATRIX: " << endl;
    cout << returnPhrase << endl;

    clearState(); // prediction messes with state a bit, so clear it up

    return returnPhrase;
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

//SIMPLE GET FUNCTIONS
MatrixXd FPM::getNNote() {
    return NNote;
}
MatrixXd FPM::getBNote() {
    return BNote;
}
MatrixXd FPM::gettNote() {
    return tNote;
}
MatrixXd FPM::getNDir() {
    return NDir;
}
MatrixXd FPM::getBDir() {
    return BDir;
}
MatrixXd FPM::gettDir() {
    return tDir;
}

vector<int> FPM::getAbsQueue() {
    return absSequence;
}
vector<pair<int,double>> FPM::getNoteQueue() {
    return noteSequence;
};
vector<int> FPM::getDirQueue() {
    return dirSequence;
}