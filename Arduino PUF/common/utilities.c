/*
 * utilities.c
 *
 *  Created on: 03 nov 2015
 *      Author: domenicoamelino
 */
#include "utilities.h"

void HexToBin(int16_t hex_number,uint8_t* bit_number)
{
    int16_t max = 128;
    for(int16_t i = 0 ; i <8 ; i++){
        bit_number [i] = (hex_number & max ) ? 1 : 0;
        max >>=1;
    }
}

void IntToBin(int16_t val, int16_t* binary_number, uint16_t size)
{
    uint16_t j = size - 1;
    while(val!=0)
    {
        binary_number[j]= val % 2;
        val = val / 2;
        j--;
    }
}

void GenMatrix_column(int16_t val, int16_t* binary_number, int16_t m)
{
    binary_number[0] = 1;
    IntToBin(val,binary_number+1,m);
}

void HexToBin_int(uint8_t hex_number,int16_t* bit_number)
{
    int16_t max = 128;
    for(int16_t i = 0 ; i < 8 ; i++){
        bit_number [i] = (hex_number & max ) ? 1 : 0;
        max >>=1;
    }
    //printf("Valore del vettore intero index ");
    //for(int16_t i=0;i<m;i++) printf("%d",bit_number[i]);
    //printf("\n");

}

uint8_t columnresult(uint8_t* v, int16_t* u, int16_t n)
{
    uint8_t sum = 0;
    for (int16_t i = 0; i < n; i++) sum += v[i]&u[i];

    sum = sum % 2; // se è pari dà zero.
    // restituisce sempre 1 o 0
    return sum;
}

void max_array(int16_t* partial_vector, int16_t* max, int16_t* index, int16_t h_size)
{
    for (int16_t k = 0; k < h_size; k++)
    {
        // printf("Valore %d indice %d Max attuale %d \n ",partial_vector[k],k,*max);
        int16_t value = ABS(partial_vector[k]);
        int16_t abs_max = ABS(*max);
        if (value > abs_max)
        {
           //printf("%d \n",k);
           *max = partial_vector[k];
           *index = k;
        //  printf("Nuovo massimo-> %d con indice--> %d \n",*max,*index);
        }

    }
    //printf("Sto ritornando il valore max %d \n",*max);
}




uint8_t BinToHex(uint8_t* bit)
{
    int16_t c = bit[0]<<7 | bit[1]<<6 | bit[2]<<5 | bit[3]<<4| bit[4]<<3 | bit[5]<<2 | bit[6]<<1 | bit[7];
    return c;
}

uint8_t cw_to_byte(int16_t* c)
{
    uint8_t b = 0U;
    uint8_t x = 0;
    for(uint8_t i = 0U; i < 8U; i++)
    {
        x = (c[i] == 1) ? 1U : 0U;
        b = b | (x << (7-i));
    }
    return b;
}