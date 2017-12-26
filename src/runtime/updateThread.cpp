/**
 * this file implemented functions
 * defined in updateThread.h
 * Author: Charlie Street
 */

#include "../../include/runtime/globalState.h"

/**
 * implemented from updateThread.h
 * works on reading data in and passing it
 * through to the echo state network
 * @param state the global state of the system
 */
void updateWorker(const shared_ptr<globalState> &state) {

    shared_ptr<atomic<bool>> stillRunning = state->running;

    //get all the synchronisation stuff out of the global state for ease of access
    shared_ptr<boost::mutex> esnMutex = state->esnMutex;
    shared_ptr<boost::mutex> streamMutex = state->streamMutex;
    shared_ptr<boost::condition_variable_any> cond = state->cond;

    //get the echo state network to reduce de-referencing
    shared_ptr<ESN> echo = state->echo;

    PaError err; //for any port audio error checking

    double newInput; //stop constant reallocation

    //repeat until system is stopped
    while(*stillRunning) {

        streamMutex->lock();
        while((err = Pa_IsStreamStopped(state->stream)) && *stillRunning) { //second check in case abnormal termination
            cond->wait(*streamMutex);

            //ignore any data made available while sleeping
            //not much should be available but its a fairly neat check to make
            ring_buffer_size_t howManyMissed = PaUtil_GetRingBufferReadAvailable(&(state->ringUpdate));
            PaUtil_AdvanceRingBufferReadIndex(&(state->ringUpdate), howManyMissed);
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

        if(PaUtil_GetRingBufferReadAvailable(&(state->ringUpdate))) {

            ring_buffer_size_t read = PaUtil_ReadRingBuffer(&(state->ringUpdate),&newInput,1); //read from echo state network

            if(read == 1) { //check read was actually successful
                esnMutex->lock();
                echo->updateReservoir(newInput); //update echo state network
                esnMutex->unlock();
            }
        }

    }



}
