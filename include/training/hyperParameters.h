/**
 * file contains all hyperparameter ranges and
 * step sizes to check through these parameters
 * Author: Charlie Street
 */
#ifndef FYP_HYPERPARAMETERS_H
#define FYP_HYPERPARAMETERS_H

//TODO: MAKE SURE TO SET HYPERPARAMETERS AND OTHER DEFINES BEFORE TRAINING
#define LOG_FILE "esnLog.txt"
#define TRAINING_FILE "esnTrain.csv"

//input reservoir weights
#define IN_RES_LOW 0.1
#define IN_RES_HIGH 2.0
#define IN_RES_STEP 0.1

//reservoir reservoir single directional weight
#define RES_R_LOW 0.1
#define RES_R_HIGH 2.0
#define RES_R_STEP 0.1

//reservoir reservoir bidirectional weight
#define RES_A_LOW 0.1
#define RES_A_HIGH 2.0
#define RES_A_STEP 0.1

//reservoir size
#define RES_SIZE_LOW 50
#define RES_SIZE_HIGH 500
#define RES_SIZE_STEP 50

//size of gap of bidirectional connections in reservoir
#define RES_JUMP_LOW 5
#define RES_JUMP_HIGH 50
#define RES_JUMP_STEP 5

//number of training epochs
#define EPOCH_LOW 100
#define EPOCH_HIGH 1000
#define EPOCH_STEP 50

//size of input & output layers (fixed)
#define IN_NEURONS 1
#define OUT_NEURONS 8

#endif //FYP_HYPERPARAMETERS_H
