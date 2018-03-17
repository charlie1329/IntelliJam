/**
 * this file implements functions prototypes defined in nsga-ii.h
 * Author: Charlie Street
 */

#include "../../include/nsga-ii/nsga-ii.h"
#include "../../include/nsga-ii/nonDomSort.h"

#include <random>
#include <iostream>


/**
 * function pre-computes all objective function values
 * for a given solution. This should save computational
 * effort at runtime
 * @param population the population of solutions
 * @param objFuns the multiple objective functions
 */
void calculateObjectives(set<solution> &population, vector<function<double (Matrix2d)>> objFuns) {

    //iterate through the population
    set<solution> newPop;
    typename set<solution>::iterator it;
    for(it = population.begin(); it != population.end(); it++) {
        solution cur = *it;
        cur.objFunValues.clear();
        for(unsigned int m = 0; m < objFuns.size(); m++) {
            //push objective value onto vector
            //of objective values for each solution
            cur.objFunValues.push_back(objFuns.at(m)(cur.str));
        }
        newPop.insert(cur);
    }
    population.clear();
    population = newPop;
}

/**
 * randomly generates an initial population of size N
 * @param population the set to store to, by doing this i reduce allocation
 * @param N the size of the population
 * @return the initial population
 */
void randomPopulation(set<solution> &population, unsigned int N) {

    for(int i = 0; i < N; i++) {
        solution currentSol;
        currentSol.str = Matrix2d::Random(); //in the range [-1; 1] which is what i want
        population.insert(currentSol);
    }
}

/**
 * function implements the nsga-ii algorithm
 * implemented from Deb (2001)
 * @param randGen a random generation function
 * @param generations the number of generations to run the algorithm
 * @param cross the crossover operator
 * @param mutate the mutation operator
 * @param objFuns the objective functions to optimise for
 * @param comp the comparison function for objective values
 * @param niche double for crossover
 * @param sigma standard deviation for mutation
 * @return the final pareto front
 */
set<solution> runNSGA(void (*randGen)(set<solution>&, unsigned int), unsigned int generations,
                         void(*cross)(Matrix2d,Matrix2d,Matrix2d&,Matrix2d&,double), void(*mutate)(Matrix2d&,double),
                         vector<function<double(Matrix2d)>> objFuns, bool(*comp)(double, double),
                         unsigned int N, double niche, double sigma) {

    //start by generating an initial population
    set<solution> P;
    randGen(P, N);
    //calculate objectives of parents
    calculateObjectives(P, objFuns);

    {
        vector<set<solution>> initialNonDom = nonDominatedSort(P, comp, N);
        P.clear();
        for (unsigned int i = 0; i < initialNonDom.size(); i++) {
            vector<solution> newRank(initialNonDom.at(i).begin(), initialNonDom.at(i).end());
            crowdingSort(newRank, getMinMaxValues(initialNonDom.at(i)), comp);
            for (unsigned int j = 0; j < newRank.size(); j++) {
                newRank.at(j).front = i;
                P.insert(static_cast<solution &&>(newRank.at(j)));
            }
            newRank.clear();
        }
        initialNonDom.clear();
    }
    assert(P.size() == N);

    //generate initial offspring for population
    set<solution> Q; //offspring population
    generateOffspring(P, Q, cross, mutate, niche, sigma);
    calculateObjectives(Q, objFuns); //calculate objectives of offspring
    set<solution> R; //combination of the parents and offspring for t-1


    //do for x generations
    for(int t = 0; t < generations; t++) {
        //run an iteration of the algorithm
        cout << "Generation: " + to_string(t) << endl;

        //copy together to do processing
        R.clear();
        for(auto it = P.begin(); it != P.end(); it++) {
            solution cur = (*it);
            R.insert(cur);
        }
        for(auto it = Q.begin(); it != Q.end(); it++) {
            solution cur = (*it);
            R.insert(cur);
        }

        assert(P.size() == N); //size checks
        assert(Q.size() == N);
        assert(R.size() == 2 * N);

        //do non-dominated sorting to generate new parent population
        P.clear(); //first empty P...
        Q.clear(); //and Q


        vector<set<solution>> sorted = nonDominatedSort(R, comp, N);
        cout << "Pareto front size: " + to_string(sorted.at(0).size()) << endl;
        unsigned int newSize = 0;
        unsigned int rank = 0;

        while(newSize + sorted.at(rank).size() <= N) {

            //set front rank of solutions and insert into population
            vector<solution> toSort(sorted.at(rank).begin(), sorted.at(rank).end());
            crowdingSort(toSort, getMinMaxValues(sorted.at(rank)), comp);
            for(int i = 0; i < toSort.size(); i++) {
                toSort.at(i).front = rank;
                P.insert(static_cast<solution &&>(toSort.at(i)));
            }
            toSort.clear();

            newSize += sorted.at(rank).size();
            rank++;
            if (rank >= sorted.size()) break;
        }

        if(P.size() < N) {
            vector<solution> toCrowd(sorted.at(rank).begin(), sorted.at(rank).end());
            crowdingSort(toCrowd, getMinMaxValues(sorted.at(rank)), comp);

            //fill remainder of population, ensuring we have parent population of N
            int leftToFill = N - P.size();
            for (unsigned int i = 0; i < leftToFill; i++) {
                toCrowd.at(i).front = rank;
                P.insert(static_cast<solution &&>(toCrowd.at(i)));
            }
            toCrowd.clear();
        }

        assert(P.size() == N);

        //precompute all objective values
        cout << "Started Computing Objectives..." << endl;
        calculateObjectives(P, objFuns);

        generateOffspring(P, Q, cross, mutate, niche, sigma);
        calculateObjectives(Q,objFuns);
        cout << "Finished Computing Objectives..." << endl;

        assert(Q.size() == N);
    }

    //combine to get final population
    set<solution> RFin;
    for(auto it = P.begin(); it != P.end(); it++) {
        solution cur = (*it);
        if(cur.front == 0) RFin.insert(cur);
    }

    //return best solutions in the final population
    return RFin;
}

