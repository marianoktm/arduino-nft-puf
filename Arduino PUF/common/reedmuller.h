/*
 * reedmuller.h
 *
 *  Created on: 03 nov 2015
 *      Author: domenicoamelino
 */

#ifndef REEDMULLER_H_
#define REEDMULLER_H_
#include <stdio.h>
#include "utilities.h"
#include <stdint.h>


void encode(uint8_t* input, int16_t* result);
void decode(int16_t* input, uint8_t* msg);
void dotproduct(int16_t* result,int16_t* v,int16_t* u,int16_t n);
void hadamard(int16_t* h);
void hadamard_dotproduct(int16_t *result, int16_t *v, int16_t n);
void hadamard_transform(int16_t * v, uint16_t n);

#endif /* REEDMULLER_H_ */
