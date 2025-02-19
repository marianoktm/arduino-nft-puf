#ifndef PUF_PARAMETERS_H
#define PUF_PARAMETERS_H

#define PUF_SIZE    304U
#define FE_KEY_SIZE 16U

#define CS_LEN_BIT  152
#define CS_LEN_BYTE 19
#define CW_LEN_BIT  2432

#ifndef PLAT_ARDUINO
#define NOINIT_OFFSET   0x1F6
#define SRAM_OFFSET     ((NOINIT_OFFSET - 0x100) * 2)
#endif // PLAT_ARDUINO

#endif // PUF_PARAMETERS_H