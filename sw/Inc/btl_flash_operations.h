/*
 * btl_flash_operations.h
 *
 *  Created on: Oct 31, 2019
 *      Author: EnginDesktop
 */

#ifndef BTL_FLASH_OPERATIONS_H_
#define BTL_FLASH_OPERATIONS_H_

#include "stm32f0xx_hal.h"

uint8_t writeFlashPart (uint32_t flashAdr, uint8_t* buffer_addr, uint32_t byteNumber);
uint8_t eraseFlashPart ( uint32_t adrBegin, uint32_t pageNumber );

#endif /* BTL_FLASH_OPERATIONS_H_ */
