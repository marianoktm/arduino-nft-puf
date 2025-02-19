#ifndef DM_HASH_H
#define DM_HASH_H

#include <inttypes.h>


#include "puf_parameters.h"

void dm_hash(uint8_t* puf, uint8_t* next_state);

#endif // DM_HASH_H