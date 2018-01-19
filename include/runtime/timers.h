/**
 * This header file contains function prototypes
 * for different timing functions judging when to stop
 * input and output a response
 * Author: Charlie Street
 */

#ifndef FYP_TIMERS_H
#define FYP_TIMERS_H

#include "../port_audio/pa_ringbuffer.h"
#include "../bridge/bridge.h"

#define SAMPLES_TILL_STOP 9000
#define SILENCE_THRESHOLD 0.0003
#define NOISE_THRESHOLD 0.01
#define ANOMALY_MAX 50 //~1/44 of a second
#define START_THRESHOLD 1000

/**
 * a timer based around the silence of the input channel
 * @param ring the ring buffer to be read from
 * @param bridge the bridge to the GUI
 */
void silenceTimer(PaUtilRingBuffer *ring, Bridge *bridge);


#endif //FYP_TIMERS_H
