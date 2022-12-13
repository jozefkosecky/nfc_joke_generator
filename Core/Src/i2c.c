/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
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
/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

/* USER CODE BEGIN 0 */
	uint8_t *aReceiveBuffer_read , end_of_read_flag = 0;
	volatile uint8_t ubReceiveIndex = 0;
/* USER CODE END 0 */

I2C_HandleTypeDef hi2c1;

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();

    /* I2C1 interrupt Init */
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);

    /* I2C1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
uint16_t   NFC_IO_IsDeviceReady (uint8_t Addr, uint32_t Trials)
{
    HAL_StatusTypeDef status;
    uint32_t tickstart = 0;
    uint32_t currenttick = 0;

    /* Get tick */
    tickstart = HAL_GetTick();

    /* Wait until M24SR is ready or timeout occurs */
    do
    {
        status = HAL_I2C_IsDeviceReady(&hi2c1, Addr, Trials, NFC_I2C_TIMEOUT_STD);
        currenttick = HAL_GetTick();
    } while( ( (currenttick - tickstart) < NFC_I2C_TIMEOUT_MAX) && (status != HAL_OK) );

    if (status != HAL_OK)
    {
        return NFC_I2C_ERROR_TIMEOUT;
    }

    return NFC_I2C_STATUS_SUCCESS;
}

uint16_t NFC_IO_WriteMultiple (uint8_t Addr, uint8_t *pBuffer, uint16_t Length)
{
  uint16_t status ;
  /* Before calling this function M24SR must be ready: check to detect potential issues */
  status = NFC_IO_IsDeviceReady(Addr, NFC_I2C_TRIALS);
  if (status != NFC_I2C_STATUS_SUCCESS)
  {
    return NFC_I2C_ERROR_TIMEOUT;
  }

  if( HAL_I2C_Master_Transmit(&hi2c1, Addr, (uint8_t*)pBuffer, Length,1000) != HAL_OK)
  {
    return NFC_I2C_ERROR_TIMEOUT;
  }

  return NFC_I2C_STATUS_SUCCESS;
}

uint16_t NFC_IO_ReadMultiple (uint8_t Addr, uint8_t *pBuffer, uint16_t Length )
{
  uint16_t status ;
  uint8_t testBuffer[0xFF];
  memset(testBuffer, 0, sizeof testBuffer);
    /* Before calling this function M24SR must be ready: check to detect potential issues */
  status = NFC_IO_IsDeviceReady(Addr, NFC_I2C_TRIALS);
  if (status != NFC_I2C_STATUS_SUCCESS)
  {
    return NFC_I2C_ERROR_TIMEOUT;
  }

  if( HAL_I2C_Master_Receive(&hi2c1, Addr, (uint8_t*)pBuffer, Length,1000) != HAL_OK)
  {
    return NFC_I2C_ERROR_TIMEOUT;
  }
  return NFC_I2C_STATUS_SUCCESS;
}
/* USER CODE END 1 */
