/**
 * this file contains the definition of a structure
 * containing all information to accurately represent the global 
 * state of the system
 * Author: Charlie Street
 */

#ifndef FYP_GLOBALSTATE_H
#define FYP_GLOBALSTATE_H

#include "port_processing.h"
#include "../esn/esn.h"
#include <memory>
#include <atomic>
#include <boost/thread.hpp>
#include <Windows.h>
#include <mmsystem.h>

/**
 * a structure to hold the global state of the system
 */
struct globalState {

    shared_ptr<passToCallback> callbackData; //to go to port audio callback function

    shared_ptr<ESN> echo; //the echo state network

    PaStream *stream{}; //the port audio stream

    shared_ptr<atomic<bool>> running; //the current running state of the system

    shared_ptr<boost::mutex> modelMutex; //for locking access to the prediction model
    shared_ptr<boost::mutex> streamMutex;
    shared_ptr<boost::condition_variable_any> cond;

    //for midi output
    shared_ptr<HMIDISTRM> outHandle;
    shared_ptr<HANDLE> event;

    //constructor for structure just copies everything in
    globalState(shared_ptr<passToCallback> cd, shared_ptr<ESN> e, PaStream *s,
                shared_ptr<atomic<bool>> run, shared_ptr<boost::mutex> mMtx,
                shared_ptr<boost::mutex> sMtx, shared_ptr<boost::condition_variable_any> cv,
                shared_ptr<HMIDISTRM> oh, shared_ptr<HANDLE> ev):
            callbackData(cd), echo(e), stream(s), running(run), modelMutex(mMtx),
            streamMutex(sMtx), cond(cv), outHandle(oh), event(ev){}
};

#endif //FYP_GLOBALSTATE_H
