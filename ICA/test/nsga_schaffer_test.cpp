/**
 * attempting to find the schaffer function using my NSGA-ii implementation
 * Author: Charlie Street
 */

#include "../include/nsga-ii/nsga-ii.h"
#include "../include/Eigen/Dense"
#include "../include/nsga-ii/operators.h"
#include <fstream>
#include <iostream>

double obj1(Matrix2d W) {
    return pow(W(0,0),2.0);
}

double obj2(Matrix2d W) {
    return pow(W(0,0) - 2.0, 2.0);
}

bool lt (double x, double y) { return x < y;}

void randomGen(set<solution> &population, unsigned int N) {
    for(int i = 0; i < N; i++) {
        solution sol;
        sol.str = Matrix2d::Random();
        sol.str *= 10;
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
    myfile.open("output.csv");


    for(auto it = pareto.begin(); it != pareto.end(); it++) {
        double first = (*it).objFunValues.at(0);
        double second = (*it).objFunValues.at(1);

        myfile << to_string(first) + "," + to_string(second) + "\n";

    }

    myfile.close();

}