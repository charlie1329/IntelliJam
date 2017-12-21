/**
 * header file for functionality needed by the
 * thread responsible for updating the echo state network
 * Author: Charlie Street
 */

#ifndef FYP_UPDATETHREAD_H
#define FYP_UPDATETHREAD_H

#include "globalState.h"

/**
 * worker function for thread that deals
 * with the updating of the echo state network
 * @param state the global state of the system
 */
void updateWorker(const shared_ptr<globalState> &state);

#endif //FYP_UPDATETHREAD_H
