/**
 * this file contains the unit tests for
 * the NSGA-II algorithm and its associated functions
 * Author: Charlie Street
 */

#define CATCH_CONFIG_MAIN

#include <Eigen/Dense>
#include "../include/catch.hpp"
#include "../include/nsga-ii/nonDomSort.h"
#include "../include/nsga-ii/nsga-ii.h"
#include "../include/nsga-ii/objectives.h"
#include "../include/nsga-ii/operators.h"
#include "../include/nsga-ii/hyperParameters.h"
#include <limits>
#include <set>

using namespace Eigen;
using namespace std;

bool lt(double x, double y) {return x < y;}

double obj1(Matrix2d W) {
    return W(0,0) + W(0,1) + W(1,0) + W(1,1);
}

double obj2(Matrix2d W) {
    return 2 * obj1(W);
}

/* to be tested
 * nonDomSort.h (TESTS complete)
 * -- dominates (DONE)
 * -- findParetoFront (DONE)
 * -- nonDominatedSort (DONE)
 * nsga-ii.h (TESTS COMPLETE AS MUCH AS POSSIBLE)
 * -- randomPopulation (DONE)
 * -- runNSGA (NON-DETERMINISTIC)
 * -- calculateObjectives (DONE)
 * -- getMinMaxValues (DONE)
 * -- generateOffspring (NON-DETERMINISTIC)
 * objectives.h (TESTS complete)
 * -- statIndependenceKurtObj (DONE)
 * -- spectrumCentreObj (DONE)
 * -- stereoSumObj (DONE)
 * operators.h (TESTS COMPLETE)
 * -- tournamentSelection (DONE)
 * -- crowdingSort (DONE)
 * -- crossover (DONE)
 * -- mutate (DONE)
 */


TEST_CASE("tests the objective functions to be worked on 2x2 matrices","[objectives]") {
   //test spectrum centre obj
    Matrix2d testMatrix;

    testMatrix(0,0) = -1.5;
    testMatrix(0,1) = 3.5;
    testMatrix(1,0) = 2.5;
    testMatrix(1,1) = -2.5;

    double centreTest = spectrumCentreObj(testMatrix);
    CHECK(centreTest == Approx(0.0));

    testMatrix(0,0) = 1.0;
    testMatrix(0,1) = 0.0;
    testMatrix(1,0) = 0.0;
    testMatrix(1,1) = 1.0;

    CHECK(spectrumCentreObj(testMatrix) == Approx(1.0));


    //test stereoSum Obj
    testMatrix(0,0) = 1.0;
    testMatrix(0,1) = 2.0;
    testMatrix(1,0) = 3.0;
    testMatrix(1,1) = 4.0;

    CHECK(stereoSumObj(testMatrix) == Approx(2.0));

    //test statIndependenceKurtObj
    testMatrix(0,0) = 1.0; //identity matrix makes hand calculations easier
    testMatrix(0,1) = 0.0;
    testMatrix(1,0) = 0.0;
    testMatrix(1,1) = 1.0;

    Matrix<double,2,3> X;
    X(0,0) = 1.0;
    X(0,1) = 2.0;
    X(0,2) = 2.0;
    X(1,0) = 2.0;
    X(1,1) = 2.0;
    X(1,2) = 4.0;

    CHECK(statIndependenceKurtObj(testMatrix,X) == Approx(9899.0));

}

