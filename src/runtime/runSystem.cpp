/**
 *  this file contains the main function for the system
 *  Author: Charlie Street
 */

#include "../../include/runtime/init_close.h"
#include "../../include/runtime/timerThread.h"
#include "../../include/runtime/updateThread.h"
#include <cstring>
#include <iostream>

#define MY_DEVICE "Line In (Scarlett 2i2 USB)"

//function prototype
PaError runSystem();

/**
 * main function for the system
 * simply calls the runSystem function
 * @return the return code from runSystem
 */
int main() {
    return runSystem();
}

/**
 * this function starts up, runs, and cleans up the system
 * @return any error codes generated in the process
 */
PaError runSystem() {

    unsigned long err = 0;
    PaError paErr = paNoError;

    //Step 1: create the event for midi stream notifications
    shared_ptr<HANDLE> event = make_shared<HANDLE>();
    *event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if(!(*event)) return 1;

    //Step 2: open the midi stream on the default MIDI device
    shared_ptr<HMIDISTRM> outHandle = make_shared<HMIDISTRM>();
    err = midiStreamOpen(outHandle.get(), (LPUINT)&err, 1, (DWORD)(*event), 0, CALLBACK_EVENT);
    if(err) {
        CloseHandle(*event);
        return (PaError)err;
    }
    ResetEvent(*event); //event gets notified after opening, clear this

    //Step 3: Initialise portAudio
    pair<PaError, vector<pair<unsigned int, const PaDeviceInfo*>>> devicesPaired = preInitSearch();

    if(devicesPaired.first != paNoError) {
        midiStreamClose(*outHandle);
        CloseHandle(*event);
        return devicesPaired.first;
    }

    vector<pair<unsigned int, const PaDeviceInfo*>> devices = devicesPaired.second;

    //Step 4: Find desired device (will change once GUI added)
    unsigned int desiredDevice = devices.size();
    for(unsigned int i = 0; i < devices.size(); i++) {
        if(strstr((devices.at(i).second->name),MY_DEVICE) != nullptr) { //if contained
            desiredDevice = i;
        }
    }
    if(desiredDevice == devices.size()) { //if device couldn't be found
        Pa_Terminate(); //best effort attempt to gracefully close system
        midiStreamClose(*outHandle);
        CloseHandle(*event);
        return 1;
    }

    //Step 5: Initialise System
    pair<PaError,shared_ptr<globalState>> global = initSystem(44100,devices.at(desiredDevice),outHandle,event);
    if(global.first != paNoError) {
        Pa_Terminate(); //if this goes wrong, there's nothing I can do
        midiStreamClose(*outHandle);
        CloseHandle(*event);
        return global.first;
    }

    //Step 6: Start PortAudio Stream
    if((paErr = Pa_StartStream(global.second->stream)) != paNoError){
        destroySystem(global.second); //try to clean up as best I can
        Pa_Terminate();
        midiStreamClose(*outHandle);
        CloseHandle(*event);
        return paErr;
    }

    boost::thread updateThread(updateWorker, boost::cref(global.second));
    boost::thread timerThread(timerWorker, boost::cref(global.second));

    //Step 8: Join the threads
    updateThread.join();
    timerThread.join();

    //Step 9: destroy the system state
    if((paErr = destroySystem(global.second))) {
        Pa_Terminate();
        midiStreamClose(*outHandle);
        CloseHandle(*event);
        return paErr;
    }

    //Step 10: terminate port audio
    paErr = Pa_Terminate();

    //Step 11: Close the Midi Stream
    midiStreamClose(*outHandle);

    //Step 12: Close the Event Handle
    CloseHandle(*event);

    return paErr;

}