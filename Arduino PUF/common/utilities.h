/*
 * utilities.h
 *
 *  Created on: 03 nov 2015
 *      Author: domenicoamelino
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

#define ABS(x) ((x) > 0) ? (x) : -(x);


#include <stdio.h>
#include <stdint.h>
#include "reedmuller.h"

void HexToBin(int16_t hex_number, uint8_t* bit_number);
void IntToBin(int16_t val, int16_t* binary_number, uint16_t size);
void GenMatrix_column(int16_t val,int16_t* binary_number,int16_t m);
void HexToBin_int(uint8_t hex_number,int16_t* bit_number);
uint8_t columnresult(uint8_t*, int16_t*, int16_t n);
void max_array(int16_t* partial_vector,int16_t* max,int16_t* index,int16_t h_size);

uint8_t cw_to_byte(int16_t* c);


//SOLO PER I MIEI TEST
uint8_t BinToHex(uint8_t* bit);
#endif /* UTILITIES_H_ */
