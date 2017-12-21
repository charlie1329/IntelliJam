/**
 * this file contains the implementation
 * of functions defined within timerThread.h
 * Author: Charlie Street
 */

#include "../../include/runtime/globalState.h"

/**
 * implemented from timerThread.h
 * function deals with coordinating system
 * through the occasion of output from
 * the echo state network being required
 * @param state the global state of the system
 */
void timerWorker(const shared_ptr<globalState> &state) {

    PaError err; //some level of error checking at the portAudio level is needed

    //unpack large amounts of the global state to reduce de-referencing
    shared_ptr<atomic<bool>> stillRunning = state->running;

    //get pointer to echo state network
    shared_ptr<ESN> echo = state->echo;

    //get all synchronisation constructs
    shared_ptr<boost::mutex> esnMutex = state->esnMutex;
    shared_ptr<boost::mutex> streamMutex = state->streamMutex;
    shared_ptr<boost::condition_variable_any> cond = state->cond;

    while(*stillRunning) {

        //TODO: Condition while loop here

        streamMutex->lock();
        err = Pa_AbortStream(state->stream); //use abort over stop for something more immediate
        streamMutex->unlock();

        if(err != paNoError) { //if something gone wrong
            //start a graceful shutdown
            *stillRunning = true;
            cond->notify_all(); //make sure update thread doesn't block
            break;
        }

        //get output from echo state network
        esnMutex->lock();
        VectorXd output = echo->predict();
        esnMutex->unlock();

        //TODO: Play/generate some form of MIDI output

        //advance along the read index to ignore any data brought through due to
        //callbacks still running when stream stopped (if that can indeed happen)
        //this should be safer than flushing the buffer entirely
        //due to the undetermined behaviour of the callbacks
        ring_buffer_size_t elementsMissed = PaUtil_GetRingBufferReadAvailable(&(state->ringTimer));
        PaUtil_AdvanceRingBufferReadIndex(&(state->ringTimer),elementsMissed);

        streamMutex->lock();
        err = Pa_StartStream(state->stream); //restart the stream again
        cond->notify_all();
        streamMutex->unlock();

        if(err != paNoError) { //more error checking if the portAudio stream is erroneous
            //start graceful shutdown
            *stillRunning = false;
            cond->notify_all(); //ensure the updateThread isn't left behind
            break;
        }
    }

}