#include "dm_hash.h"

#include "cipher.h"
//#include "common.h"
#include "constants.h"

#ifndef PLAT_ARDUINO
#include <stdio.h>
#include "pc_utils.h"
#endif // PLAT_ARDUINO

void dm_hash(uint8_t* puf, uint8_t* next_state)
{
    uint8_t state[FE_KEY_SIZE] = {0U};

    // prepara lo stato iniziale
    for(uint8_t i = 0U; i < FE_KEY_SIZE; i++)
    {
        state[i] = i;
        next_state[i] = i;
    }

#ifndef PLAT_ARDUINO
#ifdef FE_DEBUG
    printf("state: ");
    print_buffer(state, FE_KEY_SIZE);
#endif
#endif // PLAT_ARDUINO

    // CIPHER


    // schema di Davies–Meyer
    // eseguo il cifrario in cascata usando come chiave un blocco
    // diverso della puf (128 bit ciascuno)
    for(uint16_t i = 0U; i < CS_LEN_BYTE; i++)
    {

#ifndef PLAT_ARDUINO
#ifdef FE_DEBUG
        printf("key %u: ", i);
        print_buffer(&puf[i * FE_KEY_SIZE], FE_KEY_SIZE);
#endif
#endif // PLAT_ARDUINO

        // BEGIN_ENCRYPTION();
        Decrypt(next_state, &puf[i * FE_KEY_SIZE]);
        // END_ENCRYPTION();

        for(uint8_t j = 0U; j < FE_KEY_SIZE; j++)
        {
            next_state[j] = state[j] ^ next_state[j];
            state[j] = next_state[j];
        }
    }
}