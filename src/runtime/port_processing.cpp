/**
 * the implementation of the functions defined in
 * port_processing.h
 * Author: Charlie Street
 */

#include "../../include/runtime/port_processing.h"

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
    auto *out = (double*)output;
    auto *in = (double*)input;

    (void) timeInfo; //tip from port audio examples, stops IDE from moaning at me
    (void) statusFlags;

    int sampleJump = info->sampleJump; //save me de-referencing every time

    //loop through frames in input and copy when appropriate
    for(int i = 0; i < frameCount; i++) {

        if(info->nextSample == 0) { //makes assumption input is effectively mono
            PaUtil_WriteRingBuffer(&(info->ringUpdate),in,1); //write to ring buffer
            info->nextSample = sampleJump;
        }

        //TODO: CHECK THIS IS CORRECT, RELATED TO WHETHER STEREO OR MONO INPUT
        *out++ = *in++;  //copy input to output
        *out++ = *in++;

        info->nextSample--; //in using an extra variable over modulo, I can keep it consistent across callbacks
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
        //TODO Check this check is reasonable
        if(currentDevice && currentDevice->hostApi == paASIO) {
            devices.emplace_back(i,currentDevice); //makes things a little easier for the optimiser it seems
        }
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
                           PaStream *stream, passToCallback *callbackData) {

    PaStreamParameters inParams{};
    PaStreamParameters outParams{};

    inParams.channelCount = 1;//TODO COME BACK TO THIS
    inParams.device = device.first;
    inParams.hostApiSpecificStreamInfo = nullptr;
    inParams.sampleFormat = paFloat32; //this internally gets cast to a double
    inParams.suggestedLatency = device.second->defaultLowInputLatency;

    outParams.channelCount = device.second->maxOutputChannels; //should always be 2
    outParams.device = device.first;
    outParams.hostApiSpecificStreamInfo = nullptr;
    outParams.sampleFormat = paFloat32;
    outParams.suggestedLatency = device.second->defaultLowOutputLatency;

    //check if desired sample rate is supported by the device
    PaError err = Pa_IsFormatSupported(&inParams,&outParams,sampleRate);
    if(err != paFormatIsSupported) {
       return err; //don't open the stream if the sample rate isn't supported
    }

    //return the return value from this function
    return Pa_OpenStream(&stream,&inParams,&outParams,sampleRate,
                         paFramesPerBufferUnspecified,
                         paNoFlag,audioCallback,callbackData);
}
