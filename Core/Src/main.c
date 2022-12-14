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
#include "m24sr.h"
#include "gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

typedef enum{
  HIGH_IMPEDANCE= 0,
  SESSION_OPENED,
  WIP,
  I2C_ANSWER_READY,
  INTERRUPT,
  STATE_CONTROL
}M24SR_GPO_MGMT;

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
  int rows = 4;
  int columns = 255;
  char array[4][255] = { "Ahojte", "Uz to", "funguje", "aj so zamiesanim"};
  int i = 0;
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  M24SR_ManageRFGPO(M24SR_I2C_ADDR_WRITE, SESSION_OPENED); //nastavenie GPO na session_open
  char message[255];
  strcpy(message, array[i]);

  Write_Joke_To_NFC(message);

  uint8_t newJoke = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	// Free memory when done
  while (1)
  {
    /* USER CODE END WHILE */
	  if(!LL_GPIO_IsInputPinSet(NFC_GPO_GPIO_Port, NFC_GPO_Pin))
	  {
		  newJoke = 1;
	  }
	  if(LL_GPIO_IsInputPinSet(NFC_GPO_GPIO_Port, NFC_GPO_Pin) && newJoke){
		  LL_mDelay(200);

		  i++;
		  if(i == rows){
			  shuffle(array, rows);
			  i = 0;
		  }
		  char message[255];
		  strcpy(message, array[i]);

		  Write_Joke_To_NFC(message);
		  newJoke = 0;
	  }
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

void shuffle(char array[][255], int rows){
	// Seed the random number generator
	srand(time(NULL));

	// Shuffle the array by swapping each element with a random
	// element in the array
	for (int i = 0; i < rows; i++) {
		int j = rand() % rows;
		char temp[255];
		strcpy(temp, array[i]);
		strcpy(array[i], array[j]);
		strcpy(array[j],temp);
	}
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
void Write_Joke_To_NFC(char message[]){


	uint16_t success1 = M24SR_KillSession (M24SR_I2C_ADDR_WRITE); //Otvorenie I2C komunikácie
	uint16_t success2 =  M24SR_SelectApplication (M24SR_I2C_ADDR_WRITE); //Odoslanie príkazu SelectNDEFTagApplication
	uint16_t success3 =  M24SR_SelectCCfile (M24SR_I2C_ADDR_WRITE); //Vybratie CC súboru
	uint16_t success5 =  M24SR_SelectNDEFfile (M24SR_I2C_ADDR_WRITE, NDEF_FILE_ID); //vybratie NDEF súboru
	uint16_t success6 = M24SR_Verify(M24SR_I2C_ADDR_WRITE, WRITE ,0x10 ,DefaultPassword ); //odomknutie NDEF file na write

	//char message[] = "Zraz na discorde zajtra";

	uint8_t dlzka_payload = strlen(message) + 1;
	dlzka_payload += 3;
	uint8_t dlzka_spravy = strlen(message) + 1 + 7;

	uint8_t default_param[] = {0x00, dlzka_spravy, 0xd1, 0x1, dlzka_payload, 0x54, 0x2, 0x65, 0x6e};

	int32_t celkova_dlzka = sizeof(default_param) + strlen(message) + 1;
	uint8_t ndef_message[celkova_dlzka];


	memcpy(ndef_message, default_param, sizeof(default_param));
	memcpy(ndef_message + sizeof(default_param), message, strlen(message) + 1);

	//Write_Joke_Message(&sprava, &ndef_data);

	uint16_t success8 = M24SR_UpdateBinary (M24SR_I2C_ADDR_WRITE, 0x00 , sizeof(ndef_message), ndef_message); //Zapisanie spravy
	uint16_t success10 = M24SR_Verify(M24SR_I2C_ADDR_WRITE, READ ,0x10 ,DefaultPassword ); //odomknutie NDEF file na reade
	uint16_t success11 =  M24SR_SelectNDEFfile (M24SR_I2C_ADDR_WRITE, NDEF_FILE_ID); //vybratie NDEF súboru
	uint16_t success12 =  M24SR_ReadBinary (M24SR_I2C_ADDR_WRITE, 0x00 ,0x02 , buffer); //prečítanie dĺžky NDEF súboru

	uint8_t dlzka = buffer[1];	// dlzka spravy
	dlzka += 2;					// celkova dlzka buffera

	   //NbByteToRead musi byt celkova dlzka
	uint16_t success13 =  M24SR_ReadBinary (M24SR_I2C_ADDR_WRITE, 0x00 ,dlzka , buffer); //prečítanie NDEF súboru
	uint8_t successEnd = M24SR_Deselect (M24SR_I2C_ADDR_WRITE);

}

void Write_Joke_Message(char *jokeBuffer, uint8_t *NDEFmessage){

	uint8_t dlzka_payload = strlen(jokeBuffer) + 1;
	dlzka_payload += 3;

	uint8_t dlzka_spravy = strlen(jokeBuffer) + 1;
	dlzka_spravy += 7;
	uint8_t default_param[] = {0x00, dlzka_spravy, 0xd1, 0x1, dlzka_payload, 0x54, 0x2, 0x65, 0x6e};
	//int32_t celkova_dlzka = sizeof(default_param) + sizeof(jokeBuffer);
	uint16_t joke_dlzka = strlen(jokeBuffer) + 1;
	uint16_t param_dlzka = sizeof(default_param);

	memcpy(NDEFmessage, default_param, param_dlzka);
	memcpy(NDEFmessage + param_dlzka, *jokeBuffer, joke_dlzka);

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
