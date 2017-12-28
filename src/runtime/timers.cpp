/**
 * This file implements prototypes specified in
 * timers.h
 * Author: Charlie Street
 */


#include "../../include/runtime/timers.h"
#include <cmath>

/**
 * implemented from timers.h
 * this is the first attempt at a timer
 * this should be improved once the system is running
 * @param ring the ring buffer to be read from
 */
void silenceTimer(PaUtilRingBuffer *ring) {
    bool playingStarted = false;
    bool keepLooping = true;
    int anomalies = 0;
    int sampleCount = 0;

    while(keepLooping) {

        ring_buffer_size_t elementsToRead = PaUtil_GetRingBufferReadAvailable(ring);
        if(elementsToRead == 0) continue; // if nothing to read then don't bother allocating
        auto *read = new float[elementsToRead];
        ring_buffer_size_t inArr = PaUtil_ReadRingBuffer(ring,read,elementsToRead);

        for (int i = 0; i < inArr; i++) {
            if(playingStarted) { //try to detect when the user has stopped playing

                if(fabs(read[i]) < SILENCE_THRESHOLD) { //if we have a value close to silence
                    sampleCount++;
                    if(anomalies) anomalies = 0;
                } else { //if its slightly louder, it may just be an anomaly...
                    if(anomalies <= ANOMALY_MAX) {
                        anomalies++;
                        sampleCount++;
                    } else { //if we've surpassed the number of allowed anomalies
                        anomalies = 0;
                        sampleCount = 0;
                    }
                }

                if(sampleCount > SAMPLES_TILL_STOP) {
                    keepLooping = false;
                    break; //leave the for loop, deallocate the array, and then leave while loop
                }

            } else { //detect when the user has started playing
                if(fabs(read[i]) > NOISE_THRESHOLD) {
                    if(++sampleCount > START_THRESHOLD) {
                        sampleCount = 0;
                        playingStarted = true;
                    }

                } else { //if back to silence
                    sampleCount = 0;
                }
            }
        }

        delete [] read;
    }

}