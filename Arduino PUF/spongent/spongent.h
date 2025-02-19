#ifndef _SPONGENT_H_
#define _SPONGENT_H_

#define rate		8
#define capacity	128
#define hashsize	128
#define nRounds		20

#define R_SizeInBytes 	(rate / 8)
#define nBits 			(capacity + rate)
#define nSBox 			nBits/8

#include <inttypes.h>

typedef uint8_t     BitSequence;
typedef uint16_t 	DataLength;

typedef uint64_t    bit64;
typedef uint32_t	bit32;
typedef uint16_t	bit16;
typedef uint8_t	    bit8;

#define GET_BIT(x,y) (x >> y) & 0x1

typedef enum { HASH_SUCCESS = 0, HASH_FAIL = 1, BAD_HASHBITLEN = 2 } HashReturn;

typedef struct {
 	BitSequence value[nSBox];					/* current Spongent state */
 	BitSequence messageblock[R_SizeInBytes];	/* message block to be input/output */
	int16_t remainingbitlen;						/* remaining data length */
	int16_t hashbitlen;								/* # of hashed bits so far */
} hashState;

HashReturn SpongentHash(const BitSequence *data, DataLength databitlen, BitSequence *hashval);
HashReturn Init(hashState *state, BitSequence *hashval);
HashReturn Absorb(hashState *state);
HashReturn Squeeze(hashState *state);
HashReturn Pad(hashState *state);

int16_t Pi(int16_t i);
void pLayer(hashState *state);
void Permute(hashState *state);

bit16 lCounter(bit16 lfsr);
bit16 retnuoCl(bit16 lfsr);

#endif /* spongent.h */