TEST_CASE("tests the operators in operators.h", "[operators]") {

    //test mutate
    Matrix2d testMatrix;

    testMatrix(0,0) = 1;
    testMatrix(0,1) = 0;
    testMatrix(1,0) = 0;
    testMatrix(1,1) = 1;

    Matrix2d copy = testMatrix;

    mutate(testMatrix,STANDARD_DEVIATION);

    bool test1 = (testMatrix(0,0) == Approx(copy(0,0)));
    bool test2 = (testMatrix(0,1) == Approx(copy(0,1)));
    bool test3 = (testMatrix(1,0) == Approx(copy(1,0)));
    bool test4 = (testMatrix(1,1) == Approx(copy(1,1)));

    CHECK_FALSE(test1);
    CHECK_FALSE(test2);
    CHECK_FALSE(test3);
    CHECK_FALSE(test4);

    //test tournament selection
    solution sol1;
    sol1.front = 1;
    sol1.crowdDistance = 0.5;

    solution sol2;
    sol2.front = 3;
    sol2.crowdDistance = 0.7;

    CHECK(tournamentSelection(sol1, sol2));
    CHECK_FALSE(tournamentSelection(sol2, sol1));

    sol2.front = 1;
    CHECK_FALSE(tournamentSelection(sol1, sol2));
    CHECK(tournamentSelection(sol2, sol1));

    sol1.crowdDistance = numeric_limits<double>::infinity();
    CHECK(tournamentSelection(sol1, sol2));
    CHECK_FALSE(tournamentSelection(sol2, sol1));


    //test crowding sort

    solution sol6;
    sol6.objFunValues.push_back(0.31);
    sol6.objFunValues.push_back(6.10);

    solution sol7;
    sol7.objFunValues.push_back(0.22);
    sol7.objFunValues.push_back(7.09);

    solution sol8;
    sol8.objFunValues.push_back(0.79);
    sol8.objFunValues.push_back(3.97);

    solution sol9;
    sol9.objFunValues.push_back(0.27);
    sol9.objFunValues.push_back(6.93);

    vector<solution> pop;
    pop.push_back(sol6);
    pop.push_back(sol7);
    pop.push_back(sol8);
    pop.push_back(sol9);


    vector<pair<double,double>> minMax;
    minMax.push_back(make_pair(0.1,1.0));
    minMax.push_back(make_pair(0.0,60.0));

    crowdingSort(pop, minMax, lt);

    CHECK(pop.at(0).crowdDistance == numeric_limits<double>::infinity());
    CHECK(pop.at(1).crowdDistance == numeric_limits<double>::infinity());
    CHECK(pop.at(2).crowdDistance == Approx(0.63).margin(0.005));
    CHECK(pop.at(3).crowdDistance == Approx(0.12).margin(0.005));

    //test crossover
    //IMPORTANT: have to manually set mu in order to test this
    //Therefore, once I set it back to not be 0.5, then the test will fail

    Matrix2d mat1;
    mat1(0,0) = 1.0;
    mat1(0,1) = 2.0;
    mat1(1,0) = 3.0;
    mat1(1,1) = 4.0;

    Matrix2d mat2;
    mat2(0,0) = 2.0;
    mat2(0,1) = 3.0;
    mat2(1,0) = 4.0;
    mat2(1,1) = 5.0;

    Matrix2d boy;
    Matrix2d girl;

    crossover(mat1,mat2,boy,girl,2);

    //TESTED WITH mu = 0.9375 as of 24/10/17
    /*CHECK(boy(0,0) == Approx(0.5));
    CHECK(boy(0,1) == Approx(1.5));
    CHECK(boy(1,0) == Approx(2.5));
    CHECK(boy(1,1) == Approx(3.5));

    CHECK(girl(0,0) == Approx(2.5));
    CHECK(girl(0,1) == Approx(3.5));
    CHECK(girl(1,0) == Approx(4.5));
    CHECK(girl(1,1) == Approx(5.5));*/


}

