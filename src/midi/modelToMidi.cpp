/**
 * this file implements functions defined in esnToMidi.h
 * it uses my MIDI library to convert ESN output
 * into something musical
 * Author: Charlie Street
 */

#include "include/midi/modelToMidi.h"
#include <ctime>
#include <iostream>

/**
 * implemented from esnToMidi.h
 * Note: no rhythmic information used here
 * @param prediction the Echo State Network's prediction
 * @return the filename of the midi file to play
 */
string naiveMidi(VectorXd prediction) {

    string fileName = "midi_output/output" + generateTimestamp() + ".wav";

    uint16_t division = 4;
    unsigned int quarterNote = 20;

    MidiHeader hd(0,1,division);
    MidiTrack trk;

    //meta events for file
    trk.setTempo(0,50000); //120bpm (this really is very naive)
    trk.programChange(0,30); //electric guitar sound
    trk.keySignature(0,0,0); //C Major

    //note sequence
    for(int i = 0; i < prediction.rows(); i++) {
        auto currentNote = static_cast<byte>(prediction(i, 0) + NOTE_OFFSET);
        trk.noteOn(0,currentNote,0x40);
        trk.noteOff(quarterNote,currentNote,0x40);
    }

    //end of the track
    trk.endOfTrack(0);

    //write to file
    MidiFile output(fileName,hd,trk);
    output.closeFile();

    return fileName;
}

/**
 * implemented from esnToMidi.h
 * assumes output stream already opened
 * @param prediction the model prediction
 * @param out the output handle for the midi device
 * @param ppqn pulses per quarter note (to be stored here)
 * @param tempo the tempo in microseconds per quarter note
 * @return an array of midi events, where 3 longs represent one event
 */
unsigned long *naiveMidiWin(MatrixXd prediction, HMIDISTRM *out, int *ppqn, int *tempo) {
    //TODO:Adjust for different rhythms and silence (value 0)
    unsigned int quarterNote = 96;

    unsigned long err; //error variable for problems in midi

    //set division value of midi track (PPQN)
    MIDIPROPTIMEDIV prop1{};
    prop1.cbStruct = sizeof(MIDIPROPTIMEDIV);
    prop1.dwTimeDiv = 96;
    err = midiStreamProperty(*out, (LPBYTE)&prop1, MIDIPROP_SET|MIDIPROP_TIMEDIV);
    if(err) {
        return nullptr;
    }

    //set the tempo
    MIDIPROPTEMPO prop2{};
    prop2.cbStruct = sizeof(MIDIPROPTIMEDIV);
    prop2.dwTempo = 500000;
    err = midiStreamProperty(*out, (LPBYTE)&prop2, MIDIPROP_SET|MIDIPROP_TEMPO);
    if(err) {
        return nullptr;
    }

    *ppqn = 96;
    *tempo = 500000;

    //number of messages = 1 program change message + however many notes predicted
    auto *events = new unsigned long[((prediction.rows() * 2) + 1) * 3];

    events[0] = 0;
    events[1] = 0; //stupid Windows and its redundant parameters
    events[2] = ((unsigned long)MEVT_SHORTMSG << 24) | 0x00001EC0; //set to guitar sound
    //bit shifting tip from http://midi.teragonaudio.com/tech/stream.htm

    //note on and note off message for each note played
    for(int i = 0; i < prediction.rows(); i++) {
        int noteOn = ((i * 2) + 1) * 3;
        int noteOff = ((i * 2) + 2) * 3;

        auto currentNote = static_cast<unsigned char>(prediction(i, 0) + NOTE_OFFSET);
        DWORD event = 0x007F0090;
        event |= (currentNote << 8);

        events[noteOn] = 0;
        events[noteOn + 1] = 0;
        events[noteOn + 2] = ((unsigned long)MEVT_SHORTMSG << 24) | event;

        events[noteOff] = quarterNote;
        events[noteOff + 1] = 0;
        events[noteOff + 2] = ((unsigned long)MEVT_SHORTMSG << 24) | (event & 0xFFFFFF80);

    }

    return events;
}

/**
 * implemented from esnToMidi.h
 * @return an integer timestamp as a string
 */
string generateTimestamp() {
    time_t now = time(nullptr);
    auto time = (int)now; //cast to integer
    return to_string(time);
}