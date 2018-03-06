/**
 * A file to test the correctness and speed of the
 * Windows MidiStream API
 * Author: Charlie Street
 */

#include "include/midi/modelToMidi.h"
#include <chrono>
#include <iostream>
using namespace std;


int main() {

    HANDLE event;
    HMIDISTRM out;
    MIDIHDR hdr{};
    unsigned long err = 0;


    if((event = CreateEvent(nullptr, FALSE, FALSE, nullptr))) {
        if(!(err = midiStreamOpen(&out, (LPUINT)&err, 1, (DWORD)event, 0, CALLBACK_EVENT))) {

            auto start = chrono::high_resolution_clock::now(); //start timer
            ResetEvent(event);

            VectorXd prediction = VectorXd::Zero(8,1);
            prediction(0,0) = 39;
            prediction(1,0) = 43;
            prediction(2,0) = 46;
            prediction(3,0) = 50;
            prediction(4,0) = 46;
            prediction(5,0) = 43;
            prediction(6,0) = 39;
            prediction(7,0) = 39;


            unsigned long *midiEvents = naiveMidiWin(prediction, &out);

            hdr.lpData = reinterpret_cast<LPSTR>(midiEvents);
            hdr.dwBufferLength = hdr.dwBytesRecorded =  sizeof(unsigned long) * 51;
            hdr.dwFlags = 0;

            err = midiOutPrepareHeader(reinterpret_cast<HMIDIOUT>(out), &hdr, sizeof(MIDIHDR));
            if(!err) {
                err = midiStreamOut(out, &hdr, sizeof(MIDIHDR));

                if(!err) {

                    err = midiStreamRestart(out);
                    if(!err) {
                        auto finish = chrono::high_resolution_clock::now(); //start timer
                        chrono::duration<double> elapsed = finish - start;
                        cout << "Elapsed Time To Start Playing: " << elapsed.count() << " (s)" << endl;

                        WaitForSingleObject(event,INFINITE);

                    }
                }

                midiOutUnprepareHeader(reinterpret_cast<HMIDIOUT>(out), &hdr, sizeof(MIDIHDR));

            }

            midiStreamPause(out);
            MIDIHDR hdr2{};
            hdr2.lpData = reinterpret_cast<LPSTR>(midiEvents);
            hdr2.dwBufferLength = hdr2.dwBytesRecorded =  sizeof(unsigned long) * 51;
            hdr2.dwFlags = 0;
            Sleep(5000);
            ResetEvent(event);
            midiOutPrepareHeader(reinterpret_cast<HMIDIOUT>(out), &hdr2, sizeof(MIDIHDR));
            midiStreamOut(out, &hdr2, sizeof(MIDIHDR));

            midiStreamRestart(out);
            WaitForSingleObject(event,INFINITE);
            midiOutUnprepareHeader(reinterpret_cast<HMIDIOUT>(out), &hdr2, sizeof(MIDIHDR));
            midiStreamClose(out);
            delete [] midiEvents;

        }


    }

    CloseHandle(event);

}