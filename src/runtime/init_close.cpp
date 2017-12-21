/**
 * implements function definitions/prototypes
 * specified within init_close.h
 * Author: Charlie Street
 */

#include "../../include/runtime/init_close.h"
#include "../../include/port_audio/pa_util.h"

/**
 * implemented from init_close.h
 * @param sampleRate the sample rate of the input/output of the device
 * @param device the device being used for recording/playback
 * @return an error code, and the new global state
 */
pair<PaError, shared_ptr<globalState>> initSystem(unsigned int sampleRate,
                                                  pair<unsigned int,const PaDeviceInfo*> device) {

    PaError err; // initially everything's fine

    //initialise port audio
    err = Pa_Initialize();
    if(err != paNoError) return make_pair(err, nullptr);

    //initialise ESN
    //TODO REPLACE ONCE NETWORK OPTIMISED
    shared_ptr<ESN> echo(make_shared<ESN>(0.1,0.5,0.3,200,10,1,8,nullptr,nullptr));

    //allocate/initialise ring buffer
    PaUtilRingBuffer ringUpdate{};
    PaUtilRingBuffer ringTimer{};

    void *ringDataUpdate = PaUtil_AllocateMemory(RING_ELEMENT * RING_SIZE); //port audio allocation functions
    void *ringDataTimer = PaUtil_AllocateMemory(RING_ELEMENT * RING_SIZE);

    ring_buffer_size_t sizeU = PaUtil_InitializeRingBuffer(&ringUpdate,RING_ELEMENT,RING_SIZE,ringDataUpdate);
    ring_buffer_size_t sizeT = PaUtil_InitializeRingBuffer(&ringTimer,RING_ELEMENT,RING_SIZE,ringDataTimer);

    if(sizeU == -1 || sizeT == -1) { //if something gone wrong
        PaUtil_FreeMemory(ringDataUpdate);
        PaUtil_FreeMemory(ringDataTimer);

        return make_pair(paBufferTooSmall, nullptr);
    }

    //create callback data
    shared_ptr<passToCallback> callbackData(make_shared<passToCallback>(ringUpdate,ringDataUpdate,
                                                                        ringTimer,ringDataTimer,SAMPLE_JUMP));

    //open up stream
    PaStream *stream = nullptr;
    err = openStreamOnDevice(device,sampleRate,stream,callbackData.get());
    if (err != paNoError) {
        PaUtil_FreeMemory(ringDataUpdate);
        PaUtil_FreeMemory(ringDataTimer);

        return make_pair(err, nullptr);
    }
    //the getting of the raw pointer is necessary here

    //set the running state of the system
    shared_ptr<atomic<bool>> running(make_shared<atomic<bool>>(true));

    //create mutexes for use throughout the system
    shared_ptr<boost::mutex> esnMutex(make_shared<boost::mutex>());
    shared_ptr<boost::mutex> streamMutex(make_shared<boost::mutex>());

    //initialise the condition variable
    shared_ptr<boost::condition_variable_any> cond(make_shared<boost::condition_variable_any>());

    //combine into global state
    shared_ptr<globalState> global(make_shared<globalState>(ringUpdate,ringDataUpdate,
                                                            ringTimer,ringDataTimer,callbackData,
                                                            echo,stream,running,esnMutex,
                                                            streamMutex,cond));

    //return global state with no errors found
    return make_pair(paNoError,global);
}

/**
 * implemented from init_close.h
 * @param state the global state of the system
 * @return any error codes returned from any of the port audio functions
 */
PaError destroySystem(shared_ptr<globalState> state) {

    PaError err;

    //close the stream (also acts as if abort had been called, all pending buffers are discarded)
    state->streamMutex->lock(); //better safe than sorry...
    err = Pa_CloseStream(state->stream);
    state->streamMutex->unlock();

    //now deallocate the ring buffer (everything else dealt with by shared_ptr and port audio
    PaUtil_FreeMemory(state->ringDataUpdate);
    PaUtil_FreeMemory(state->ringDataTimer);

    //now terminate port audio
    if(err != paNoError) { //return the earliest error found
        err = Pa_Terminate();
    } else {
        Pa_Terminate();
    }


    return err;
}