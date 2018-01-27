/**
 * file implements functions from simulated_annealing.h
 * Author: Charlie Street
 */

#include "../../include/training/simulated_annealing.h"

/**
 * function carries out simulated annealing
 * to learn the weights of a neural network
 * @param schedule the temperature schedule
 * @param neighbour a function to generate a neighbouring solution
 * @param echo the echo state network being trained
 * @param trainingSet the data set we are learning from
 */
void simulatedAnnealing(double (*schedule)(unsigned int),MatrixXd(*neighbour)(MatrixXd),
                        shared_ptr<ESN> echo, training_set_t trainingSet) {

    //set initial solution space to be [-300,300]
    MatrixXd currentSolution = MatrixXd::Random(echo->resOutWeights.rows(),echo->resOutWeights.cols()) * 300;
    echo->resOutWeights = currentSolution;
    double currentError = getError(echo,trainingSet);

    //initial minimum is the initial solution
    MatrixXd minSolution = currentSolution;
    double minError = currentError;

    double T; //temperature value
    unsigned int iterations = 0;

    while((T = schedule(iterations)) != 0.0) { //while temperature not 0

        MatrixXd newSolution = neighbour(currentSolution); //generate new solution and evaluate it
        echo->resOutWeights = newSolution;
        double newError = getError(echo,trainingSet);

        if(newError < currentSolution) { //if better, set current to new
            currentSolution = newSolution;
            currentError = newError;

            if(currentError < minError) { //if we've found a new minimum, set it and write to file
                minSolution = currentSolution;
                minError = currentError;
                echo->resOutWeights = minSolution;
                echo->saveNetwork();

        } else { //sometimes accept worse off solutions
            //crazy temperature probability stuff here
        }

        iterations++; //increment our number of iterations
    }
}