/**
 * this file contains the implementation
 * of functions defined within timerThread.h
 * Author: Charlie Street
 */

#include "../../include/runtime/globalState.h"
#include "../../include/midi/esnToMidi.h"
#include "../../include/runtime/timerThread.h"
#include "../../include/runtime/timers.h"

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

        silenceTimer(&(state->ringTimer)); //use this function to wait on a condition
        //TODO: Improve Timer!!!

        streamMutex->lock();
        err = Pa_AbortStream(state->stream); //use abort over stop for something more immediate
        streamMutex->unlock();

        if(err != paNoError) { //if something gone wrong
            //start a graceful shutdown
            *stillRunning = false;
            cond->notify_all(); //make sure update thread doesn't block
            break;
        }

        //get output from echo state network
        esnMutex->lock();
        VectorXd output = echo->predict();
        esnMutex->unlock();

        int midiErr = handleMIDI(output, state->outHandle, state->event);
        //TODO: Improve prediction to MIDI function!

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

/**
 * implemented from timerThread.h
 * handles all the midi output for us
 * @param prediction the prediction/output from the ESN
 * @param outHandle the output handle for the midi stream
 * @param event the event handle for the midi stream
 * @return any error codes
 */
int handleMIDI(VectorXd prediction, shared_ptr<HMIDISTRM> outHandle, shared_ptr<HANDLE> event) {

    MIDIHDR hdr{};
    unsigned long err;

    unsigned long *midiEvents = naiveMidiWin(prediction,outHandle.get()); //form our new output

    //fill header struct
    hdr.lpData = reinterpret_cast<LPSTR>(midiEvents);
    hdr.dwBufferLength = hdr.dwBytesRecorded = sizeof(unsigned long) * ((prediction.rows() * 2 + 1) * 3);
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

    //wait/sleep while MIDI is being played
    WaitForSingleObject(*event,INFINITE);

    //clean up (for now)
    midiOutUnprepareHeader(reinterpret_cast<HMIDIOUT>(*outHandle), &hdr, sizeof(MIDIHDR));
    err = midiStreamPause(*outHandle); //pause the stream for now until next output
    delete [] midiEvents; //midiEvents is heap allocated within naiveMidiWin
    if(err) return 1; //something gone wrong in pausing

    return 0; //all is good
}