#include <stdio.h>
void gen(uint8_t block[],int size){
    for (int i = 0; i < size; i++) {
        block[i] = rand() % 256;
    }
}