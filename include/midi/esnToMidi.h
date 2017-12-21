/**
 * this file contains functions
 * that can be used to convert the output from
 * the echo state network into some musical output
 * due to the iterative improvement of the software
 * some of these functions will be much more naive than others
 * Author: Charlie Street
 */

#ifndef FYP_ESNTOMIDI_H
#define FYP_ESNTOMIDI_H

#include "midi.h"
#include "../Eigen/Dense" //data arrives in form of Eigen vectors
#include <string>

using namespace std;
using namespace Eigen;

#define NOTE_OFFSET 9 //the offset between my note convention and that of MIDI

/**
 * this function takes the ESN's output
 * and creates a MIDI file based on it
 * in this naive function, no rhythmic information
 * is given, and so the notes are played as quarter(ish) notes
 * @param prediction the ESN prediction
 * @return the filename of the new output
 */
string naiveMidi(VectorXd prediction);

/**
 * used to provide me with a timestamp
 * so that I can (realistically) uniquely name midi files
 * @return a time stamp as an integer string
 */
string generateTimestamp();

#endif //FYP_ESNTOMIDI_H
