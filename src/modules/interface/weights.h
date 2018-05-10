/*
Header file for weights.
*/
#ifndef WEIGHTS_H_
#define WEIGHTS_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
** Allocate memory to store learned weights
** uploaded from the PC
*/
#define WEIGHT_BUFFER_OUT_LEN 32
#define WEIGHT_BUFFER_IN_LEN  32
#define WEIGHTS_MEMORY_SIZE sizeof(float)*WEIGHT_BUFFER_OUT_LEN*WEIGHT_BUFFER_IN_LEN+2
extern float weights_memory[WEIGHT_BUFFER_OUT_LEN][WEIGHT_BUFFER_IN_LEN];
extern uint8_t weights_received_memory[WEIGHTS_MEMORY_SIZE];

// public function
void crtpWeightsInit(void);

#endif
