/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f3xx_hal.h"

#include "stm32f3xx_ll_rcc.h"
#include "stm32f3xx_ll_bus.h"
#include "stm32f3xx_ll_system.h"
#include "stm32f3xx_ll_exti.h"
#include "stm32f3xx_ll_cortex.h"
#include "stm32f3xx_ll_utils.h"
#include "stm32f3xx_ll_pwr.h"
#include "stm32f3xx_ll_dma.h"
#include "stm32f3xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdint.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
uint16_t Write_Joke_To_NFC(char message[]);
void Write_Joke_Message(char *jokeBuffer, uint8_t *NDEFmessage);
void shuffle(char array[][255], int rows);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define NFC_GPO_Pin 				LL_GPIO_PIN_3
#define NFC_GPO_GPIO_Port 			GPIOA

/* USER CODE BEGIN Private defines */
#define M24SR_I2C_ADDR_WRITE       (uint8_t)  0xAC
#define M24SR_I2C_ADDR_READ        (uint8_t)  0xAD/*!< M24SR address */
#define PCB_FIELD_02 			   (uint8_t)  0x02
#define PCB_FIELD_03 			   (uint8_t)  0x03
#define CRC_1					   (uint8_t)  0x35
#define CRC_2					     0xC0
#define BUFFER_SIZE					16
#define READ					    (uint8_t) 0x0001
#define WRITE					    (uint8_t) 0x0002
#define ERROR_BAD_LENGTH			(uint16_t) 0x0069
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
