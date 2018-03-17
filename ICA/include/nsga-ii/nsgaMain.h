/**
 * header file for main nsga file
 * Author: Charlie Street
 */

#ifndef ICA_NSGAMAIN_H
#define ICA_NSGAMAIN_H

#include <string>

using namespace std;

void generateSolutions(string inputPath, string outputPrefix);

bool lt(double x, double y);

#endif //ICA_NSGAMAIN_H
