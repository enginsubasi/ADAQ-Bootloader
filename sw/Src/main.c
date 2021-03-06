/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "iwdg.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <string.h>

#include "usbd_cdc_if.h"

#include "btl_flash_operations.h"
#include "btl_com.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* Communication operation */
com_t rx;
com_t tx;

uint8_t txBuffer[ 256 ] = { 0 };

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	uint32_t tsFor100ms = 0;
	uint8_t comTxTrigger = 0;
	int8_t updFlagCheckRetVal = 0;

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* Com. init */
  rx.index = 0;
  tx.index = 0;

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  MX_GPIO_Init();

  updFlagCheckRetVal = updFlagCheck ( );

  if ( updFlagCheckRetVal == BTL_UPD )
  {

  }
  else if ( updFlagCheckRetVal == BTL_OK )
  {
      jumpToApplication ( ADR_APP_BEGIN );
  }
  else
  {
      LED1_HIGH;
      LED2_HIGH;
      LED3_HIGH
      while ( 1 );
  }

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      HAL_IWDG_Refresh ( &hiwdg );

	  comEvaluate ( rx.buffer, &rx.index, tx.buffer, &tx.index, &comTxTrigger );

	  if ( comTxTrigger )
	  {
	      if ( CDC_Transmit_FS ( tx.buffer, tx.index ) == USBD_OK )
	      {
	          /* Intentionally blank */
	      }

	      comTxTrigger = 0;
	  }

	  if ( ( HAL_GetTick ( ) - tsFor100ms ) > 99 )
	  {
		  tsFor100ms = HAL_GetTick ( );

		  /* CPU LED BLINKING */
		  LED3_TOGGLE;
	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/*
 * @about: Calculates 32-bits CRC between address range. CRC begin macro defined in CRC_BEGIN.
 */
uint32_t crcCalculator ( uint32_t adrBegin, uint32_t adrEnd )
{
    uint32_t calculatedChecksum = CRC_BEGIN;

    uint32_t *ptr32;

    uint32_t adrCounter = adrBegin;

    for ( ; adrCounter < adrEnd; adrCounter += 4 )
    {
        ptr32 = ( uint32_t* )adrCounter;

        calculatedChecksum ^= *ptr32;
    }

    return ( calculatedChecksum );
}

/*
 * @about:
 */
int8_t btlCrcControl ( void )
{
	int8_t retVal = BTL_ER;

	uint32_t *ptr32;

	/* Calculates bootloader CRC value */
	uint32_t calculatedCrc = crcCalculator ( ADR_BTL_BEGIN, ADR_BTL_END );

	/* Reads bootloader CRC value from flash area */
	/* ptr32 = ( uint32_t* )( ADR_BTL_CRC ); */
	ptr32 = ( uint32_t* ) ADR_BTL_CRC;

	if ( *ptr32 == 0xFFFFFFFF )
	{
	    HAL_FLASH_Unlock ( );

        HAL_FLASH_Program ( FLASH_TYPEPROGRAM_WORD, ADR_BTL_CRC, calculatedCrc );

        HAL_FLASH_Lock ( );

        retVal = BTL_OK;
	}
	else
	{
	    if ( calculatedCrc == *ptr32 )
	    {
	        /* Bootloader side CRC OK */
	        retVal = BTL_OK;
	    }
	    else
	    {
	        /* Bootloader side CRC Error */
	        retVal = BTL_ER;
	    }
	}

	return ( retVal );
}

/*
 * @about:
 */
int8_t appCrcControl ( void )
{
    int8_t retVal = BTL_OK;

    return ( retVal );
}

/*
 * @about:
 */
int8_t updFlagCheck ( void )
{
	int8_t retVal = BTL_ER;

	uint32_t *ptr32;

	if ( HAL_GPIO_ReadPin ( FORCED_BTL_GPIO_Port, FORCED_BTL_Pin ) == GPIO_PIN_SET )
    {
        retVal = BTL_UPD;
    }
    else
    {
        ptr32 = ( uint32_t* ) ADR_UPDATE_FLAG;

        if ( *ptr32 != 0xFFFFFFFF )
        {
            retVal = BTL_UPD;
        }
        else
        {
            ptr32 = ( uint32_t* ) ADR_APP_BEGIN;

            if ( *ptr32 == 0xFFFFFFFF )
            {
                retVal = BTL_UPD;
            }
            else
            {
                retVal = BTL_OK;
            }
        }
    }

	return ( retVal );
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
