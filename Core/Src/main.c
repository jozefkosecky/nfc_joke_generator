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
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "i2c.h"
#include "m24sr.h"
#include "gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "time.h"
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
#define GETMSB(val)     ( (uint8_t) ((val & 0xFF00 )>>8) )
#define GETLSB(val)     ( (uint8_t) (val & 0x00FF ))
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t buffer[2];

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
  int rows = 5;
  int columns = 255;
  char jokes[5][1024];
  createJokes(jokes);
  int i = 0;
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  M24SR_ManageRFGPO(M24SR_I2C_ADDR_WRITE, SESSION_OPENED); //nastavenie GPO na session_open
  char message[1024];
  strcpy(message, jokes[i]);

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
		  LL_mDelay(500);

		  i++;
		  if(i == rows){
			  shuffle(jokes, rows);
			  i = 0;
		  }
		  char message[1024];
		  strcpy(message, jokes[i]);

		  Write_Joke_To_NFC(message);
		  newJoke = 0;
	  }
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

void shuffle(char array[][1024], int rows){
	// Seed the random number generator
	srand(time(NULL));

	// Shuffle the array by swapping each element with a random
	// element in the array
	for (int i = 0; i < rows; i++) {
		int j = rand() % rows;
		char temp[1024];
		strcpy(temp, array[i]);
		strcpy(array[i], array[j]);
		strcpy(array[j],temp);
	}
}

void createJokes(char array[][1024]) {
	strcpy(array[0], "Prijdu dvaja kamarati pred zaverecnou do restaurscie, ze chcu veceru. Casnik im odpovie, ze uz maju len trocha ryze a dva rezne, ale jeden je taky maly. Oni povedia: - To nevadi, my sa podelime. Ryzu si rozdelia na polovicku a jeden si zoberie ten vscsi rezen a druhz hovori: - Ty si pekny kamarat, zobral si si ten vacsi. - No a keby si si ty bral tak ktory si zoberies? - No ja by som si zobral ten mensi. - A co si hubu otvaras ved ho tam mas.\0");
	strcpy(array[1], "Putin is held hostage by a terrorist. A Russian truckdriver stops at the back of a long queue on the motorway. He sees a policeman walking down the line of stopped cars to briefly talk to the drivers. As the policeman approaches the truck, the truckdriver rolls down his window and asks, What's going on? Policeman: A terrorist is holding Putin hostage in a car. He's demanding 10 million rubles, or he'll douse Putin in petrol and set him on fire. So we're asking drivers for donations.Driver: Oh, ok. How much do people donate on average. Policeman: About a gallon.\0");
	strcpy(array[2], "- Povedzte, James, kto je tu blbec, vy ci ja? - Pan grof... vy nie ste clovek, ktory by zamestnaval blbca\0");
	strcpy(array[3], "Zakaznik v predajni pocitacov: - Hladam napinavu dobrodruznu hru na PC, ktora pre mna bude predstavovat ozajstnu vyzvu. Predavac: - Tak to predsa skuste Windows - budete nadseny.\0");
	strcpy(array[4], "Viete, aky je rozdiel medzi hardwarom a softwarom? Hardware sa da tazsie skopirovat.\0");
	strcpy(array[5], "-Tatko, potreboval by som novy hardisk. - A kuzelne slovicko? - Do riti! Vsetko je dnes zaheslovane!\0");
	strcpy(array[6], "Tehotna zena sa bije s kamaratom. Tak som sa pridal ku kamaratovi, aby to bolo dvaja na dvoch.\0");
	strcpy(array[7], "Vies ako sa volal cinan, ktory vynasiel slipy? Fuciminavajo.\0");
	strcpy(array[8], "Vies aky je rozdiel medzi cernochom a pneumatikou? Ak na pneumatiku nasadis retaz tak nerepuje.\0");
	strcpy(array[9], "Viete preco maju zeny o jeden mozgovy zavit viac ako kone? Aby nepili vodu z vedra, ked umyvaju dlazku.\0");
	strcpy(array[10], "Arab prezleceny za tehotnu zenu nastupuje do lietadla a letuska sa ho pyta: „Co to bude, chlapcek alebo dievcatko?“ Arab: „Dvojicky.“\0");
	strcpy(array[11], "Pride homosexual do masiarstva: - Prosim si kilo salamy. - Nakrajat? - Debil, ty si myslis, že mam v zadku jukebox.\0");
	strcpy(array[12], "Life is too short to remove USB safely.\0");
	strcpy(array[13], "Manzelska hadka: ”Pamatas sa, ako si mi hovoril, ze ma budes nosit na rukach?” ”A pamatas sa, aka si bola stihla?”\0");
	strcpy(array[14], "Vojde turista v San Francisku do obchodu so zvieratami a obzera si opice. V tom vojde iny chlap, ide rovno k predavacovi a pyta si: – Prosim si jednu opicu typu C++. Predavac mu ju poda, chlap zaplati 5000 dolarov a odide. Prekvapeny turista sa pyta: – Prosim vas, co to bolo za opicu, ze bola taka draha? – No, viete, ona dokaze programovat v C++, bez chyb, efektivne, uhladny kod. Velmi dobra investicia. – A tamtie opice? – Tie stoja 10 000 dolarov, ale vedia robit v C++, v Jave i v XML. – Preboha, a tam mate opicu za 50 000! Co dokaze ta?” – No, pravdupovediac, este som ju nevidel nic robit, ale ostatne opice ju oslovuju “project manager”.\0");
	strcpy(array[15], "Co je to FEI?  FEI je nieco medzi zoo a cintorinom.\0");
	strcpy(array[16], "Ked chces o sebe vsetko vediet, povedz manzelke, ze je tlsta.\0");
	strcpy(array[17], "Jednim z nejvetsich projevu neduvery v Boha je hromosvod na kostele.\0");
	strcpy(array[18], "Teoria je, ked vsetko vieme, ale nic nefunguje. Prax je, ked vsetko funguje, ale nikto nevie preco. My sme spojili teoriu s praxou – nic nefunguje a nikto nevie preco.\0");
	strcpy(array[19], "Co sa stane z manzelky po 30 rokoch manzelstva? Tucny kamarat.\0");
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

	uint8_t max_send = 240;
	int16_t dlzka_payload = strlen(message) + 1;
	dlzka_payload += 3;
	uint8_t P1_dlzka_payload = GETMSB(dlzka_payload);
	uint8_t P2_dlzka_payload = GETLSB(dlzka_payload);

	int16_t dlzka_spravy = strlen(message) + 1 + 7 + 3;
	uint8_t P1_dlzka_spravy = GETMSB(dlzka_spravy);
	uint8_t P2_dlzka_spravy = GETLSB(dlzka_spravy);
	uint8_t TNF = 0xd1 & 0xEF; //Short record na 0, čiže zapisujeme dlzku payloadu v 4 bajtoch

	uint8_t default_param[] = {P1_dlzka_spravy, P2_dlzka_spravy, TNF, 0x1, 0x0, 0x0, P1_dlzka_payload, P2_dlzka_payload, 0x54, 0x2, 0x65, 0x6e};//12

	int16_t celkova_dlzka = sizeof(default_param) + strlen(message) + 1;

	if(celkova_dlzka < max_send){
		uint8_t ndef_message[celkova_dlzka];
		memcpy(ndef_message, default_param, sizeof(default_param));
		memcpy(ndef_message + sizeof(default_param), message, strlen(message) + 1);
		Write_Joke_Message(ndef_message, celkova_dlzka, 0x00);

	}else
	{
		uint8_t ndef_message[max_send];
		uint8_t param_len = sizeof(default_param);
		uint16_t mess_offset = 0;
		uint16_t update_offset = 0;
		memcpy(ndef_message, default_param, param_len);
		memcpy(ndef_message + param_len, message, max_send - param_len);
		Write_Joke_Message(ndef_message, max_send, 0x00); //odosleme defaul_parametre s castou spravy
		celkova_dlzka -= (max_send - param_len);
		mess_offset = max_send - param_len;
		update_offset = max_send;

		while(celkova_dlzka > 0){ //odosielame zvysok spravy
			memset(ndef_message,0,max_send);
			memcpy(ndef_message, message+mess_offset, max_send);
			mess_offset += max_send;
			Write_Joke_Message(ndef_message, max_send, update_offset);
			update_offset += max_send;
			celkova_dlzka -= max_send;
		}
	}




}

