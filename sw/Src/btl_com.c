/*
 * btl_com.c
 *
 *  Created on: Nov 4, 2019
 *      Author: EnginDesktop
 */

#include "btl_com.h"

#include "btl_flash_operations.h"

#include "usbd_cdc_if.h"

#include <string.h>
#include <stdlib.h>

const char resOk[ ]             = "OK\r\n";
const char resNOk[ ]            = "NOK\r\n";
const char resAdrEr[ ]          = "ADDRESS ERROR\r\n";
const char resCrcEr[ ]          = "CRC ERROR\r\n";
const char resCmdError[ ]       = "COMMAND ERROR\r\n";
const char resPacketError[ ]    = "PACKET ERROR\r\n";
const char resWriteError[ ]     = "WRITE ERROR\r\n";
const char resRst[ ]            = "OK\r\n";
const char resWhoAmI[ ]         = "ADAQ Bootloader\r\n";
const char resVer[ ]            = "V100 2019 11 05\r\n";
const char resInfo[ ]           = "Kies Research and Development Center-USB Bootloader for ADAQ data acquisition system.\r\n";
const char resRepo[ ]           = "github.com/enginsubasi/ADAQ-Bootloader";

const char resError[ ]          = "ERROR\r\n";

void comEvaluate ( uint8_t* rxString, uint32_t* indexOfRx, uint8_t* txString, uint32_t* indexOfTx, uint8_t* txTrigger )
{
    if ( *indexOfRx != 0 )
    {
        if ( rxString[ *indexOfRx - 1 ] == '\n' && rxString[ *indexOfRx - 2 ] == '\r' )
        {
            rxString[ *indexOfRx ] = 0;

            /*
             * General commands
             */
            if ( strCmpCast ( rxString, "AT\r\n" ) == 0 )
            {
                strCpyCast ( txString, resOk );
            }
            else if ( strCmpCast ( rxString, "AT+RST\r\n" ) == 0 )
            {
                strCpyCast ( txString, resRst );
            }
            else if ( strCmpCast ( rxString, "AT+WHOAMI\r\n" ) == 0 )
            {
                strCpyCast ( txString, resWhoAmI );
            }
            else if ( strCmpCast ( rxString, "AT+VER\r\n" ) == 0 )
            {
                strCpyCast ( txString, resVer );
            }
            else if ( strCmpCast ( rxString, "AT+INFO\r\n" ) == 0 )
            {
                strCpyCast ( txString, resInfo );
            }
            else if ( strCmpCast ( rxString, "AT+REPO\r\n" ) == 0 )
            {
                strCpyCast ( txString, resRepo );
            }
            /*
             * Bootloader commands
             */
            else if ( strCmpCast ( rxString, "AT+JTA\r\n") == 0 )
            {
                jumpToApplication ( ADR_APP_BEGIN );
            }
            else if ( strCmpCast ( rxString, "AT+ERASEAPP\r\n") == 0 )
            {
                if ( eraseFlashPart ( ADR_APP_BEGIN, APP_PAGE_LENGHT ) == 1 )
                {
                    strCpyCast ( txString, resOk );
                }
                else
                {
                    strCpyCast ( txString, resNOk );
                }
            }

            else if ( rxString[ 0 ] == 'A' && rxString[ 1 ] == 'T' &&
                    rxString[ 2 ] == '+' && rxString[ 3 ] == 'W' &&
                    rxString[ 4 ] == 'R' && rxString[ 5 ] == '=' )
            {
                static int32_t countOfPart = 0;
                static uint32_t wrAdr = 0;
                static uint8_t crcOfPart = 0;
                static uint8_t calculatedCrcOfPart = 0;
                static uint8_t tempHexToIntCharArray[ 64 ] = { 0 };
                static uint8_t tempWriteData[ 64 ] = { 0 };

                static char * pEnd; /* for strtol */

                static uint32_t i = 0;

                rxString += 6;

                if ( rxString[ 1 ] == '3' )
                {
                    /* Count of part */
                    rxString += 2;

                    memcpy ( tempHexToIntCharArray, rxString, 2 );

                    countOfPart = strtol ( ( char* ) tempHexToIntCharArray, &pEnd, 16 );

                    calculatedCrcOfPart = countOfPart;

                    /* Write address */
                    rxString += 2;

                    memcpy ( tempHexToIntCharArray, rxString, 8 );

                    wrAdr = strtol ( ( char* ) tempHexToIntCharArray, &pEnd, 16 );

                    tempHexToIntCharArray[ 2 ] = 0;

                    calculatedCrcOfPart += ( ( wrAdr >> 24 ) & 0xFF );
                    calculatedCrcOfPart += ( ( wrAdr >> 16 ) & 0xFF );
                    calculatedCrcOfPart += ( ( wrAdr >> 8 ) & 0xFF );
                    calculatedCrcOfPart += ( ( wrAdr >> 0 ) & 0xFF );

                    /* Write data array */
                    rxString += 8;

                    for ( i = 0 ; i < ( countOfPart - 5 ) ; ++i )
                    {
                        memcpy ( tempHexToIntCharArray, rxString, 2 );
                        tempWriteData[ i ] = strtol ( ( char* ) tempHexToIntCharArray, &pEnd, 16 );

                        calculatedCrcOfPart += tempWriteData[ i ];

                        rxString += 2;
                    }

                    calculatedCrcOfPart ^= 0xFF;

                    /* Checksum */
                    memcpy ( tempHexToIntCharArray, rxString, 2 );

                    crcOfPart = strtol ( ( char* ) tempHexToIntCharArray, &pEnd, 16 );

                    if ( ( wrAdr > ADR_APP_BEGIN ) && ( wrAdr < ADR_END_OF_FLASH ) )
                    {
                        if ( crcOfPart == calculatedCrcOfPart )
                        {
                            if ( writeFlashPart ( wrAdr, tempWriteData, ( countOfPart - 5 ) ) == 1 )
                            {
                                strCpyCast ( txString, resOk );
                            }
                            else
                            {
                                strCpyCast ( txString, resWriteError );
                            }
                        }
                        else
                        {
                            strCpyCast ( txString, resCrcEr );
                        }
                    }
                    else
                    {
                        strCpyCast ( txString, resAdrEr );
                    }
                }
                else if ( rxString[ 1 ] == '0' )
                {
                    strCpyCast ( txString, resOk );
                }
                else if ( rxString[ 1 ] == '7' )
                {
                    strCpyCast ( txString, resOk );
                }
                else
                {
                    strCpyCast ( txString, resNOk );
                }
            }
            else
            {
                strCpyCast ( txString, resCmdError );
            }
        }

        *indexOfTx = strlen ( ( char* ) txString );

        *txTrigger = 1;

        *indexOfRx = 0;
    }
    else if ( *indexOfRx > 100 )
    {
        strCpyCast ( txString, resPacketError );

        *indexOfTx = strlen ( ( char* ) txString );

        *txTrigger = 1;

        *indexOfRx = 0;
    }


}

void strCpyCast ( uint8_t* prm1, const char* prm2 )
{
    strcpy ( ( char* ) prm1, prm2 );
}

int strCmpCast ( uint8_t* prm1, const char* prm2 )
{
    return ( strcmp ( ( char* ) prm1, prm2 ) );
}


