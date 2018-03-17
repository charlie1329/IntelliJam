/**
 * attempting to find the fonseca and fleming function using my NSGA-ii implementation
 * Author: Charlie Street
 */

#include "../include/nsga-ii/nsga-ii.h"
#include "../include/Eigen/Dense"
#include "../include/nsga-ii/operators.h"
#include <fstream>
#include <iostream>

double obj1(Matrix2d W) {
    double sum;

    sum += pow((W(0,0) - (1.0/sqrt(3.0))),2.0);
    sum += pow((W(0,1) - (1.0/sqrt(3.0))),2.0);
    sum += pow((W(1,0) - (1.0/sqrt(3.0))),2.0);
    //sum += pow((W(1,1) - (1.0/sqrt(2.0))),2);

    return 1 - exp(-1 * sum);
}

double obj2(Matrix2d W) {
    double sum;

    sum += pow((W(0,0) + (1.0/sqrt(3.0))),2.0);
    sum += pow((W(0,1) + (1.0/sqrt(3.0))),2.0);
    sum += pow((W(1,0) + (1.0/sqrt(3.0))),2.0);
    //sum += pow((W(1,1) + (1.0/2)),2);

    return 1 - exp(-1 * sum);
}

bool lt (double x, double y) { return x < y;}

void randomGen(set<solution> &population, unsigned int N) {
    for(int i = 0; i < N; i++) {
        solution sol;
        sol.str = Matrix2d::Random();
        sol.str *= 4;
        population.insert(sol);
    }
}

//run the algorithm
int main() {
    vector<function<double (Matrix2d)>> objFuns;
    objFuns.push_back(&obj1);
    objFuns.push_back(&obj2);
    set<solution> pareto = runNSGA(randomGen,50,crossover, mutate,objFuns, lt, 200, 2, 1);

    ofstream myfile;
    myfile.open("output_fonseca.csv");


    for(auto it = pareto.begin(); it != pareto.end(); it++) {
        double first = (*it).objFunValues.at(0);
        double second = (*it).objFunValues.at(1);

        myfile << to_string(first) + "," + to_string(second) + "\n";

    }

    myfile.close();

}