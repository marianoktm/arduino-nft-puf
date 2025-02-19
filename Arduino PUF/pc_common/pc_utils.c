#include "pc_utils.h"
#include <stdio.h>

void print_buffer(const uint8_t* msg, size_t len)
{
    for(uint32_t i = 0; i < len; i++)
    {
        printf("%02X", msg[i]);
    }
    printf("\n\n");
}

uint8_t ascii_to_byte(char c)
{
    uint8_t rtVal = 0;

    if(c >= '0' && c <= '9')
    {
        rtVal = c - '0';
    }
    else
    {
        rtVal = c - 'a' + 10;
    }

    return rtVal;
}

void print_codeword(int16_t* cw, size_t len)
{
    for(uint32_t i = 0; i < len; i++)
    {
        printf("%d", cw[i]);
    }
    printf("\n\n");
}

void load_buffer_from_file(const char* filename, uint8_t* buff, size_t len)
{
    FILE* fp1 = fopen(filename,"r");

    for (uint16_t i = 0U; i < len; i++)
    {
        buff[i] = ((ascii_to_byte(fgetc(fp1)) & 0xF) << 4 | (ascii_to_byte(fgetc(fp1)) & 0xF));
        if( feof(fp1) )
        {
            printf("\n\nError while reading file %s!\n\n", filename);
            break ;
        }
    }
    fclose(fp1);
}