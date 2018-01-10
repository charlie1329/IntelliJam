/**
 * this file contains unit tests for various functionality within
 * the runtime part of the system
 * Author: Charlie Street
 */


#define CATCH_CONFIG_MAIN

#include "../../include/test/catch.hpp"
#include "../../include/runtime/init_close.h"
#include <iostream>
#include <cstring>

using namespace std;

/**
 * tests the init_close file functionality
 * Note: this test will only pass if a suitable device is plugged in to the computer
 */
TEST_CASE("Tests the device searching and setup/teardown of the system","[init_close]") {

    pair<PaError, vector<pair<unsigned int, const PaDeviceInfo*>>> preInit = preInitSearch();

    REQUIRE(preInit.first == paNoError);
    cout << "Completed Search" << endl;
    cout << Pa_GetDeviceInfo(Pa_GetDefaultInputDevice())->name << endl;
    int devNum = -1;
    for(unsigned int i = 0; i < preInit.second.size(); i++) {
        cout << preInit.second.at(i).second->name << endl;
        if(strstr(preInit.second.at(i).second->name,"Line In (Scarlett 2i2 USB)") != nullptr) {
            devNum = i;

            cout << "name " << preInit.second.at(i).second->name << endl;
            cout << "maxInputChannels " << preInit.second.at(i).second->maxInputChannels << endl;
            cout << "defaultSampleRate " << preInit.second.at(i).second->defaultSampleRate << endl;
            cout << "defaultHighInputLatency " << preInit.second.at(i).second->defaultHighInputLatency << endl;
            cout << "defaultHighOutputLatency " << preInit.second.at(i).second->defaultHighOutputLatency << endl;
            cout << "defaultLowInputLatency " << preInit.second.at(i).second->defaultLowInputLatency << endl;
            cout << "defaultLowOutputLatency " << preInit.second.at(i).second->defaultLowOutputLatency << endl;
            cout << "hostApi " << preInit.second.at(i).second->hostApi << endl;
            cout << "maxOutputChannels " << preInit.second.at(i).second->maxOutputChannels << endl;

            break;
        }
    }

    REQUIRE(devNum != -1);
    cout << "Found Device: " << devNum << endl;

    shared_ptr<HANDLE> event = make_shared<HANDLE>();
    *event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    REQUIRE(*event);
    cout << "Created Event" << endl;


    shared_ptr<HMIDISTRM> outHandle = make_shared<HMIDISTRM>();
    unsigned long err = 0;
    err = midiStreamOpen(outHandle.get(), (LPUINT)&err, 1, (DWORD)(*event), 0, CALLBACK_EVENT);
    REQUIRE(!err);
    cout << "Set Up Midi Output Stream" << endl;

    ResetEvent(*event);

    pair<PaError, shared_ptr<globalState>> global = initSystem(44100,preInit.second.at(
            static_cast<unsigned int>(devNum)),outHandle,event);

    cout << Pa_GetErrorText(global.first) << endl;
    REQUIRE(global.first == paNoError);
    cout << "Completed Initialisation" << endl;


    //check global state seems fine
    shared_ptr<globalState> state = global.second;
    CHECK(state->callbackData->sampleJump == 10);
    CHECK(state->callbackData->nextSample == 0);
    CHECK(*(state->running));

    //tear down the system
    PaError paErr = destroySystem(state);
    REQUIRE(paErr == paNoError);
    cout << "Destroyed System" << endl;

    paErr = Pa_Terminate();

    REQUIRE(paErr == paNoError);
    cout << "Terminated Port Audio" << endl;

    midiStreamClose(*outHandle);
    CloseHandle(*event);




}
