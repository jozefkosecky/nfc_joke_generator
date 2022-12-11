/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "i2c.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NDEF_FILE_ID            0x0001
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t buffer[256];
uint8_t ndef_data[256];
uint8_t DefaultPassword[16]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
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

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();

  // 8.1 Selection of an NDEF message
  uint16_t success1 = M24SR_KillSession (M24SR_I2C_ADDR_WRITE); //Otvorenie I2C komunikácie
  uint16_t success2 =  M24SR_SelectApplication (M24SR_I2C_ADDR_WRITE); //Odoslanie príkazu SelectNDEFTagApplication
  uint16_t success3 =  M24SR_SelectCCfile (M24SR_I2C_ADDR_WRITE); //Vybratie CC súboru
  uint16_t success4 =  M24SR_ReadBinary (M24SR_I2C_ADDR_WRITE, 0x00 ,0x02 , buffer); //prečítanie dĺžky CC súboru
  uint16_t success42 =  M24SR_ReadBinary (M24SR_I2C_ADDR_WRITE, 0x00 ,buffer[1] , buffer); //prečítanie CC súboru
  uint16_t success5 =  M24SR_SelectNDEFfile (M24SR_I2C_ADDR_WRITE, NDEF_FILE_ID); //vybratie NDEF súboru
  uint16_t success6 = M24SR_Verify(M24SR_I2C_ADDR_WRITE, WRITE ,0x10 ,DefaultPassword ); //odomknutie NDEF file na write
  uint8_t ndefLengthZero[] = {0x00,0x00};
  uint16_t success7 = M24SR_UpdateBinary (M24SR_I2C_ADDR_WRITE, 0x00 , 2, ndefLengthZero); //Prepísanie dĺžky NDEF na 0
  uint8_t testSprava[] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
  uint16_t success8 = M24SR_UpdateBinary (M24SR_I2C_ADDR_WRITE, 0x02 , sizeof(testSprava), testSprava); //Zapisanie spravy od 3 pozicie, 1,2 vyhradene pre dlzku spravy
  uint8_t dlzka = sizeof(testSprava);
  uint16_t success9 = M24SR_UpdateBinary (M24SR_I2C_ADDR_WRITE, 0x01 , 1, dlzka); //Zapisanie dlzky NDEF spravy



  //uint8_t success12 = M24SR_DisableVerificationRequirement (M24SR_I2C_ADDR_WRITE, WRITE);
  //uint8_t success110 = M24SR_Verify(M24SR_I2C_ADDR_WRITE, READ ,0x10 ,DefaultPassword );
  //uint8_t success13 = M24SR_DisableVerificationRequirement (M24SR_I2C_ADDR_WRITE, READ);

  uint8_t successEnd = M24SR_Deselect (M24SR_I2C_ADDR_WRITE);
  /* USER CODE BEGIN 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
  {
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }
  LL_SetSystemCoreClock(8000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
  LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_HSI);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
