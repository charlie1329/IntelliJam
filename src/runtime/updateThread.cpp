/**
 * this file implemented functions
 * defined in updateThread.h
 * Author: Charlie Street
 */

#include "../../include/runtime/globalState.h"
#include "../../include/runtime/updateThread.h"
#include <iostream>
#include <cmath>

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

    //generate vectors for frequency and corresponding note
    vector<int> noteVec;
    vector<double> freqVec;
    double A2 = 55.0; //the note A2 in Hz

    for(int i = 24; i < 80; i++) {
        noteVec.push_back(i);
        freqVec.push_back(A2 * pow(pow(2.0,1.0/12.0),i-24));
    }

    double newInput[FFT_SIZE]; //for storing data for fft
    int currentNote = -1; //what note is currently being played?
    int currentBins = 0; //how long has this note been playing for?

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

        if(PaUtil_GetRingBufferReadAvailable(&(callback->ringUpdate)) >= FFT_SIZE) {
            ring_buffer_size_t read = PaUtil_ReadRingBuffer(&(callback->ringUpdate),newInput,FFT_SIZE); //read from echo state network
            if(read == FFT_SIZE) { //check read was actually successful
                int newNote = findNewNote(newInput); //what's being played right now?

                //decide which action to take depending on the note
                if(currentNote == -1 && newNote == 0) { //don't accept initial silence as a note
                    continue; //TODO: Check calling this here is appropriate!
                } else if(currentNote == -1) { //first note being played
                    currentNote = newNote;
                    currentBins = 1;
                }  else if(currentNote != newNote) {

                    //how long was the note played for?
                    double duration = ((double)(currentBins * FFT_SIZE))/((double)SAMPLE_RATE);

                    modelMutex->lock();
                    fpm->queueNote(currentNote,duration);
                    modelMutex->unlock();
                    currentNote = newNote;
                    currentBins = 1;

                } else if(currentNote == newNote) {
                    currentBins++;
                }

            }
        }

    }
}


/**
 * function finds the cent offset between two notes
 * @param freq1 frequency in Hz
 * @param freq2 frequency in Hz
 * @return the cent offset between the two
 */
double centOffset(double freq1, double freq2) {
    return 1200.0 * log2(freq2/freq1);
}

/**
 * for a given frequency, find the closest note to it
 * using lists of frequencies and corresponding notes
 * @param freq the frequency to use
 * @param freqList a list of available frequencies
 * @param noteList the list of available notes
 * @return the closest note to the frequency
 */
int findClosestNote(double freq, const vector<double> &freqList, const vector<int> &noteList) {
    if(freq <= freqList.at(0)) { //if note smaller than our range
        return noteList.at(0);
    } else if(freq >= freqList.at(freqList.size()-1)) { //if note larger than our range
        return noteList.at(noteList.size()-1);
    } else {
        for (unsigned int i = 0; i < freqList.size()-1; i++) { //only want up until the penultimate item
            if(freq >= freqList.at(i) && freqList.at(i+1)) {
                //calculate offsets
                double leftOffset = fabs(centOffset(freq,freqList.at(i)));
                double rightOffset = fabs(centOffset(freq,freqList.at(i+1)));
                if(leftOffset <= rightOffset) {
                    return noteList.at(i);
                } else {
                    return noteList.at(i+1);
                }
            }
        }
    }
}