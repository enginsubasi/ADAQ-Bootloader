/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

typedef  void (*pFunction)(void);

typedef struct {
	uint8_t 		buffer[ 256 ];
	uint32_t 		index;
} com_t;

extern com_t rx;
extern com_t tx;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

#define ADR_APP_BEGIN           0x08008000 /* From 32k to 128k. Size 96k */
#define APP_SIZE                0x18000
#define APP_PAGE_LENGHT         ( APP_SIZE / 2 / 2048 )
#define ADR_UPDATE_FLAG         ( ADR_APP - 0x800 )

#define ADR_BTL_BEGIN           0x08000000
#define BTL_SIZE                0x6000
#define ADR_BTL_END             ( ADR_BTL_BEGIN + BTL_SIZE )

#define ADR_BTL_CRC             ( ADR_APP_BEGIN - 0x1000 )
#define ADR_APP_CRC             ( ADR_APP_BEGIN - 0x1800 )

#define CRC_BEGIN               0xFFFFFFFF

enum returnValue
{
	BTL_ER = 0,
	BTL_OK = 1,
	BTL_UPD = 2,
};

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

uint32_t crcCalculator ( uint32_t adrBegin, uint32_t adrEnd );
uint32_t readCRCFromFlash ( uint32_t adrBegin, uint32_t adrEnd );

int8_t btlCrcControl ( void );
int8_t appCrcControl ( void );
int8_t updFlagCheck ( void );

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED1_Pin GPIO_PIN_7
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_8
#define LED2_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_9
#define LED3_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define LED1_HIGH 	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
#define LED1_LOW 	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
#define LED1_TOGGLE	HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
#define LED1(VAL)	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, VAL);

#define LED2_HIGH 	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
#define LED2_LOW 	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
#define LED2_TOGGLE	HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
#define LED2(VAL)	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, VAL);

#define LED3_HIGH 	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
#define LED3_LOW 	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
#define LED3_TOGGLE	HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
#define LED3(VAL)	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, VAL);

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
