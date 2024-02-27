 #include "flash.h"
 
 /** @brief Wait until Last Operation has Ended
 * This loops indefinitely until an operation (write or erase) has completed
 * by testing the busy flag.
 */
void flash_wait_for_last_operation(void)
{
        while ((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY);
}
 
/** @brief Clear the Programming Sequence Error Flag
 * This flag is set when incorrect programming configuration has been made.
 */
void flash_clear_pgserr_flag(void)
{
        FLASH->SR |= FLASH_SR_PGSERR;
}
 
/** Clear programming size error flag */
void flash_clear_size_flag(void)
{
        FLASH->SR |= FLASH_SR_SIZERR;
}
 
/** @brief Clear the Programming Alignment Error Flag
 */
void flash_clear_pgaerr_flag(void)
{
        FLASH->SR |= FLASH_SR_PGAERR;
}
 
/** @brief Clear the Write Protect Error Flag
 */
void flash_clear_wrperr_flag(void)
{
        FLASH->SR |= FLASH_SR_WRPERR;
}
 
/** @brief Clear the Programming Error Status Flag
 */
void flash_clear_progerr_flag(void)
{
        FLASH->SR |= FLASH_SR_PROGERR;
}

void flash_clear_eop_flag(void)
{
        FLASH->SR |= FLASH_SR_EOP;
}
 
/** @brief Clear All Status Flags
 * Program error, end of operation, write protect error, busy.
 */
void flash_clear_status_flags(void)
{
        flash_clear_pgserr_flag();
        flash_clear_size_flag();
        flash_clear_pgaerr_flag();
        flash_clear_wrperr_flag();
        flash_clear_progerr_flag();
        flash_clear_eop_flag();
}
 
/** @brief Lock the Option Byte Access
 * This disables write access to the option bytes. It is locked by default on
 * reset.
 */
void flash_lock_option_bytes(void)
{
        FLASH->CR |= FLASH_CR_OPTLOCK;
}
 
/** @brief Program a 64 bit word to FLASH
 *
 * This performs all operations necessary to program a 64 bit word to FLASH memory.
 * The program error flag should be checked separately for the event that memory
 * was not properly erased.
 *
 * @param[in] address Starting address in Flash.
 * @param[in] data Double word to write
 */
void flash_program_double_word(uint32_t address, uint64_t data)
{
				unlock_flash();
        /* Ensure that all flash operations are complete. */
        flash_wait_for_last_operation();
 
        /* Enable writes to flash. */
        FLASH->CR |= FLASH_CR_PG;
 
        /* Program the each word separately. */
        MMIO32(address) = (uint32_t)data;
        MMIO32(address+4) = (uint32_t)(data >> 32);
 
        /* Wait for the write to complete. */
        flash_wait_for_last_operation();
				
				/* Wait for the EOP flag to be set then clear it */
				while((FLASH->SR & FLASH_SR_EOP) == FLASH_SR_EOP);
				FLASH->SR &= ~FLASH_SR_EOP;
	
        /* Disable writes to flash. */
        FLASH->CR &= ~FLASH_CR_PG;
				lock_flash();
}

void unlock_flash(void)
{
	// Write sequence in flash register to unlock
	FLASH->KEYR = 0x45670123;
	FLASH->KEYR = 0xCDEF89AB;
}

void lock_flash(void)
{
	FLASH->CR |= FLASH_CR_LOCK;
}

void flash_erase_page(uint8_t bank, uint8_t page)
{
	flash_wait_for_last_operation();
	flash_clear_status_flags();
	FLASH->CR |= FLASH_CR_PER;
	FLASH->CR |= page << 3;
	if(bank == 1)
	{
		FLASH->CR &= ~FLASH_CR_BKER;
	}
	else
	{
		FLASH->CR |= FLASH_CR_BKER;
	}
	FLASH->CR |= FLASH_CR_STRT;
	flash_wait_for_last_operation();
	FLASH->CR &= ~FLASH_CR_PER;
}
	