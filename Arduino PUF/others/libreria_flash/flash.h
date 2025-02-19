#ifndef __FLASH_H__
#define __FLASH_H__

#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/boot.h>

#define SPM_PAGE_ERASE (1 << PGERS) | (1 << SELFPRGEN)
#define SPM_RWWSEC_EN (1 << RWWSRE) | (1 << SELFPRGEN)
#define SPM_FILL_BUFF (1 << SELFPRGEN)

#define PUF_FLASH_START ((uint16_t)&__puf_start)
#define PUF_LEN ((uint16_t)&__PUF_LEN__)
#define PAGE_LENB 128

#define split_16_to_8(in, outH, outL)     \
    outH = (uint8_t)((in & 0xFF00) >> 8); \
    outL = (uint8_t)((in & 0x00FF));

#define merge_8_to_16(hi, low) \
    (uint16_t)((hi << 8) | (low & 0xFF));

/** Linker script variables */
extern int __puf_start;
extern int __PUF_LEN__;

void flash_write_page(uint16_t page, uint8_t *buf) __attribute__((section(".dospm")));

/** Extern ASM Functions Prototypes */
extern uint8_t flash_read_byte(uint16_t address);

#endif // __FLASH_H__