TEST_CASE("tests the functions for non dominated sorting", "[nonDomSort]") {

    //test dominates
    solution sol1;
    sol1.objFunValues.push_back(0.5);
    sol1.objFunValues.push_back(0.7);
    sol1.objFunValues.push_back(0.9);

    solution sol2;
    sol2.objFunValues.push_back(0.5);
    sol2.objFunValues.push_back(0.7);
    sol2.objFunValues.push_back(0.9);

    CHECK_FALSE(dominates(sol1,sol2,lt));
    CHECK_FALSE(dominates(sol2,sol1,lt));

    solution sol3;
    sol3.objFunValues.push_back(0.3);
    sol3.objFunValues.push_back(0.7);
    sol3.objFunValues.push_back(0.3);
    CHECK(dominates(sol3,sol1,lt));
    CHECK_FALSE(dominates(sol1,sol3,lt));

    solution sol4;
    sol4.objFunValues.push_back(0.3);
    sol4.objFunValues.push_back(0.8);
    sol4.objFunValues.push_back(0.3);
    CHECK_FALSE(dominates(sol4,sol1,lt));
    CHECK_FALSE(dominates(sol1,sol4,lt));

    solution sol5;
    sol5.objFunValues.push_back(0.3);
    sol5.objFunValues.push_back(0.7);
    sol5.objFunValues.push_back(0.9);
    CHECK(dominates(sol5,sol1,lt));
    CHECK_FALSE(dominates(sol1,sol5,lt));

    //test findParetoFront
    solution sol6;
    sol6.objFunValues.push_back(1.0);
    sol6.objFunValues.push_back(1.0);

    solution sol7;
    sol7.objFunValues.push_back(0.0);
    sol7.objFunValues.push_back(2.0);

    solution sol8;
    sol8.objFunValues.push_back(3.0);
    sol8.objFunValues.push_back(3.0);

    solution sol9;
    sol9.objFunValues.push_back(2.0);
    sol9.objFunValues.push_back(2.0);

    solution sol10;
    sol10.objFunValues.push_back(4.0);
    sol10.objFunValues.push_back(3.0);

    set<solution> P;
    P.insert(sol6);
    P.insert(sol7);
    P.insert(sol8);
    P.insert(sol9);
    P.insert(sol10);

    set<solution> pareto = findParetoFront(P, lt);

    REQUIRE(pareto.size() == 2);

    auto it = pareto.begin();
    solution first = *it;
    if(first.objFunValues.at(0) == 1.0) {
        CHECK(first.objFunValues.at(0) == Approx(1.0));
        CHECK(first.objFunValues.at(1) == Approx(1.0));
        solution second = *(++it);
        CHECK(second.objFunValues.at(0) == Approx(0.0));
        CHECK(second.objFunValues.at(1) == Approx(2.0));
    } else {
        CHECK(first.objFunValues.at(0) == Approx(0.0));
        CHECK(first.objFunValues.at(1) == Approx(2.0));
        solution second = *(++it);
        CHECK(second.objFunValues.at(0) == Approx(1.0));
        CHECK(second.objFunValues.at(1) == Approx(1.0));
    }

    //testing nonDominatedSort
    vector<set<solution>> nonDomSorted = nonDominatedSort(P,lt,5);

    REQUIRE(nonDomSorted.size() == 4);

    REQUIRE(nonDomSorted.at(0).size() == 2);
    auto it1 = nonDomSorted.at(0).begin();
    if((*it1).objFunValues.at(0) == 1.0) {
        CHECK((*it1).objFunValues.at(0) == 1.0);
        CHECK((*it1).objFunValues.at(1) == 1.0);
        it1++;
        CHECK((*it1).objFunValues.at(0) == 0.0);
        CHECK((*it1).objFunValues.at(1) == 2.0);
    } else {
        CHECK((*it1).objFunValues.at(0) == 0.0);
        CHECK((*it1).objFunValues.at(1) == 2.0);
        it1++;
        CHECK((*it1).objFunValues.at(0) == 1.0);
        CHECK((*it1).objFunValues.at(1) == 1.0);
    }

    REQUIRE(nonDomSorted.at(1).size() == 1);
    auto it2 = nonDomSorted.at(1).begin();
    CHECK((*it2).objFunValues.at(0) == 2.0);
    CHECK((*it2).objFunValues.at(1) == 2.0);

    REQUIRE(nonDomSorted.at(2).size() == 1);
    auto it3 = nonDomSorted.at(2).begin();
    CHECK((*it3).objFunValues.at(0) == 3.0);
    CHECK((*it3).objFunValues.at(1) == 3.0);

    REQUIRE(nonDomSorted.at(3).size() == 1);
    auto it4 = nonDomSorted.at(3).begin();
    CHECK((*it4).objFunValues.at(0) == 4.0);
    CHECK((*it4).objFunValues.at(1) == 3.0);



    nonDomSorted = nonDominatedSort(P,lt,1);
    REQUIRE(nonDomSorted.size() == 1);

    REQUIRE(nonDomSorted.at(0).size() == 2);
    it1 = nonDomSorted.at(0).begin();
    if((*it1).objFunValues.at(0) == 1.0) {
        CHECK((*it1).objFunValues.at(0) == 1.0);
        CHECK((*it1).objFunValues.at(1) == 1.0);
        it1++;
        CHECK((*it1).objFunValues.at(0) == 0.0);
        CHECK((*it1).objFunValues.at(1) == 2.0);
    } else {
        CHECK((*it1).objFunValues.at(0) == 0.0);
        CHECK((*it1).objFunValues.at(1) == 2.0);
        it1++;
        CHECK((*it1).objFunValues.at(0) == 1.0);
        CHECK((*it1).objFunValues.at(1) == 1.0);
    }


    nonDomSorted = nonDominatedSort(P,lt,4);
    REQUIRE(nonDomSorted.size() == 3);

    REQUIRE(nonDomSorted.at(0).size() == 2);
    it1 = nonDomSorted.at(0).begin();
    if((*it1).objFunValues.at(0) == 1.0) {
        CHECK((*it1).objFunValues.at(0) == 1.0);
        CHECK((*it1).objFunValues.at(1) == 1.0);
        it1++;
        CHECK((*it1).objFunValues.at(0) == 0.0);
        CHECK((*it1).objFunValues.at(1) == 2.0);
    } else {
        CHECK((*it1).objFunValues.at(0) == 0.0);
        CHECK((*it1).objFunValues.at(1) == 2.0);
        it1++;
        CHECK((*it1).objFunValues.at(0) == 1.0);
        CHECK((*it1).objFunValues.at(1) == 1.0);
    }

    REQUIRE(nonDomSorted.at(1).size() == 1);
    it2 = nonDomSorted.at(1).begin();
    CHECK((*it2).objFunValues.at(0) == 2.0);
    CHECK((*it2).objFunValues.at(1) == 2.0);

    REQUIRE(nonDomSorted.at(2).size() == 1);
    it3 = nonDomSorted.at(2).begin();
    CHECK((*it3).objFunValues.at(0) == 3.0);
    CHECK((*it3).objFunValues.at(1) == 3.0);



    /*nonDomSorted = nonDominatedSort(P,lt,7);

    REQUIRE(nonDomSorted.size() == 4);

    REQUIRE(nonDomSorted.at(0).size() == 2);
    it1 = nonDomSorted.at(0).begin();
    if((*it1).objFunValues.at(0) == 1.0) {
        CHECK((*it1).objFunValues.at(0) == 1.0);
        CHECK((*it1).objFunValues.at(1) == 1.0);
        it1++;
        CHECK((*it1).objFunValues.at(0) == 0.0);
        CHECK((*it1).objFunValues.at(1) == 2.0);
    } else {
        CHECK((*it1).objFunValues.at(0) == 0.0);
        CHECK((*it1).objFunValues.at(1) == 2.0);
        it1++;
        CHECK((*it1).objFunValues.at(0) == 1.0);
        CHECK((*it1).objFunValues.at(1) == 1.0);
    }

    REQUIRE(nonDomSorted.at(1).size() == 1);
    it2 = nonDomSorted.at(1).begin();
    CHECK((*it2).objFunValues.at(0) == 2.0);
    CHECK((*it2).objFunValues.at(1) == 2.0);

    REQUIRE(nonDomSorted.at(2).size() == 1);
    it3 = nonDomSorted.at(2).begin();
    CHECK((*it3).objFunValues.at(0) == 3.0);
    CHECK((*it3).objFunValues.at(1) == 3.0);

    REQUIRE(nonDomSorted.at(3).size() == 1);
    it4 = nonDomSorted.at(3).begin();
    CHECK((*it4).objFunValues.at(0) == 4.0);
    CHECK((*it4).objFunValues.at(1) == 3.0);
    */

}

