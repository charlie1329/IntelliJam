/**
 * this file contains the implementation
 * of functions defined within timerThread.h
 * Author: Charlie Street
 */

#include "../../include/runtime/globalState.h"
#include "include/midi/modelToMidi.h"
#include "../../include/runtime/timerThread.h"
#include "../../include/runtime/timers.h"

#include <iostream>

/**
 * implemented from timerThread.h
 * function deals with coordinating system
 * through the occasion of output from
 * the echo state network being required
 * @param state the global state of the system
 * @param bridge the bridge to the interface
 */
void timerWorker(const shared_ptr<globalState> &state, Bridge *bridge) {


    PaError err; //some level of error checking at the portAudio level is needed

    //unpack large amounts of the global state to reduce de-referencing
    shared_ptr<atomic<bool>> stillRunning = state->running;

    //bring in callback data to give access to ring buffers
    shared_ptr<passToCallback> callback = state->callbackData;

    //get pointer to echo state network
    shared_ptr<FPM> fpm = state->fpm;

    //get all synchronisation constructs
    shared_ptr<boost::mutex> modelMutex = state->modelMutex;
    shared_ptr<boost::mutex> streamMutex = state->streamMutex;
    shared_ptr<boost::condition_variable_any> cond = state->cond;

    while(*stillRunning) {
        silenceTimer(&(callback->ringTimer),bridge,stillRunning); //use this function to wait on a condition

        //in case stopped during timer execution
        if(!(*stillRunning)) {
            cond->notify_all();
            break;
        }

        streamMutex->lock();
        err = Pa_AbortStream(state->stream); //use abort over stop for something more immediate
        streamMutex->unlock();

        if(err != paNoError) { //if something gone wrong
            //start a graceful shutdown
            *stillRunning = false;
            cond->notify_all(); //make sure update thread doesn't block
            break;
        }

        //switch players in interface
        if(bridge != nullptr) bridge->switchPlayer();

        //get output from echo state network
        modelMutex->lock();
        MatrixXd output = fpm->combinedPredict();
        modelMutex->unlock();

        int midiErr = handleMIDI(output, state->outHandle, state->event, bridge);

        if(midiErr != 0) {
            //graceful shutdown
            *stillRunning = false;
            cond->notify_all();
            break;
        }


        //advance along the read index to ignore any data brought through due to
        //callbacks still running when stream stopped (if that can indeed happen)
        //this should be safer than flushing the buffer entirely
        //due to the undetermined behaviour of the callbacks
        ring_buffer_size_t elementsMissed = PaUtil_GetRingBufferReadAvailable(&(callback->ringTimer));
        PaUtil_AdvanceRingBufferReadIndex(&(callback->ringTimer),elementsMissed);

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

        //switch player back
        if(bridge != nullptr) bridge->switchPlayer();
    }
}

/**
 * implemented from timerThread.h
 * handles all the midi output for us
 * @param prediction the prediction/output from the fpm
 * @param outHandle the output handle for the midi stream
 * @param event the event handle for the midi stream
 * @param bridge the bridge to the interface
 * @return any error codes
 */
int handleMIDI(MatrixXd prediction, shared_ptr<HMIDISTRM> outHandle, shared_ptr<HANDLE> event, Bridge *bridge) {

    MIDIHDR hdr{};
    unsigned long err;

    //to be passed to piano update
    int ppqn;
    int tempo;

    unsigned long *midiEvents = naiveMidiWin(prediction,outHandle.get(),&ppqn,&tempo); //form our new output

    //get size of event array
    //0 (silence) doesn't get an event
    int predictionNotZero = 0;
    for(int i  = 0; i < prediction.rows(); i ++) {
        if(prediction(i,0) != 0) predictionNotZero++;
    }
    int arrSize = ((predictionNotZero * 2) + 1) * 3;

    //fill header struct
    hdr.lpData = reinterpret_cast<LPSTR>(midiEvents);
    hdr.dwBufferLength = hdr.dwBytesRecorded = sizeof(unsigned long) * arrSize;
    hdr.dwFlags = 0;

    //queue up header into midi event queue
    err = midiOutPrepareHeader(reinterpret_cast<HMIDIOUT>(*outHandle), &hdr, sizeof(MIDIHDR));
    if(err) {
        delete [] midiEvents; //memory clean up
        return 1;
    }

    err = midiStreamOut(*outHandle, &hdr, sizeof(MIDIHDR));
    if(err) {
        delete [] midiEvents; //clean up
        return 1;
    }

    //reset event before restarting stream
    ResetEvent(*event);

    //restart the midi stream
    err = midiStreamRestart(*outHandle);
    if(err) {
        delete [] midiEvents; //de-allocation
        return 1;
    }

    if(bridge != nullptr) bridge->pianoUpdate(midiEvents,arrSize,ppqn,tempo); //update the piano in the gui
    //wait/sleep while MIDI is being played
    WaitForSingleObject(*event,INFINITE);

    //clean up (for now)
    midiOutUnprepareHeader(reinterpret_cast<HMIDIOUT>(*outHandle), &hdr, sizeof(MIDIHDR));
    err = midiStreamPause(*outHandle); //pause the stream for now until next output
    delete [] midiEvents; //midiEvents is heap allocated within naiveMidiWin
    if(err) return 1; //something gone wrong in pausing

    return 0; //all is good
}