/*
 * btl_flash_operations.c
 *
 *  Created on: Oct 31, 2019
 *      Author: EnginDesktop
 */

#include "btl_flash_operations.h"

#include "main.h"

/* Flash operation */
pFunction JumpToApplication;
uint32_t JumpAddress;

/*
 * @about: Write flash area.
 * @retval: Returns 1 on success 0 on fail.
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

/*
 * @about: Erase flash area.
 * @retval: Returns 1 on success 0 on fail.
 */
uint8_t eraseFlashPart( uint32_t adrBegin, uint32_t pageNumber )
{
    uint8_t retVal = 0;
    uint32_t pageError = 0;

    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef EraseInitStruct;

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = adrBegin;
    EraseInitStruct.NbPages = pageNumber; // 1 page 2048 bytes

    if ( HAL_FLASHEx_Erase ( &EraseInitStruct, &pageError ) == HAL_OK )
    {
        retVal = 1;
    }

    HAL_FLASH_Lock();

    return ( retVal );
}

/*
 * @about: Jump to application function. Call in main loop!
 */
void jumpToApplication ( uint32_t adr )
{
    /* Jump to user application */
    JumpAddress = *(__IO uint32_t*) ( adr + 4);
    JumpToApplication = (pFunction) JumpAddress;
    /* Initialize user application's Stack Pointer */
    __set_MSP(*(__IO uint32_t*) adr );
    JumpToApplication ( );
}

