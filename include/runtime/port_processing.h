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
#include <utility>

using namespace std;

#define RING_ELEMENT sizeof(double)

/**
 * a structure used within the callback function
 * contains information about the ring buffer
 * as well as info about the effective sample rate
 */
struct passToCallback {
    PaUtilRingBuffer ring;
    const void *ringBufferData;
    int sampleJump; //interval of how often to write samples to ring buffer
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
vector<pair<unsigned int, const PaDeviceInfo*>> getUsableDevices();

/**
 * function deals with opening up a stream on a particular device
 * @param device all of the device information as well as its internal identifier
 * @param sampleRate the desired sample rate for reading from the device
 * @param stream the port audio stream
 * @param callbackData info about the ring buffer etc.
 * @return the return code from Pa_OpenStream
 */
PaError openStreamOnDevice(pair<unsigned int, const PaDeviceInfo*> device,
                           int sampleRate, PaStream *stream, passToCallback *callbackData);

#endif //FYP_PORT_PROCESSING_H
