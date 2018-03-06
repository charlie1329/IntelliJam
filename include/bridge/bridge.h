/**
 * Header file defines a class
 * which aids in cleanly communicating between
 * the front and back ends of the system
 * Author: Charlie Street
 */

#ifndef FYP_BRIDGE_H
#define FYP_BRIDGE_H

#define SAMPLES_PER_SECOND 30

#include "../runtime/init_close.h"
#include "include/midi/modelToMidi.h"
#include "../interface/nametile.h"
#include "../interface/vmeter.h"
#include "../interface/piano.h"

/**
 * enum for any errors that may be generated
 */
enum IntelliJamErr {
    noError,
    midiError,
    portAudioError
};

/**
 * class implements a 'bridge' between the two parts of the system
 */
class Bridge {
private:
    shared_ptr<globalState> currentSystemState;
    shared_ptr<boost::thread> timerThread;
    shared_ptr<boost::thread> updateThread;
    shared_ptr<HMIDISTRM> outHandle;
    shared_ptr<HANDLE> event;
    vector<pair<unsigned int, const PaDeviceInfo*>> devices;
    IntelliJamErr err; //global error codes
    int sampleCounter; //used for updating volume meter

    //GUI components
    NameTile *userTile;
    NameTile *aiTile;
    VMeter *vmeter;
    Piano *piano;

public:

    //functions which deal with system state
    Bridge();
    void closeApp();
    void startApp(string deviceName);
    void stopApp();

    //functions to adjust certain GUI components
    void switchPlayer();
    void volumeUpdate(double newVolume);
    void pianoUpdate(unsigned long *midiEvents, int arrSize, int ppqn, int tempo);

    //setters
    void setTiles(NameTile *newUserTile, NameTile *newAiTile);
    void setVMeter(VMeter *newVMeter);
    void setPiano(Piano *piano);

    //auxillary functions
    IntelliJamErr getErr();
    vector<string> getDevices();
};

#endif //FYP_BRIDGE_H