TEST_CASE("tests functions contained within nsga-ii.h" "[nsga-ii.h]") {
    //test random population
    set<solution> population;
    randomPopulation(population, 500);
    CHECK(population.size() == 500);
    for(auto i = population.begin(); i != population.end(); i++) {
        bool check1 = (*i).str.rows() == 2;
        bool check2 = (*i).str.cols() == 2;
        bool check = check1 && check2;
        CHECK(check);
    }

    //test calculateObjectives
    vector<function<double (Matrix2d)>> objFuns;
    objFuns.push_back(&obj1);
    objFuns.push_back(&obj2);

    solution sol1;
    Matrix2d test1;
    test1(0,0) = 1.0;
    test1(0,1) = 2.0;
    test1(1,0) = 3.0;
    test1(1,1) = 4.0;
    sol1.str = test1;

    solution sol2;
    Matrix2d test2;
    test2(0,0) = 2.0;
    test2(0,1) = 3.0;
    test2(1,0) = 4.0;
    test2(1,1) = 5.0;
    sol2.str = test2;

    set<solution> pop;
    pop.insert(sol1);
    pop.insert(sol2);

    calculateObjectives(pop, objFuns);

    auto it = pop.begin();

    REQUIRE(pop.size() == 2);

    CHECK((*it).objFunValues.at(0) == 10);
    CHECK((*it).objFunValues.at(1) == 20);

    it++;

    CHECK((*it).objFunValues.at(0) == 14);
    CHECK((*it).objFunValues.at(1) == 28);

    //testing getMinMaxValues function
    solution sol3;
    solution sol4;
    solution sol5;

    sol3.objFunValues.push_back(10);
    sol4.objFunValues.push_back(13);
    sol5.objFunValues.push_back(11);
    sol3.objFunValues.push_back(20);
    sol4.objFunValues.push_back(15);
    sol5.objFunValues.push_back(17);

    set<solution> P;
    P.insert(sol3);
    P.insert(sol4);
    P.insert(sol5);

    vector<pair<double,double>> minMax = getMinMaxValues(P);

    REQUIRE(minMax.size() == 2);
    CHECK(minMax.at(0).first == 10);
    CHECK(minMax.at(0).second == 13);
    CHECK(minMax.at(1).first == 15);
    CHECK(minMax.at(1).second == 20);
}