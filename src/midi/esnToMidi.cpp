/**
 * this file implements functions defined in esnToMidi.h
 * it uses my MIDI library to convert ESN output
 * into something musical
 * Author: Charlie Street
 */

#include "../../include/midi/esnToMidi.h"
#include <ctime>

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
        trk.noteOn(0,currentNote,0x40); //TODO: Check offset is correct
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
 * @return an integer timestamp as a string
 */
string generateTimestamp() {
    time_t now = time(nullptr);
    auto time = (int)now; //cast to integer
    return to_string(time);
}