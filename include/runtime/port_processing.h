/**
 * This file contains definitions for functions/structures
 * required for the portAudio functionality required by the project
 * Author: Charlie Street
 */

#ifndef FYP_PORT_PROCESSING_H
#define FYP_PORT_PROCESSING_H

#include "../port_audio/portaudio.h"
#include "../port_audio/pa_ringbuffer.h"
#include <vector>
#include <string>

using namespace std;

/**
 * a structure used within the callback function
 * contains information about the ring buffer
 * as well as info about the effective sample rate
 */
struct passToCallback {
    PaUtilRingBuffer ring;
    const void *ringBufferData;
    int sampleJump; //interval of how often to write samples
};

/**
 * the callback function for portAudio
 * in my case, this function will be called
 * once a certain amount of data has been received
 * from the device
 * @param input the input buffers
 * @param output output buffers to be filled
 * @param frameCount the number of frames in the buffers
 * @param timeInfo not particularly useful to me
 * @param statusFlags no particularly useful to me
 * @param userData my own data to be passed in (my own struct)
 * @return in my case, probably only 0, I don't need anything more complicated
 */
int audioCallback(const void *input, void *output,
                  unsigned long frameCount,
                  const PaStreamCallbackTimeInfo *timeInfo,
                  PaStreamCallbackFlags statusFlags,
                  void *userData);

/**
 * a function to list all usable devices for the system
 * will be used later for selection on a GUI
 * @return the list/vector of all usable devices
 */
vector<string> getUsableDevices();

#endif //FYP_PORT_PROCESSING_H
