/**
 * This header file contains function prototypes
 * for different timing functions judging when to stop
 * input and output a response
 * Author: Charlie Street
 */

#ifndef FYP_TIMERS_H
#define FYP_TIMERS_H

#include "../port_audio/pa_ringbuffer.h"

#define SAMPLES_TILL_STOP 22050 //half a second
#define SILENCE_THRESHOLD 0.01
#define NOISE_THRESHOLD 0.1
#define ANOMALY_MAX 1000 //~1/44 of a second
#define START_THRESHOLD 10000

/**
 * a timer based around the silence of the input channel
 * @param ring the ring buffer to be read from
 */
void silenceTimer(PaUtilRingBuffer *ring);


#endif //FYP_TIMERS_H
