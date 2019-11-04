/*
 * btl_com.h
 *
 *  Created on: Nov 4, 2019
 *      Author: EnginDesktop
 */

#ifndef BTL_COM_H_
#define BTL_COM_H_

#include "stm32f0xx_hal.h"

void comEvaluate ( uint8_t* rxString, uint32_t* indexOfRx, uint8_t* txString, uint32_t* indexOfTx, uint8_t* txTrigger );

void strCpyCast ( uint8_t* prm1, const char* prm2 );
int strCmpCast ( uint8_t* prm1, const char* prm2 );

#endif /* BTL_COM_H_ */
