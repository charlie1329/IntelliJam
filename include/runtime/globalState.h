//
// Created by charl on 21/12/2017.
//

#ifndef FYP_GLOBALSTATE_H
#define FYP_GLOBALSTATE_H

#include "port_processing.h"
#include "../esn/esn.h"
#include <memory>
#include <atomic>
#include <boost/thread.hpp>
/**
 * a structure to hold the global state of the system
 */
struct globalState {

    PaUtilRingBuffer ringUpdate{}; //ring buffer for esn update
    void *ringDataUpdate;

    PaUtilRingBuffer ringTimer{}; //ring buffer for timer checks
    void *ringDataTimer;

    shared_ptr<passToCallback> callbackData; //to go to port audio callback function

    shared_ptr<ESN> echo; //the echo state network

    PaStream *stream{}; //the port audio stream

    shared_ptr<atomic<bool>> running; //the current running state of the system

    shared_ptr<boost::mutex> esnMutex; //for locking access to the echo state network
    shared_ptr<boost::mutex> streamMutex;
    shared_ptr<boost::condition_variable_any> cond;

    //constructor for structure just copies everything in
    globalState(PaUtilRingBuffer r, void *rd, PaUtilRingBuffer rt, void *rdt,
                shared_ptr<passToCallback> cd, shared_ptr<ESN> e, PaStream *s,
                shared_ptr<atomic<bool>> run, shared_ptr<boost::mutex> eMtx,
                shared_ptr<boost::mutex> sMtx, shared_ptr<boost::condition_variable_any> cv):
            ringUpdate(r), ringDataUpdate(rd), ringTimer(rt), ringDataTimer(rdt),
            callbackData(cd), echo(e), stream(s), running(run), esnMutex(eMtx),
            streamMutex(sMtx), cond(cv){}
};

#endif //FYP_GLOBALSTATE_H
