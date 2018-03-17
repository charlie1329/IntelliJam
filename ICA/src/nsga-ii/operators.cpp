/**
 * implements functions from operators.h
 * Author: Charlie Street
 */

#include "../../include/nsga-ii/operators.h"
#include <limits> //infinity for doubles is necessary for crowd sort
#include <algorithm>
#include <random>
#include <iostream>

/**
 * carries out the tournament selection operator
 * @param i a solution
 * @param j a solution
 * @return true if i wins, false if j wins
 */
bool tournamentSelection(solution i, solution j) {
    if(i.front < j.front) {
        return true;
    } else if (i.front == j.front && i.crowdDistance > j.crowdDistance) {
        return true;
    }

    return false; //j wins
}

/**
 * function sorts by crowding distance
 * reason being we want the most diverse set of solutions
 * on the pareto optimal front
 * implemented from Deb (2001)
 * @param F the vector of our solutions (with room to add crowding distance for later)
 * @param comp the comparison function for the objective functions
 * @param minMax population mins, and maxes for each objective function
 */
void crowdingSort(vector<solution> &F, vector<pair<double,double>> minMax, bool (*comp)(double,double)) {


    //step C1
    unsigned int l = F.size();
    for(int i = 0; i < l; i++) { //set all distances to 0 initially
        F.at(i).crowdDistance = 0.0;
    }

    //step c2/c3: for each objective function, sort from worst to best
    //then assign distance values appropriately
    for(int m = 0; m < F.at(0).objFunValues.size(); m++) {
        sort(F.begin(), F.end(), [=](solution i, solution j) { //sort in worst order
            return comp(i.objFunValues.at(m), j.objFunValues.at(m));
        } );


        //assign large distance to boundary solutions
        F.at(0).crowdDistance = numeric_limits<double>::infinity();
        F.at(l-1).crowdDistance = numeric_limits<double>::infinity();

        //assign calculated distance for all other solutions
        for(int j = 1; j < l-1; j++) {
            F.at(j).crowdDistance += (F.at(j + 1).objFunValues.at(m) - F.at(j - 1).objFunValues.at(m))
                       /(minMax.at(m).second - minMax.at(m).first);
        }
    }


    //sort by descending crowd distance
    sort(F.begin(), F.end(), [=](solution i, solution j) {
       return i.crowdDistance > j.crowdDistance;
    });
}


/** generates two children solutions based on parents
 * implemented from Deb (2001)
 * note I am trying to reduce how many matrix copies are made
 * @param mum parent 1
 * @param dad parent 2
 * @param boy child 1
 * @param girl child 2
 * @param niche the niche count (determines width in some respects)
 * @return a pair of children
 */
void crossover(Matrix2d mum, Matrix2d dad,
               Matrix2d &boy, Matrix2d &girl, double niche) {

    //initialise random generator stuff
    //credit to: http://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
    random_device rd;
    mt19937 gen(rd()); //for seeds i believe
    uniform_real_distribution<double> dis(0.0, 1.0); //range [0 1)

    //for each element in the matrices, do the cross-over
    for(int i = 0; i < 4; i++) {
        double mu = dis(gen);

        double beta;
        if(mu <= 0.5) {
            beta = pow(2*mu, 1.0/(niche+1.0));
        } else {
            beta = pow(1.0/(2*(1 - mu)), 1.0/(niche+1.0));
        }

        //getting matrix index
        int row = i / 2;
        int col = i % 2;

        double boy_gene = 0.5 * (((1 + beta) * mum(row,col)) + ((1 - beta) * dad(row,col)));
        double girl_gene = 0.5 * (((1 - beta) * mum(row,col)) + ((1 + beta) * dad(row,col)));

        boy(row,col) = boy_gene;
        girl(row,col) = girl_gene;
    }
}

/** carries out a mutation on an (offspring) solution
 * normally distributed mutation from Deb (2001)
 * @param sol the solution to be mutated
 * @param sigma the standard deviation (same in all dimensions)
 */
void mutate(Matrix2d &sol, double sigma) {
    //initialise normal distribution generator
    //credit to: http://www.cplusplus.com/reference/random/normal_distribution/
    default_random_engine gen;
    normal_distribution<double> dis(0.0,sigma);

    //go through each matrix element
    //and mutate slightly with normal distribution
    for(int i = 0; i < 4; i++) {
        sol(i/2,i%2) += dis(gen);
    }
}

