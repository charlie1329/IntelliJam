/**
 * header file for functionality needed by the
 * thread responsible for updating the echo state network
 * Author: Charlie Street
 */

#ifndef FYP_UPDATETHREAD_H
#define FYP_UPDATETHREAD_H

#define FFT_SIZE 8192
#define SAMPLE_RATE 44100.0

#include "globalState.h"

/**
 * worker function for thread that deals
 * with the updating of the echo state network
 * @param state the global state of the system
 */
void updateWorker(const shared_ptr<globalState> &state);

/**
 * function finds the cent offset between two notes
 * @param freq1 frequency in Hz
 * @param freq2 frequency in Hz
 * @return the cent offset between the two
 */
double centOffset(double freq1, double freq2);


/**
 * for a given frequency, find the closest note to it
 * using lists of frequencies and corresponding notes
 * @param freq the frequency to use
 * @param freqList a list of available frequencies
 * @param noteList the list of available notes
 * @return the closest note to the frequency
 */
int findClosestNote(double freq, const vector<double> &freqList, const vector<int> &noteList);

/**
 * uses fourier transform to find note being played NOW
 * @param newInput the raw audio input
 * @return the note being played
 */
int findNewNote(double *newInput);

#endif //FYP_UPDATETHREAD_H
