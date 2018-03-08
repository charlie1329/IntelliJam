/**
 * this file implemented functions
 * defined in updateThread.h
 * Author: Charlie Street
 */

#include "../../include/runtime/globalState.h"
#include <iostream>

/**
 * implemented from updateThread.h
 * works on reading data in and passing it
 * through to the echo state network
 * @param state the global state of the system
 */
void updateWorker(const shared_ptr<globalState> &state) {
    shared_ptr<atomic<bool>> stillRunning = state->running;

    //bring in to access ring buffers
    shared_ptr<passToCallback> callback = state->callbackData;

    //get all the synchronisation stuff out of the global state for ease of access
    shared_ptr<boost::mutex> modelMutex = state->modelMutex;
    shared_ptr<boost::mutex> streamMutex = state->streamMutex;
    shared_ptr<boost::condition_variable_any> cond = state->cond;

    //get the fpm to reduce de-referencing
    shared_ptr<FPM> fpm = state->fpm;

    PaError err; //for any port audio error checking

    float newInput; //stop constant reallocation

    //repeat until system is stopped
    while(*stillRunning) {

        streamMutex->lock();
        while((err = Pa_IsStreamStopped(state->stream)) && *stillRunning) { //second check in case abnormal termination
            cond->wait(*streamMutex);

            //ignore any data made available while sleeping
            //not much should be available but its a fairly neat check to make
            ring_buffer_size_t howManyMissed = PaUtil_GetRingBufferReadAvailable(&(callback->ringUpdate));
            PaUtil_AdvanceRingBufferReadIndex(&(callback->ringUpdate), howManyMissed);
        }
        streamMutex->unlock();
        //do a little bit of error checking
        //by documentation, negative return values are errors
        //probably the best thing to do here is quit
        //and change the running state of the system
        if(err < 0) {
            //tidily start shut-down procedure
            *stillRunning = false;
            break;
        }

        //TODO: Modify here!
        if(PaUtil_GetRingBufferReadAvailable(&(callback->ringUpdate)) > 0) {
            ring_buffer_size_t read = PaUtil_ReadRingBuffer(&(callback->ringUpdate),&newInput,1); //read from echo state network
            if(read == 1) { //check read was actually successful
                modelMutex->lock();
                echo->updateReservoir((double)newInput); //update echo state network (now cast to double)
                modelMutex->unlock();
            }
        }

    }



}
