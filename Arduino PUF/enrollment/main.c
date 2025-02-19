#include "reedmuller.h"
#include "dm_hash.h"
#include "pc_utils.h"
#include "puf_parameters.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>



// #define FE_DEBUG
#define FE_BUILD_HELPER_DATA

int main()
{
    uint8_t puf_response_1[PUF_SIZE] = {0U};
    uint8_t puf_response_2[PUF_SIZE] = {0U};
    uint8_t recovered_puf[PUF_SIZE] = {0U};
    uint8_t helper_data[PUF_SIZE] = {0U};
    uint8_t key[FE_KEY_SIZE] = {0U};

    uint8_t cs_1[CS_LEN_BYTE] = {0U};
    uint8_t cs_2 = 0U;
    uint8_t cs_2_bin[8] = {0U};
    int16_t codeword_1[CW_LEN_BIT] = {0};
    int16_t codeword_2[128] = {0};
    uint8_t codeword_1_bytes[PUF_SIZE] = {0U};
    uint8_t codeword_2_bytes[16] = {0U};

    uint8_t cs_1_test[CS_LEN_BIT] = {0U};
    uint8_t cs_1_test_bytes[CS_LEN_BYTE] = {0U};

    uint16_t count;

    load_buffer_from_file("enrollment/dump/new_arduino_1_0.puf",puf_response_1, PUF_SIZE);
    load_buffer_from_file("enrollment/dump/new_arduino_1_1.puf",puf_response_2, PUF_SIZE);
#ifndef FE_BUILD_HELPER_DATA
    load_buffer_from_file("enrollment/dump/helper_data.puf",helper_data, PUF_SIZE);
#endif

    printf("\n\nPUF response 1: ");
    print_buffer(puf_response_1, PUF_SIZE);
    printf("\nPUF response 2: ");
    print_buffer(puf_response_2, PUF_SIZE);

    // creazione del secret symbol
    for(uint32_t i = 0U; i < CS_LEN_BYTE; i++)
    {
        cs_1[i] = (uint8_t) i + 0xA0;
    }
    cs_1[CS_LEN_BYTE-1] &= (uint8_t) (0xff << ((8*CS_LEN_BYTE) % CS_LEN_BIT));
    printf("\nSecret Symbol: ");
    print_buffer(cs_1, CS_LEN_BYTE);

    for(uint32_t i = 0U; i < CS_LEN_BYTE; i++)
    {
        // Codifica il secret symbol
        encode(&cs_1[i], &codeword_1[i * 128]);
    }

#ifdef FE_DEBUG
    printf("\n\nCodeword 1: ");
    print_codeword(codeword_1, CW_LEN_BIT);
#endif // FE_DEBUG

    // converti la codeword in byte (352)
    count = 0;
    for (uint16_t i = 0U; i < CW_LEN_BIT; i += 8U)
    {
        codeword_1_bytes[count] = cw_to_byte(&codeword_1[i]);
        count++;
    }

#ifdef FE_DEBUG
    //printf("\nCodeword 1 bytes: ");
    //print_buffer(codeword_1_bytes, 352);
#endif // FE_DEBUG

#ifdef FE_BUILD_HELPER_DATA
    // Calcola Helper Data = PUF XOR Codeword
    for(uint16_t i = 0U; i < PUF_SIZE; i++)
    {
        helper_data[i] = puf_response_1[i] ^ codeword_1_bytes[i];
    }
#endif
    printf("\nHelper Data: ");
    print_buffer(helper_data, PUF_SIZE);

    // Calcola la chiave
    // SpongentHash(puf_response_1, CW_LEN_BIT, key);
    dm_hash(puf_response_1, key);
    printf("\n\nKey 1: ");
    print_buffer(key, sizeof(key));


    // VERIFICHE


    // Decodifica effettuata solo come test
    // Attenzione: la codeword viene alterata dalla decode
    for(uint32_t i = 0U; i < CS_LEN_BYTE; i++)
    {
        decode(&codeword_1[i * 128], &cs_1_test[i * 8]);
    }

    // converti da bin a byte
    count = 0;
    for(unsigned i = 0; i < CS_LEN_BIT; i += 8)
    {
        cs_1_test_bytes[count] = BinToHex(cs_1_test+i);
        count++;
    }

    printf("\nDecoded Symbol 1: ");
    print_buffer(cs_1_test_bytes, CS_LEN_BYTE);

    if(0 == memcmp(cs_1, cs_1_test_bytes, CS_LEN_BYTE))
    {
        printf("\nEncode - Decode (puf_response_1) successful!");
    }
    else
    {
        printf("\nError during Encode - Decode!");
    }



    // Fase di ricostruzione


    // PUF response 2 XOR Helper Data
    for(uint16_t i = 0U; i < CS_LEN_BYTE; i++)
    {
        // prepara il blocco di codeword da 128 bit
        for(uint16_t j = 0U; j < 16; j++)
        {
            HexToBin_int(puf_response_2[i*16 + j] ^ helper_data[i*16 + j], &codeword_2[j * 8]);
        }

        // decodifica il risultato della XOR
        decode(codeword_2, cs_2_bin);

        // converti il decoded symbol da bin a byte
        cs_2 = BinToHex(cs_2_bin);

        // codifica di nuovo il secret symbol
        encode(&cs_2, codeword_2);

        // converti la codeword in bytes
        for(uint16_t j = 0U; j < 16; j++)
        {
            codeword_2_bytes[j] = cw_to_byte(&codeword_2[j * 8]);

            // Recovered PUF Response = Codeword XOR Helper_Data
            recovered_puf[i*16 + j] = codeword_2_bytes[j] ^ helper_data[i*16 + j];
        }
    }

    printf("\n\nrecovered_puf: ");
    print_buffer(recovered_puf, PUF_SIZE);

#ifdef FE_DEBUG
    printf("\n\nRisultato XOR: ");
    print_codeword(codeword_2, CW_LEN_BIT);
#endif // FE_DEBUG


    // printf("\nDecoded Symbol 2: ");
    // print_buffer(cs_2, CS_LEN_BYTE);


#ifdef FE_DEBUG
    printf("\nCodeword 2: ");
    print_codeword(codeword_2, CW_LEN_BIT);
#endif // FE_DEBUG


    // confronta la PUF ottenuta
    if(0 == memcmp(puf_response_1, recovered_puf, PUF_SIZE))
    {
        printf("\nLe puf sono uguali");
    }
    else
    {
        printf("\nLe puf NON sono uguali. Recovered PUF: \n");
        print_buffer(recovered_puf, PUF_SIZE);
    }

    // Ricostruisci la chiave
    // memset(key, 0U, FE_KEY_SIZE);
    // SpongentHash(recovered_puf, CW_LEN_BIT, key);
    dm_hash(recovered_puf, key);
    printf("\nKey 2: ");
    print_buffer(key, sizeof(key));

    return 0;
}