/**
 * A file to test the correctness and speed of the
 * Windows MidiStream API
 * Author: Charlie Street
 */

#include "../../include/midi/esnToMidi.h"
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

            ResetEvent(event);

            VectorXd prediction = VectorXd::Zero(8,1);
            prediction(0,0) = 31;
            prediction(1,0) = 31;
            prediction(2,0) = 33;
            prediction(3,0) = 33;
            prediction(4,0) = 34;
            prediction(5,0) = 34;
            prediction(6,0) = 36;
            prediction(7,0) = 36;


            unsigned long *midiEvents = naiveMidiWin(prediction, &out);

            hdr.lpData = reinterpret_cast<LPSTR>(midiEvents);
            hdr.dwBufferLength = hdr.dwBytesRecorded =  sizeof(unsigned long) * 51;
            hdr.dwFlags = 0;

            err = midiOutPrepareHeader(reinterpret_cast<HMIDIOUT>(out), &hdr, sizeof(MIDIHDR));
            if(!err) {
                err = midiStreamOut(out, &hdr, sizeof(MIDIHDR));
                cout << "Prepared Header" << endl;
                if(!err) {
                    cout << "Done midiStreamOut" << endl;
                    err = midiStreamRestart(out);
                    if(!err) {
                        cout << "Restarted" << endl;
                        WaitForSingleObject(event,INFINITE);
                        cout << "Done" << endl;

                    }
                }

                midiOutUnprepareHeader(reinterpret_cast<HMIDIOUT>(out), &hdr, sizeof(MIDIHDR));

            }
            delete [] midiEvents;
            midiStreamClose(out);
        }


    }
    CloseHandle(event);

}