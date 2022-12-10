/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.h
  * @brief   This file contains all the function prototypes for
  *          the i2c.c file
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
#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "m24sr.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN Private defines */
#define M24SR_I2C_ADDR_WRITE       (uint8_t)  0xAC
#define M24SR_I2C_ADDR_READ        (uint8_t)  0xAD/*!< M24SR address */
#define NFC_I2C_STATUS_SUCCESS     (uint16_t) 0x0000
#define NFC_I2C_ERROR_TIMEOUT      (uint16_t) 0x0011
#define NFC_I2C_TIMEOUT_STD        (uint32_t) 1000 /* I2C Time out (ms), used to call Transmit/Receive HAL functions */
#define NFC_I2C_TIMEOUT_MAX        (uint32_t) 2000 /* I2C Time out (ms), this is the maximum time needed by M24SR to complete any command */
#define NFC_I2C_TRIALS             (uint32_t) 1 /* In case M24SR will reply ACK failed allow to perform retry before returning error (HAL option not used) */

/* USER CODE END Private defines */

void MX_I2C1_Init(void);
uint16_t NFC_IO_WriteMultiple (uint8_t Addr, uint8_t *pBuffer, uint16_t Length);
uint16_t NFC_IO_IsDeviceReady (uint8_t Addr, uint32_t Trials);
uint16_t NFC_IO_ReadMultiple (uint8_t Addr, uint8_t *pBuffer, uint16_t Length );
/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */

