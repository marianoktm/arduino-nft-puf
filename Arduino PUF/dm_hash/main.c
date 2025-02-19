#include "dm_hash.h"
#include "pc_utils.h"
#include <stdio.h>

int main()
{
    uint8_t puf_response_1[PUF_SIZE] = {0U};
    uint8_t key[FE_KEY_SIZE] = {0U};

    load_buffer_from_file("./puf_dump/a1/new_arduino_1_0.puf", puf_response_1, PUF_SIZE);
    printf("PUF: ");
    print_buffer(puf_response_1, PUF_SIZE);


    dm_hash(puf_response_1, key);


    printf("Risultato Hash (key): ");
    print_buffer(key, FE_KEY_SIZE);
}