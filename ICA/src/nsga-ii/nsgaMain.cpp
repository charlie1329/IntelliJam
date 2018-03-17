/**
 * this is the file that runs the nsga algorithm for me
 * and stores the solution/
 * Author: Charlie Street
 */

#include "../../include/nsga-ii/nsgaMain.h"
#include "../../include/nsga-ii/nsga-ii.h"
#include "../../include/nsga-ii/hyperParameters.h"
#include "../../include/nsga-ii/objectives.h"
#include "../../include/nsga-ii/operators.h"
#include "../../include/Eigen/Dense"
#include "../../include/audioFileHelpers.h"
#include "../../include/ICAHelpers.h"

#include <iostream>
#include <set>

using namespace Eigen;

#define NSGA_DAMPENING 0.05 //not sure if I will need or not but may be useful

/**
 * main file runs the nsga-ii algorithm on a particular track
 * @param argc number of arguments
 * @param argv the arguments
 * @return exit code
 */
int main(int argc, char **argv) {

    generateSolutions("../../../Documents/fyp/extraction_tests/original_audio/metal.wav", "../../../Documents/fyp/extraction_tests/NSGA2/metal");
    return 0;
}

//pretty self-explanatory
bool lt (double x, double y) {return x < y;}

/**
 * function runs nsga-ii and does all pre/post processing
 * @param inputPath the sound file to use
 * @param outputPrefix the prefix for each source file output
 */
void generateSolutions(string inputPath, string outputPrefix) {

    cout << "Starting..." << endl;

    //read in sound file
    shared_ptr<audioBuffers> stereoRead = readInFromFile(move(inputPath));

    //now into an eigen matrix
    Matrix<double,SPEAKERS,Dynamic> X = fromBuffers(*stereoRead);
    Matrix<double,SPEAKERS,Dynamic> XOriginal = X;

    cout << "Read In The Data..." << endl;

    //subtract mean and whiten the data
    Matrix<double,SPEAKERS,1> m = subtractMean(X);
    shared_ptr<EigenDecomp> eigen = whitenData(X);

    //generate objective functions
    vector<function<double(Matrix2d)>> objectiveFunctions;
    function<double(Matrix2d)> statPartial = [=](Matrix2d W) -> double {return statIndependenceKurtObj(W,X);};
    function<double(Matrix2d)> skewPartial = [=](Matrix2d W) -> double {return skewnessObj(W,X);};
    objectiveFunctions.push_back(statPartial);
    objectiveFunctions.emplace_back(&spectrumCentreObj);
    objectiveFunctions.emplace_back(&stereoSumObj);

    //run algorithm to get pareto-optimal front
    set<solution> paretoFront = runNSGA(randomPopulation,GENERATIONS,crossover,mutate,objectiveFunctions,
                                                    lt,POPULATION_SIZE,NICHE_COUNT,STANDARD_DEVIATION);



    cout << "Obtained Pareto Optimal Front..." << endl;
    //for each solution write sources to file

    unsigned int sol = 0;
    Matrix<double,2,Dynamic> A;
    Matrix<double,Dynamic,Dynamic> s;
    for(auto it = paretoFront.begin(); it != paretoFront.end(); it++) {
        A = ((*it).str).inverse();
        unwhitenData(A, *eigen); //unwhiten the data

        s = A.inverse() * XOriginal;

        addMean(s, A, m);

        vector<vector<double>> sVec(2);

        for(int i = 0; i < s.cols(); i++) {
            sVec.at(0).push_back(s(0,i) * NSGA_DAMPENING);
            sVec.at(1).push_back(s(1,i) * NSGA_DAMPENING);
        }

        cout << "Writing solution " + to_string(sol) + " to file..." << endl;

        writeToFile(outputPrefix + to_string(sol), sVec);

        sol++;
    }

}