void Write_Joke_Message(uint8_t *NDEFmessage, uint16_t dlzka, uint16_t offset){

	uint16_t success1 = M24SR_KillSession (M24SR_I2C_ADDR_WRITE); //Otvorenie I2C komunikácie
	uint16_t success2 =  M24SR_SelectApplication (M24SR_I2C_ADDR_WRITE); //Odoslanie príkazu SelectNDEFTagApplication
	uint16_t success3 =  M24SR_SelectCCfile (M24SR_I2C_ADDR_WRITE); //Vybratie CC súboru
	uint16_t success5 =  M24SR_SelectNDEFfile (M24SR_I2C_ADDR_WRITE, NDEF_FILE_ID); //vybratie NDEF súboru
	uint16_t success6 = M24SR_Verify(M24SR_I2C_ADDR_WRITE, WRITE ,0x10 ,DefaultPassword ); //odomknutie NDEF file na write
	uint16_t success8 = M24SR_UpdateBinary (M24SR_I2C_ADDR_WRITE, offset , dlzka, NDEFmessage); //Zapisanie spravy
	uint16_t success10 = M24SR_Verify(M24SR_I2C_ADDR_WRITE, READ ,0x10 ,DefaultPassword ); //odomknutie NDEF file na reade
	uint16_t success11 =  M24SR_SelectNDEFfile (M24SR_I2C_ADDR_WRITE, NDEF_FILE_ID); //vybratie NDEF súboru
	uint16_t success12 =  M24SR_ReadBinary (M24SR_I2C_ADDR_WRITE, 0x00 ,0x02 , buffer); //prečítanie dĺžky NDEF súboru

	//dlzka = buffer[1];	// dlzka spravy
	dlzka = (buffer[0] << 8) + buffer[1];
	dlzka += 2;					// celkova dlzka buffera
		   //NbByteToRead musi byt celkova dlzka
	//uint16_t success13 =  M24SR_ReadBinary (M24SR_I2C_ADDR_WRITE, offset ,dlzka , buffer); //prečítanie NDEF súboru
	uint16_t successEnd = M24SR_Deselect (M24SR_I2C_ADDR_WRITE);


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
