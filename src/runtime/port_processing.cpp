/**
 * the implementation of the functions defined in
 * port_processing.h
 * Author: Charlie Street
 */

#include "../../include/runtime/port_processing.h"
#include <iostream>
/**
 * implemented from port_processing.h
 * callback function for port audio stream
 * @param input input buffers
 * @param output output buffers
 * @param frameCount number of frames in buffer
 * @param timeInfo N/A
 * @param statusFlags N/A
 * @param userData an instance of the passToCallback struct
 * @return 0
 */
int audioCallback(const void *input, void *output, unsigned long frameCount,
                  const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags,
                  void *userData) {
    auto *info = (passToCallback*)userData; //cast from void pointer
    auto *in = (float*)input;
    (void) timeInfo; //tip from port audio examples, stops IDE from moaning at me
    (void) statusFlags;
    (void) output;

    //loop through frames in input and copy when appropriate
    for(int i = 0; i < frameCount; i++) {
        PaUtil_WriteRingBuffer(&(info->ringUpdate),in,1); //write to update ring buffer
        //write to timer buffer as well as update buffer
        //update buffer currently being written to at a different sample rate
        PaUtil_WriteRingBuffer(&(info->ringTimer),in,1);

        //TODO: CHECK THIS IS CORRECT, RELATED TO WHETHER STEREO OR MONO INPUT
        in += 2;

    }

    return paContinue; //0
}

/**
 * implemented from port_processing.h
 * returns a list of all available devices
 * @return a list of all available devices with associated device number
 */
vector<pair<unsigned int, const PaDeviceInfo*>> getUsableDevices() {

    vector<pair<unsigned int, const PaDeviceInfo*>> devices;

    int numDevices = Pa_GetDeviceCount(); //how many devices are available?
    for(unsigned int i = 0; i < numDevices; i++) {
        const PaDeviceInfo *currentDevice = Pa_GetDeviceInfo(i);
        devices.emplace_back(i,currentDevice);

    }

    return devices;
}

/**
 * implemented from post_processing.h
 * opens up a port audio stream on a particular device
 * @param device the information required about the device to use it
 * @param sampleRate the sample rate of the incoming/outgoing data
 * @param stream the stream being used
 * @param callbackData info about the ring buffer etc.
 * @return the error code from Pa_OpenStream
 */
PaError openStreamOnDevice(pair<unsigned int, const PaDeviceInfo*> device, int sampleRate,
                           PaStream **stream, passToCallback *callbackData) {

    PaStreamParameters inParams{};

    inParams.channelCount = device.second->maxInputChannels;
    inParams.device = device.first;
    inParams.hostApiSpecificStreamInfo = nullptr;
    inParams.sampleFormat = paFloat32; //this internally gets cast to a double when input to ESN
    inParams.suggestedLatency = device.second->defaultLowInputLatency;


    PaError err = Pa_IsFormatSupported(&inParams,nullptr,sampleRate);
    if(err != paFormatIsSupported) {
        std::cout << Pa_GetErrorText(err) << std::endl;
        return err; //don't open the stream if the sample rate isn't supported
    }

    //only want input stream, no need for output
    //return the return value from this function
    return Pa_OpenStream(stream,&inParams,nullptr,sampleRate,
                         paFramesPerBufferUnspecified,
                         paNoFlag,audioCallback,callbackData);
}
