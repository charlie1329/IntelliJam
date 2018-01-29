/**
 * file contains functions required to do simulated annealing
 * to train the weights of the echo state network
 * Author: Charlie Street
 */

#include "trainNetwork.h"
#include <random>

#define MAX_ITERATIONS 100000
#define DECREASE_RATE 1//0.999999
#define SIGMA 20

/**
 * function generates a new 'neighbouring' solution
 * @param currentSolution the current solution
 * @param gen the random generator
 * @param dis a normal distribution
 * @return the new solution
 */
MatrixXd generateNeighbour(MatrixXd currentSolution,
                           std::default_random_engine &gen, std::uniform_real_distribution<double> &dis);

/**
 * simultaneously the temperature schedule and the stopping condition for siumulated annealing
 * @param iteration the iteration number we are on
 * @return the current temperature value
 */
double temperatureSchedule(unsigned int iteration);

/**
 * function carries out simulated annealing
 * to learn the weights of a neural network
 * @param schedule the temperature schedule
 * @param neighbour a function to generate a neighbouring solution
 * @param echo the echo state network being trained
 * @param trainingSet the data set we are learning from
 * @return the minimum error found
 */
double simulatedAnnealing(double (*schedule)(unsigned int),MatrixXd(*neighbour)(MatrixXd,
                                                                                std::default_random_engine &,
                                                                                std::uniform_real_distribution<double> &),
                        shared_ptr<ESN> echo, training_set_t trainingSet);

#ifndef FYP_SIMULATED_ANNEALING_H
#define FYP_SIMULATED_ANNEALING_H

#endif //FYP_SIMULATED_ANNEALING_H