/**this function calculates the population minimum and maximum objective values
 * for each objective function and stores it
 * fmin, fmax calculated as suggested in Deb(2001)
 * @param P the population itself
 * @return a vector of pairs, one for each objective function (in form, (min, max))
 */
vector<pair<double,double>> getMinMaxValues(set<solution> P) {
    assert(P.size() != 0);

    auto it = P.begin();
    vector<pair<double,double>> minMax((*it).objFunValues.size());

    //set appropriate initial values
    for(unsigned int m = 0; m < minMax.size(); m++) {
        minMax.at(m).first = numeric_limits<double>::max();
        minMax.at(m).second = numeric_limits<double>::min();
    }

    //find min and max of population objective values in a linear way
    for(it = P.begin(); it != P.end(); it++) {

        assert(minMax.size() == (*it).objFunValues.size());

        for(unsigned int m = 0; m < (*it).objFunValues.size(); m++) {
            double current = (*it).objFunValues.at(m);
            if(current > minMax.at(m).second) minMax.at(m).second = current;
            if(current < minMax.at(m).first) minMax.at(m).first = current;
        }
    }

    return minMax;
};

/**
 * function generates a set of offspring given a parent population
 * @param P the parent population
 * @param Q the child population to be filled
 * @param cross the crossover operator
 * @param mutate the mutation operator
 * @param niche the niche count for the crossover operator
 * @param sigma the standard deviation for the mutation (if needed)
 */
void generateOffspring(set<solution> P, set<solution> &Q,
                       void(*cross)(Matrix2d,Matrix2d,Matrix2d&,Matrix2d&,double),
                       void(*mutate)(Matrix2d&,double),
                       double niche, double sigma) {

    //create a uniform distribution for sampling from P
    uniform_int_distribution<> dis(1, P.size() - 1);
    random_device rd;
    mt19937 gen(rd());

    //for tournament selection and selecting who goes against who
    int seed1 = dis(gen);
    int seed2 = dis(gen);
    while(seed1 == seed2 || ((seed1-seed2) %2 == 0)) seed2 = dis(gen); //ensure different values and odd difference
    //the odd difference prevents repeats assuming even sized population
    vector<solution> PVec(P.begin(), P.end());
    PVec.reserve(P.size());

    //create the mating pool
    vector<solution> matingPool;
    matingPool.reserve(P.size());
    vector<int> checker(P.size(), 2);
    for(unsigned int i = 0; i < PVec.size(); i++) {
        if(checker.at(i) == 0) continue;
        unsigned int index1 = (i + seed1) % PVec.size();
        unsigned int index2 = (i + seed2) % PVec.size();
        matingPool.push_back(tournamentSelection(PVec.at(index1), PVec.at(index2)) ? PVec.at(index1) : PVec.at(index2));
    }
    assert(matingPool.size() == P.size());

    //I feel it may be worth shuffling the mating pool around a bit
    shuffle(matingPool.begin(), matingPool.end(), gen);

    //for each two items in the mating pool, generate two offspring
    //then mutate them and add to the new child population Q_t+1
    for(unsigned int i = 0; i < matingPool.size(); i+=2) {

        Matrix2d mum = matingPool.at(i).str;
        Matrix2d dad = matingPool.at(i+1).str;

        Matrix2d boy;
        Matrix2d girl;

        //run crossover operator to generate offspring
        cross(mum, dad, boy, girl, niche);

        //mutate the offspring
        mutate(boy, sigma);
        mutate(girl, sigma);

        //add the offspring to population
        solution boySol;
        boySol.str = boy;
        Q.insert(boySol);

        solution girlSol;
        girlSol.str = girl;
        Q.insert(girlSol);
    }
    assert(Q.size() == P.size());
}



