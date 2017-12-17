/**
 * this file deals with setting up and closing the global state of the system
 * Author: Charlie Street
 */
#ifndef FYP_INIT_CLOSE_H
#define FYP_INIT_CLOSE_H

#include "port_processing.h"
#include "../esn/esn.h"
#include <memory>

#define RING_ELEMENT sizeof(double)
#define RING_SIZE (1048576 / RING_ELEMENT) //1MB ring buffer
#define SAMPLE_JUMP 10

/**
 * a structure to hold the global state of the system
 */
struct globalState {
    PaUtilRingBuffer ring{};
    void *ringData;
    shared_ptr<passToCallback> callbackData;
    shared_ptr<ESN> echo;
    PaStream *stream{};

    globalState(PaUtilRingBuffer r, void *rd, shared_ptr<passToCallback> cd, shared_ptr<ESN> e, PaStream *s):
            ring(r), ringData(rd), callbackData(cd), echo(e), stream(s){}
};

/**
 * function initialises the global state of the system
 * @param sampleRate the sample rate to be used with the device
 * @param device the device information as well as its portAudio Identifier
 * @return an error code (if needed), as well as the newly formed system state
 */
pair<PaError, shared_ptr<globalState>> initSystem(unsigned int sampleRate,
                                                  pair<unsigned int,const PaDeviceInfo*> device);


/**
 * cleans up the system on the system's closure
 * @param state the current global state of the system
 * @return an error code should anything go wrong in the clean up
 */
PaError destroySystem(shared_ptr<globalState> state);

#endif //FYP_INIT_CLOSE_H
