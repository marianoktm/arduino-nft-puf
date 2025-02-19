#include "flash.h"

void flash_write_page(uint16_t page, uint8_t *buf)
{
    uint8_t sreg;

    /** Disable interrupts */
    sreg = SREG;
    cli();

    eeprom_busy_wait();

    /** Do erase page*/
    boot_page_erase(page);
    boot_spm_busy_wait();

    /** word-by-word */
    for (int i = 0; i < SPM_PAGESIZE; i += 2)
    {
        uint16_t w = *buf++;
        w += (*buf++) << 8;

        /** fill the page buffer */
        boot_page_fill(page + i, w);
    }

    /** Do page write */
    boot_page_write(page);
    boot_spm_busy_wait();

    /** re-enable RWW section */
    boot_rww_enable();

    /** Restore interrupts */
    SREG = sreg;
}
