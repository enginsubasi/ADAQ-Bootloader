/*
 * btl_com.c
 *
 *  Created on: Nov 4, 2019
 *      Author: EnginDesktop
 */

#include "btl_com.h"

#include "usbd_cdc_if.h"

#include <string.h>

const char resAt[ ]             = "OK\r\n";
const char resRst[ ]            = "OK\r\n";
const char resWhoAmI[ ]         = "ADAQ Bootloader\r\n";
const char resVer[ ]            = "V100 2019 11 05\r\n";
const char resInfo[ ]           = "Kies Research and Development Center-USB Bootloader for ADAQ data acquisition system.\r\n";
const char resRepo[ ]           = "github.com/enginsubasi/ADAQ-Bootloader";

const char resError[ ] = "ERROR\r\n";

void comEvaluate ( uint8_t* rxString, uint32_t* indexOfRx, uint8_t* txString, uint32_t* indexOfTx, uint8_t* txTrigger )
{
    if ( *indexOfRx != 0 )
    {
        if ( rxString[ *indexOfRx - 1 ] == '\n' && rxString[ *indexOfRx - 2 ] == '\r' )
        {
            rxString[ *indexOfRx ] = 0;

            if ( strCmpCast ( rxString, "AT\r\n" ) == 0 )
            {
                strCpyCast ( txString, resAt );
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
            else
            {
                strCpyCast ( txString, resError );
            }

            *indexOfTx = strlen ( ( char* ) txString );

            *txTrigger = 1;

            *indexOfRx = 0;
        }
    }
    else if ( *indexOfRx > 100 )
    {
        strCpyCast ( txString, resError );

        *indexOfTx = strlen ( ( char* ) txString );

        CDC_Transmit_FS ( txString, *indexOfTx );

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


