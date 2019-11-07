/*
 * btl_flash_operations.c
 *
 *  Created on: Oct 31, 2019
 *      Author: EnginDesktop
 */

#include "btl_flash_operations.h"

#include "main.h"

/*
 * Returns 1 on success
 * 0 on fail
 */
uint8_t writeFlashPart ( uint32_t flashAdr, uint8_t* buffer_addr, uint32_t byteNumber )
{
    static uint32_t i, j;
    static uint64_t data;

    uint8_t retVal = 1;

    HAL_FLASH_Unlock ( );

    for ( i = 0, j = flashAdr; i < byteNumber; i += 2, j += 2 )
    {
        data = buffer_addr[ i ];
        data |= ( buffer_addr[ i + 1 ] << 8 );

        if ( HAL_FLASH_Program ( FLASH_TYPEPROGRAM_HALFWORD, j, data ) != HAL_OK )
        {
            retVal = 0;
        }
    }

    HAL_FLASH_Lock ( );

    return ( retVal );
}

uint8_t eraseFlashPart( uint32_t adrBegin, uint32_t pageNumber )
{
    uint8_t retVal = 0;
    uint32_t pageError = 0;

    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef EraseInitStruct;

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = adrBegin;
    EraseInitStruct.NbPages = pageNumber; // 46 : 94208 / 2048 Application with CRC

    if ( HAL_FLASHEx_Erase ( &EraseInitStruct, &pageError ) == HAL_OK )
    {
        retVal = 1;
    }

    HAL_FLASH_Lock();

    return ( retVal );
}

