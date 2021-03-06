/**
 * this file deals with setting up and closing the global state of the system
 * Author: Charlie Street
 */
#ifndef FYP_INIT_CLOSE_H
#define FYP_INIT_CLOSE_H

#include "globalState.h"
#include <vector>

using namespace std;

#define RING_ELEMENT sizeof(float)
#define RING_SIZE (1048576 / RING_ELEMENT) //1MB ring buffer
#define SAMPLE_JUMP 1

#define N_NOTE_PATH "matrices/NNote.csv"
#define B_NOTE_PATH "matrices/BNote.csv"
#define T_NOTE_PATH "matrices/tNote.csv"
#define K_NOTE 0.5
#define T_NOTE 0.4
#define N_DIR_PATH "matrices/NDir.csv"
#define B_DIR_PATH "matrices/BDir.csv"
#define T_DIR_PATH "matrices/tDir.csv"
#define K_DIR 0.5
#define T_DIR 1.9


/**
 * this function should be called prior to any calls to initSystem
 * this function initialises portAudio, and also returns the list of devices
 * currently available
 * @return a pair of the list of devices and any errors generated during initialisation
 */
pair<PaError, vector<pair<unsigned int, const PaDeviceInfo*>>> preInitSearch();

/**
 * function initialises the global state of the system
 * DO NOT call this function before first calling preInitSearch()
 * @param sampleRate the sample rate to be used with the device
 * @param device the device information as well as its portAudio Identifier
 * @param outHandle the output handle for the midi stream
 * @param event handle for notifications from the midi stream
 * @return an error code (if needed), as well as the newly formed system state
 */
pair<PaError, shared_ptr<globalState>> initSystem(unsigned int sampleRate,
                                                  pair<unsigned int,const PaDeviceInfo*> device,
                                                  shared_ptr<HMIDISTRM> outHandle, shared_ptr<HANDLE> event);


/**
 * cleans up the system on the system's closure
 * @param state the current global state of the system
 * @return an error code should anything go wrong in the clean up
 */
PaError destroySystem(shared_ptr<globalState> state);

#endif //FYP_INIT_CLOSE_H
