/**
 * file implements member functions from bridge.h
 * Author: Charlie Street
 */

#include "../../include/bridge/bridge.h"
#include "../../include/runtime/updateThread.h"
#include "../../include/runtime/timerThread.h"

#include <iostream>
#include <set>

/**
 * constructor does basic setup of system
 * as this object is created upon startup
 */
Bridge::Bridge() {

    userTile = nullptr; //needs to be set at a later time
    aiTile = nullptr;
    vmeter = nullptr;
    piano = nullptr;

    currentSystemState = nullptr; //not used until a call to openApp()
    timerThread = nullptr;
    updateThread = nullptr;

    err = noError;
    sampleCounter = 0; //used to reduce how many GUI updates we force

    //create the event for midi stream notifications
    event = make_shared<HANDLE>();
    *event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if(!(*event)) {
        err = midiError; //1 = event problem
        return;
    }

    //open the midi stream on the default MIDI device
    outHandle = make_shared<HMIDISTRM>();
    unsigned long midiOpenError = 0;
    midiOpenError = midiStreamOpen(outHandle.get(), (LPUINT)&err, 1, (DWORD)(*event), 0, CALLBACK_EVENT);
    if(midiOpenError) {
        err = midiError;
        CloseHandle(*event);
        return;
    }
    ResetEvent(*event); //event gets notified after opening, clear this

    //Initialise portAudio and get devices list
    pair<PaError, vector<pair<unsigned int, const PaDeviceInfo*>>> devicesPaired = preInitSearch();

    if(devicesPaired.first != paNoError) {
        midiStreamClose(*outHandle);
        CloseHandle(*event);
        err = portAudioError; //2 = port audio problem
        return;
    }

    //store devices
    devices = devicesPaired.second;
}

/**
 * function stops the application, if it is already running
 */
void Bridge::stopApp() {

    err = noError;

    //if nothing running, we should do nothing here
    if(currentSystemState != nullptr && *(currentSystemState->running)) {

        //try to stop the system and wait until it has
        *(currentSystemState->running) = false;
        timerThread->join();
        updateThread->join();

        timerThread = nullptr; //this should cause the thread objects to be deallocated from memory
        updateThread = nullptr;

        //destroy the system state
        if(destroySystem(currentSystemState) != paNoError) err = portAudioError;
    }

    volumeUpdate(0.0); //reset the volume meter for now
}

/**
 * function deals with any clean up required upon the shutdown of the system
 */
void Bridge::closeApp() {

    err = noError;

    stopApp(); //try to stop everything from running if necessary

    //Terminate Port Audio
    if(Pa_Terminate() != paNoError) err = portAudioError;

    //Close the MIDI Stream
    midiStreamClose(*outHandle);

    //Close the Event Handle
    CloseHandle(*event);

}

/**
 * function takes a particular device and
 * attempts to start the system using it
 * @param deviceName the device the user wishes to use
 */
void Bridge::startApp(string deviceName) {

    err = noError; //reset error

    int deviceNum = -1;

    //try to find the device number
    for (auto device: devices) {
        if (deviceName == device.second->name) {
            deviceNum = device.first;
            break;
        }
    }

    if (deviceNum == -1){
        err = portAudioError;
        return;
    }

    //Initialise the System
    pair<PaError, shared_ptr<globalState>> global = initSystem(44100,
                                                               devices.at(static_cast<unsigned int>(deviceNum)),
                                                               outHandle, event);
    if(global.first != paNoError) {
        err = portAudioError;
        return;
    }

    currentSystemState = global.second;

    //start up the port audio stream
    if(Pa_StartStream(currentSystemState->stream) != paNoError) {
        err = portAudioError;
        destroySystem(currentSystemState); //rollback slightly to delete the system state
        currentSystemState = nullptr;
        return;
    }

    //start up the threads
    updateThread = std::make_shared<boost::thread>(updateWorker, boost::cref(currentSystemState));
    timerThread = std::make_shared<boost::thread>(timerWorker, boost::cref(currentSystemState), this);

}

/**
 * function is used to switch the players around when
 * one has finished playing
 */
void Bridge::switchPlayer() {

    volumeUpdate(0.0); //reset volume meter when switching player

    //switch the tiles active state
    if(userTile != nullptr) userTile->switchActive();
    if(aiTile != nullptr) aiTile->switchActive();
}

/**
 * function is used to carry out an update on the volume meter
 * @param newVolume the new volume value to be used
 */
void Bridge::volumeUpdate(double newVolume) {
    if(sampleCounter != SAMPLES_PER_SECOND) {
        sampleCounter++;
    } else {
        sampleCounter = 0;
        if(vmeter != nullptr) vmeter->setNewValue(newVolume);
    }
}

/**
 * function is used to send periodic events to
 * have the keyboard repainted with the right note selected
 * @param events the midi events being played
 * @param arrSize the size of the events array
 * @param ppqn the timebase (pulses per quarter note)
 * @param tempo the midi tempo
 */
void Bridge::pianoUpdate(unsigned long *events, int arrSize, int ppqn, int tempo) {

    //loop round in 3s and mimic the midi events
    for(int i = 3; i < arrSize; i+=3) {

        //carry out any sleep if necessary
        auto millisecondsToSleep = static_cast<unsigned long>((((double)events[i] / (double)ppqn) * tempo) / 1000);
        if(millisecondsToSleep != 0) {
            boost::this_thread::sleep_for(boost::chrono::milliseconds(millisecondsToSleep));
        }

        //examine the midi event, and find the appropriate note to set on the keyboard
        unsigned long event = events[i+2];
        bool noteOn = (event & 0xFF) == 0x90;
        if(noteOn) {
            auto note = (unsigned int)((event & 0xFF00) >> 8); //note is the second byte of the event
            piano->setNoteOn(note-NOTE_OFFSET);
        } else {
            piano->setNoteOff();
        }

    }
}

/**
 * function returns global error status
 * 0 (noError) means good
 * anything else is bad
 * @return the global error code
 */
IntelliJamErr Bridge::getErr() {
    return err;
}

/**
 * function returns the string list of device names
 * for selection by the user
 * @return the list of device names
 */
vector<string> Bridge::getDevices() {
    vector<string> deviceNames;

    //just loop through device list and get names
    for (pair<unsigned int, const PaDeviceInfo*> device: devices) {
       deviceNames.emplace_back(device.second->name);
    }

    //remove all the duplicates
    set<string> noDups(deviceNames.begin(), deviceNames.end());
    deviceNames.assign(noDups.begin(), noDups.end());

    return deviceNames;
}

/**
 * function is a simple setter
 * @param newUserTile the user tile
 * @param newAiTile the ai tile
 */
void Bridge::setTiles(NameTile *newUserTile, NameTile *newAiTile) {
    userTile = newUserTile;
    aiTile = newAiTile;
}

/**
 * function is a simple setter
 * @param newVMeter the new vmeter to set
 */
void Bridge::setVMeter(VMeter *newVMeter) {
    vmeter = newVMeter;
}

/**
 * function is a simple setter
 * @param newPiano the new piano to set
 */
void Bridge::setPiano(Piano *newPiano) {
    piano = newPiano;
}
