/**
 * this file implements the functions defined within nonDomSort.h
 * Author: Charlie Street
 */

#include "../../include/nsga-ii/nonDomSort.h"
#include <algorithm>
#include <cassert>
#include <iostream>


/** implemented from nonDomSort.h
 *
 * @param i the first solution
 * @param j the second solution
 * @param objFuns the objective functions used for MOO
 * @param comp a comparison function for the objective space (< or >)
 * @return a boolean stating whether i dominates j or not
 */
bool dominates(solution i,solution j, bool(*comp)(double,double)) {
    bool better = false;

    //test for each objective function, break early if doesn't dominate
    for(unsigned int m = 0; m < i.objFunValues.size(); m++) {
        double obji = i.objFunValues.at(m);
        double objj = j.objFunValues.at(m);
        if(comp(obji,objj)) { //if i is strictly better than j in this objective function
            better = true;
        } else if(obji != objj) { //if i is worse than j in this objective function
            return false;
        }
    }

    return better;
}

/** implemented from nonDomSort.h
 *
 * @param population the population in the GA
 * @param objFuns the objective functions used for MOO
 * @param comp a comparison function for the objective space
 * @return the pareto-optimal non-dominated set
 */
set<solution> findParetoFront(set<solution> population, bool(*comp)(double,double)) {
    vector<solution> P(population.begin(), population.end());

    std::sort(P.begin(), P.end(), //sort by descending order of importance
        [=](solution i, solution j) {
            return comp(i.objFunValues.at(0), j.objFunValues.at(0));
        });

    vector<solution> paretoVec = front(P, comp);
    set<solution> pareto(paretoVec.begin(), paretoVec.end()); //put back into a set
    return pareto;

}

/** implemented from nonDomSort.h
 *
 * @param population the population of the GA
 * @param objFuns the objective functions used for MOO
 * @param comp a comparison function for the objective space
 * @param N desired population size
 * @return the sorted of set of non-dominated sets by their rank (high-low)
 */
vector<set<solution>> nonDominatedSort(set<solution> population, bool(*comp)(double,double), int N) {

    vector<int> n(population.size(), 0);
    n.reserve(population.size());
    vector<vector<unsigned int>> S(population.size()); //inner values are population indexes
    S.reserve(population.size());
    vector<set<unsigned int>> P; //indexes are population indexes
    vector<solution> popVec(population.begin(), population.end());//allows me easier indexing
    popVec.reserve(population.size());
    set<unsigned int> P_1; //first front of indexes

    //steps 1 and 2
    //for each solution, get the number of solutions that dominate it (n_i)
    //and the solutions it dominates (S_i)
    //for ease of access, everything uses population indexes until the end
    for(unsigned int i = 0; i < popVec.size(); i++) {
        for(unsigned int j = 0; j < popVec.size(); j++) {
            if(i != j) {
                if(dominates(popVec.at(i), popVec.at(j), comp)) {
                    S.at(i).push_back(j);
                } else if(dominates(popVec.at(j), popVec.at(i), comp)) {
                    n.at(i)++;
                }
            }
        }
        //part of step 3 (add stuff to non-dominated front 1)
        if(n.at(i) == 0) P_1.insert(i); //nothing dominates solution i
    }


    //rest of step 3
    P.push_back(P_1); //add first front to set
    unsigned int k = 0;

    //saving runtime by only generating desired number of solutions
    int totalSolutions = P_1.size();

    //steps 4 and 5: everything still dealt with as indexes
    while(!P.at(k).empty()) {

        if(totalSolutions >= N) break; //break early if full

        set<unsigned int> Q;
        vector<unsigned int> P_k(P.at(k).begin(), P.at(k).end()); //need to iterate through set
        P_k.reserve(P.at(k).size());
        for(unsigned int i = 0; i < P_k.size(); i++) {
            vector<unsigned int> s_i = static_cast<vector<unsigned int> &&>(S.at(P_k.at(i)));
            s_i.reserve(S.at(P_k.at(i)).size());
            for(unsigned int j = 0; j < s_i.size(); j++) {
                n.at(s_i.at(j))--;
                if(n.at(s_i.at(j)) == 0) Q.insert(reinterpret_cast<unsigned int &&>(s_i.at(j)));
            }
        }

        totalSolutions += Q.size();
        P.push_back(Q);
        k++;
    }

    assert(totalSolutions >= N);


    //now to generate return value
    vector<set<solution>> returnSet;
    returnSet.reserve(P.size());
    for(unsigned int i = 0; i < P.size(); i++) {
        set<solution> P_i;

        auto it = P.at(i).begin();
        if(P.at(i).size() == 0) continue; //just in case someone passes in an N that is greater than the population size
        for(unsigned int j = 0; j < P.at(i).size(); j++) { //convert current front to solutions
            P_i.insert(static_cast<solution &&>(popVec.at(*it)));
            it++;
        }
        returnSet.push_back(P_i);
    }
    return returnSet;
}

/**
 * recursive function for finding pareto-optimal front
 * based on algorithm developed by Kung et al.
 * due to required sorting, vectors are easier to work with over sets
 * however moving up a level, they are converted back to sets
 * @tparam T the type of solutions in the GA
 * @param P the population as a vector
 * @param comp the objective space comparison function
 * @return the non-dominated front
 */
vector<solution> front(vector<solution> P, bool(*comp)(double,double)) {

    if(P.size() <= 1) { //base case
        return P;
    }

    //recursive calls
    int mid = P.size() / 2;
    vector<solution> firstHalf(P.begin(), P.begin() + mid);
    vector<solution> secondHalf(P.begin() + mid, P.end());
    vector<solution> t = front(firstHalf, comp);
    vector<solution> b = front(secondHalf, comp);

    vector<solution> M(t.begin(), t.end());

    //create the merged set
    for(unsigned int i = 0; i < b.size(); i++) {
        bool shouldAdd = true;
        for (unsigned int j = 0; j < t.size(); j++) {
            if(dominates(t.at(j), b.at(i), comp)) { //if something from t dominates i, don't add
                shouldAdd = false;
                break;
            }
        }

        if(shouldAdd) M.push_back(static_cast<solution &&>(b.at(i)));
    }

    return M;

}
