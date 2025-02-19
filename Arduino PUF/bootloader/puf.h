/*
 * puf.c
 *
 *  Created on: 09 ott 2015
 *      Author: mariobarbareschi
 *      Modified: Erasmo La Montagna 01/03/2021
 */

#ifndef __PUF_H__
#define __PUF_H__

#include <inttypes.h>
#include "puf_parameters.h"

void puf_extract(uint8_t *puf_response, uint8_t *key);

#endif // __PUF_H__
