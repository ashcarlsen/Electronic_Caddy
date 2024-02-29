#ifndef FLASH_H
#define FLASH_H
#include "stm32l476xx.h"

/* Generic memory-mapped I/O accessor functions */
#define MMIO8(addr)             (*(volatile uint8_t *)(addr))
#define MMIO16(addr)            (*(volatile uint16_t *)(addr))
#define MMIO32(addr)            (*(volatile uint32_t *)(addr))
#define MMIO64(addr)            (*(volatile uint64_t *)(addr))

void flash_wait_for_last_operation(void);
void flash_clear_pgserr_flag(void);
void flash_clear_size_flag(void);
void flash_clear_pgaerr_flag(void);
void flash_clear_wrperr_flag(void);
void flash_clear_progerr_flag(void);
void flash_clear_eop_flag(void);
void flash_clear_status_flags(void);
void flash_lock_option_bytes(void);
void flash_program_double_word(uint32_t address, uint64_t data);
void flash_erase_page(uint8_t bank, uint8_t page);
void unlock_flash(void);
void lock_flash(void);

void readClubs(uint16_t* clubs);
void writeClubs(uint16_t* clubs);
#endif
