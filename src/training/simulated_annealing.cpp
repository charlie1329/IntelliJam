/**
 * file implements functions from simulated_annealing.h
 * Author: Charlie Street
 */

#include "../../include/training/simulated_annealing.h"
#include <iostream>
#include <cmath>
#include <chrono>
#include <utility>

/**
 * this function avoids eigens poor random initialisation
 * by using a random number generator with a unique(ish) seed
 * @param rows the rows of the matrix
 * @param cols the columns of the matrix
 * @param range the absolute value range of the matrix
 * @return the randomly initialised matrix
 */
MatrixXd generateRandomMatrix(unsigned int rows, unsigned int cols, unsigned int range) {

    MatrixXd randMatrix = MatrixXd::Zero(rows,cols);

    //set up random number generator
    std::default_random_engine gen;
    gen.seed((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> dis;

    //initialise matrix
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            randMatrix(i,j) = dis(gen) * range;
        }
    }

    return randMatrix;
}

/**
 * function carries out simulated annealing
 * to learn the weights of a neural network
 * @param schedule the temperature schedule
 * @param neighbour a function to generate a neighbouring solution
 * @param echo the echo state network being trained
 * @param trainingSet the data set we are learning from
 * @return minimum training set error
 */
double simulatedAnnealing(double (*schedule)(unsigned int),MatrixXd(*neighbour)(MatrixXd,
                                                                              std::default_random_engine &,
                                                                              std::uniform_real_distribution<double> &),
                        shared_ptr<ESN> echo, training_set_t trainingSet) {

    //initialise random distribution for deciding whether to take bad moves
    std::default_random_engine generator;
    generator.seed((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> distribution(0.0,1.0);


    //initialise normal distribution for use in generating neighbours
    std::default_random_engine normalGen;
    generator.seed((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> normalDis(-SIGMA,SIGMA);


    MatrixXd currentSolution = generateRandomMatrix((unsigned int)echo->resOutWeights.rows(),
                                                    (unsigned int)echo->resOutWeights.cols(), 100);
    echo->resOutWeights = currentSolution;
    double currentError = getError(echo,trainingSet);

    //initial minimum is the initial solution
    MatrixXd minSolution = currentSolution;
    double minError = currentError;

    double T; //temperature value
    unsigned int iterations = 0;

    while((T = schedule(iterations)) != 0.0) { //while temperature not 0

        std::cout << "New Iteration: " << iterations << std::endl;

        MatrixXd newSolution = neighbour(currentSolution,normalGen,normalDis); //generate new solution and evaluate it
        echo->resOutWeights = newSolution;
        double newError = getError(echo,trainingSet);


        if(newError < currentError) { //if better, set current to new
            currentSolution = newSolution;
            currentError = newError;

            if(currentError < minError) { //if we've found a new minimum, set it and write to file
                minSolution = currentSolution;
                minError = currentError;
                echo->resOutWeights = minSolution;
                echo->saveNetwork();
                std::cout << "Found new Minimum: " << minError << std::endl;
            }

        } else { //sometimes accept worse off solutions
            double p = exp((currentError-newError)/T);
            double pPrime = distribution(generator);
            if(p > pPrime) { //in this case accept the bad move in the name of exploration
               currentSolution = newSolution;
               currentError = newError;
            }

        }

        std::cout << "Current Error: " << currentError << std::endl;
        iterations++; //increment our number of iterations
    }

    return minError;
}

/**
 * simultaneously the temperature schedule and the stopping condition for siumulated annealing
 * @param iteration the iteration number we are on
 * @return the current temperature value
 */
double temperatureSchedule(unsigned int iteration) {
    if(iteration > MAX_ITERATIONS) return 0.0;

    return pow(DECREASE_RATE,iteration); //exponentially decreasing
}

/**
 * function generates a new 'neighbouring' solution
 * @param currentSolution the current solution
 * @param gen the random generator
 * @param dis a normal distribution
 * @return the new solution
 */
MatrixXd generateNeighbour(MatrixXd currentSolution,
                           std::default_random_engine &gen, std::uniform_real_distribution<double> &dis) {

    MatrixXd newSolution = std::move(currentSolution);
    for(int i = 0; i < newSolution.rows(); i++) {
        for(int j = 0; j < newSolution.cols(); j++) {
            newSolution(i,j) += (dis(gen)); //add on some random movement
        }

    }


    return newSolution;
}
