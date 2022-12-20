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
uint16_t error;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char * jokes[] = {
		"Prijdu dvaja kamarati pred zaverecnou do restaurscie, ze chcu veceru. Casnik im odpovie, ze uz maju len trocha ryze a dva rezne, ale jeden je taky maly. Oni povedia: - To nevadi, my sa podelime. Ryzu si rozdelia na polovicku a jeden si zoberie ten vscsi rezen a druhz hovori: - Ty si pekny kamarat, zobral si si ten vacsi. - No a keby si si ty bral tak ktory si zoberies? - No ja by som si zobral ten mensi. - A co si hubu otvaras ved ho tam mas.\0",
		"Putin is held hostage by a terrorist. A Russian truckdriver stops at the back of a long queue on the motorway. He sees a policeman walking down the line of stopped cars to briefly talk to the drivers. As the policeman approaches the truck, the truckdriver rolls down his window and asks, What's going on? Policeman: A terrorist is holding Putin hostage in a car. He's demanding 10 million rubles, or he'll douse Putin in petrol and set him on fire. So we're asking drivers for donations.Driver: Oh, ok. How much do people donate on average. Policeman: About a gallon.\0",
			"- Povedzte, James, kto je tu blbec, vy ci ja? - Pan grof... vy nie ste clovek, ktory by zamestnaval blbca\0",
		"Zakaznik v predajni pocitacov: - Hladam napinavu dobrodruznu hru na PC, ktora pre mna bude predstavovat ozajstnu vyzvu. Predavac: - Tak to predsa skuste Windows - budete nadseny.\0",
		"Viete, aky je rozdiel medzi hardwarom a softwarom? Hardware sa da tazsie skopirovat.\0",
		"-Tatko, potreboval by som novy hardisk. - A kuzelne slovicko? - Do riti! Vsetko je dnes zaheslovane!\0",
		"Tehotna zena sa bije s kamaratom. Tak som sa pridal ku kamaratovi, aby to bolo dvaja na dvoch.\0",
		"Vies ako sa volal cinan, ktory vynasiel slipy? Fuciminavajo.\0",
		"Vies aky je rozdiel medzi cernochom a pneumatikou? Ak na pneumatiku nasadis retaz tak nerepuje.\0",
		"Viete preco maju zeny o jeden mozgovy zavit viac ako kone? Aby nepili vodu z vedra, ked umyvaju dlazku.\0",
		"Arab prezleceny za tehotnu zenu nastupuje do lietadla a letuska sa ho pyta: „Co to bude, chlapcek alebo dievcatko?“ Arab: „Dvojicky.“\0",
		"Pride homosexual do masiarstva: - Prosim si kilo salamy. - Nakrajat? - Debil, ty si myslis, ze mam v zadku jukebox.\0",
		"Life is too short to remove USB safely.\0",
		"Manzelska hadka: ”Pamatas sa, ako si mi hovoril, ze ma budes nosit na rukach?” ”A pamatas sa, aka si bola stihla?”\0",
		"Vojde turista v San Francisku do obchodu so zvieratami a obzera si opice. V tom vojde iny chlap, ide rovno k predavacovi a pyta si: – Prosim si jednu opicu typu C++. Predavac mu ju poda, chlap zaplati 5000 dolarov a odide. Prekvapeny turista sa pyta: – Prosim vas, co to bolo za opicu, ze bola taka draha? – No, viete, ona dokaze programovat v C++, bez chyb, efektivne, uhladny kod. Velmi dobra investicia. – A tamtie opice? – Tie stoja 10 000 dolarov, ale vedia robit v C++, v Jave i v XML. – Preboha, a tam mate opicu za 50 000! Co dokaze ta?” – No, pravdupovediac, este som ju nevidel nic robit, ale ostatne opice ju oslovuju “project manager”.\0",
		"Co je to FEI?  FEI je nieco medzi zoo a cintorinom.\0",
		"Ked chces o sebe vsetko vediet, povedz manzelke, ze je tlsta.\0",
		"Jednim z nejvetsich projevu neduvery v Boha je hromosvod na kostele.\0",
		"Teoria je, ked vsetko vieme, ale nic nefunguje. Prax je, ked vsetko funguje, ale nikto nevie preco. My sme spojili teoriu s praxou – nic nefunguje a nikto nevie preco.\0",
		"Co sa stane z manzelky po 30 rokoch manzelstva? Tucny kamarat.\0",
};
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
  /* USER CODE BEGIN 2 */

  uint8_t newJoke = 0;
  int rows = 20;
  char message[maxSize];
  int i = 0;

  M24SR_ManageRFGPO(M24SR_I2C_ADDR_WRITE, SESSION_OPENED); //nastavenie GPO na session_open
  strcpy(message, jokes[i]); // skopirovanie vtipu do bufferu
  Write_Joke_To_NFC(message); // prvotne nahratie vtipu no NDEF suboru NFC
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    /* USER CODE END WHILE */
	  if(!LL_GPIO_IsInputPinSet(NFC_GPO_GPIO_Port, NFC_GPO_Pin)) // polling GPO pin, zistujeme ci prebieha RF komunikácia, 1 (NIE), 0 (ANO)
	  {
		  newJoke = 1; // nastavime na 1, po skonceni komunikacie mozme zacat nahravat novy vtip
	  }
	  if(LL_GPIO_IsInputPinSet(NFC_GPO_GPIO_Port, NFC_GPO_Pin) && newJoke){ // kontrola ci skoncila RF komunikacia


		  i++; // po kazdom vtipe sa posuvame v liste
		  if(i == rows){ // kontrola ci sme dosli na koniec listu
			  shuffle(jokes, rows); // zamiesanie poradia vtipov
			  i = 0;
		  }

		  char message[maxSize];
		  strcpy(message, jokes[i]);
		  LL_mDelay(500);
		  Write_Joke_To_NFC(message);
		  newJoke = 0;

	  }
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
void shuffle(char *array[], int rows){ // premiesanie poradia vtipov
	srand(time(NULL));
	for (int i = 0; i < rows; i++) {
		int j = rand() % rows;

		char *test = array[i];
		 array[i] =  array[j];
		 array[j] = test;
	}
}

void Write_Joke_To_NFC(char message[]){

	uint8_t max_send = 240;

	// zistujeme dlzky NDEF spravy a jeho payloadu
	int16_t dlzka_payload = strlen(message) + 1;
	dlzka_payload += 3;
	uint8_t P1_dlzka_payload = GETMSB(dlzka_payload);
	uint8_t P2_dlzka_payload = GETLSB(dlzka_payload);

	int16_t dlzka_spravy = strlen(message) + 1 + 7 + 3;
	uint8_t P1_dlzka_spravy = GETMSB(dlzka_spravy);
	uint8_t P2_dlzka_spravy = GETLSB(dlzka_spravy);
	//
	uint8_t TNF = 0xd1 & 0xEF; //Short record na 0, čiže zapisujeme dlzku payloadu v 4 bajtoch

	// hlavicka NDEF spravy, obsahuje dlzku spravy, dlzku payloadu, TNF - flagy
	uint8_t default_param[] = {P1_dlzka_spravy, P2_dlzka_spravy, TNF, 0x1, 0x0, 0x0, P1_dlzka_payload, P2_dlzka_payload, 0x54, 0x2, 0x65, 0x6e};//12

	int16_t celkova_dlzka = sizeof(default_param) + strlen(message) + 1;
	uint8_t param_len = sizeof(default_param);

	if(celkova_dlzka < max_send) // kontrolujeme ci dokazeme celu NDEF spravu preniest naraz
	{
		uint8_t ndef_message[celkova_dlzka];

		// konstrujujeme celu spravu hlavicka + payload
		memcpy(ndef_message, default_param, param_len);
		memcpy(ndef_message + param_len, message, strlen(message) + 1);

		// odosielame NDEF spravu a kontrolujeme ci nenastali chyby ak ano zopakujeme odosielanie
		if(Write_Joke_Message(ndef_message, celkova_dlzka, 0x00) !=M24SR_ACTION_COMPLETED){
			LL_mDelay(25);
			Write_Joke_Message(ndef_message, max_send, 0x00);
		}

	}else // ak je sprava vacsie ako vieme odoslat naraz tak treba ju odoslat po castiach
	{
		uint8_t ndef_message[max_send];
		uint16_t mess_offset = 0;
		uint16_t update_offset = 0;

		memcpy(ndef_message, default_param, param_len);
		memcpy(ndef_message + param_len, message, max_send - param_len);

		//odosleme defaul_parametre s castou spravy + kontrola
		if(Write_Joke_Message(ndef_message, max_send, 0x00) !=M24SR_ACTION_COMPLETED){
			LL_mDelay(25);
			Write_Joke_Message(ndef_message, max_send, 0x00);
		}

		// offsety aby sme si vedeli NDEF spravu spravne rozdelit a vedeli kam ju zapisovat
		celkova_dlzka -= (max_send - param_len);
		mess_offset = max_send - param_len;
		update_offset = max_send;

		while(celkova_dlzka > 0){ //odosielame zvysok spravy/payloadu bez parametrov kedze tie chceme iba raz na zaciatku NDEF spravy
			LL_mDelay(20);
			memset(ndef_message,0,max_send);
			memcpy(ndef_message, message+mess_offset, max_send);

			if(Write_Joke_Message(ndef_message, max_send, update_offset) !=M24SR_ACTION_COMPLETED){
				LL_mDelay(100);
				Write_Joke_Message(ndef_message, max_send, 0x00);
			}

			mess_offset += max_send;
			update_offset += max_send;
			celkova_dlzka -= max_send;
		}
	}

}

uint16_t Write_Joke_Message(uint8_t *NDEFmessage, uint16_t dlzka, uint16_t offset){

//Zacatie I2C komunikacie
	  if ((error = M24SR_KillSession (M24SR_I2C_ADDR_WRITE)) != M24SR_ACTION_COMPLETED)
	  {
	    return error;
	  }

//Odoslanie príkazu SelectNDEFTagApplication
	  if ((error = M24SR_SelectApplication (M24SR_I2C_ADDR_WRITE)) != M24SR_ACTION_COMPLETED)
	  {
	    return error;
	  }

//Vybratie CC súboru
	  if ((error = M24SR_SelectCCfile (M24SR_I2C_ADDR_WRITE)) != M24SR_ACTION_COMPLETED)
	  {
	    return error;
	  }

//vybratie NDEF súboru
	  if ((error = M24SR_SelectNDEFfile (M24SR_I2C_ADDR_WRITE, NDEF_FILE_ID)) != M24SR_ACTION_COMPLETED)
	  {
	    return error;
	  }

//odomknutie NDEF file, aby sme vedeli zapisovat
	  if ((error = M24SR_Verify(M24SR_I2C_ADDR_WRITE, WRITE ,0x10 ,DefaultPassword )) != M24SR_ACTION_COMPLETED)
	  {
	    return error;
	  }
//Zapisanie NDEF spravy
	  if ((error = M24SR_UpdateBinary (M24SR_I2C_ADDR_WRITE, offset , dlzka, NDEFmessage)) != M24SR_ACTION_COMPLETED)
	  {
	  	return error;
	  }
//odomknutie NDEF file, aby sme ho vedeli citat
	  if ((error = M24SR_Verify(M24SR_I2C_ADDR_WRITE, READ ,0x10 ,DefaultPassword )) != M24SR_ACTION_COMPLETED)
	  {
	  	return error;
	  }
//vybratie NDEF súboru
	  if ((error = M24SR_SelectNDEFfile (M24SR_I2C_ADDR_WRITE, NDEF_FILE_ID)) != M24SR_ACTION_COMPLETED)
	  {
	  	return error;
	  }
//prečítanie dĺžky NDEF súboru
	  if ((error = M24SR_ReadBinary (M24SR_I2C_ADDR_WRITE, 0x00 ,0x02 , buffer)) != M24SR_ACTION_COMPLETED)
	  {
	  	return error;
	  }
// ukoncenie komunikacie
	if ((error = M24SR_Deselect (M24SR_I2C_ADDR_WRITE)) != M24SR_ACTION_COMPLETED)
	{
		return error;
	}
	return M24SR_ACTION_COMPLETED;


}

uint16_t disableEnableRead(uint8_t bool){
	//Zacatie I2C komunikacie
		  if ((error = M24SR_KillSession (M24SR_I2C_ADDR_WRITE)) != M24SR_ACTION_COMPLETED)
		  {
		    return error;
		  }

	//Odoslanie príkazu SelectNDEFTagApplication
		  if ((error = M24SR_SelectApplication (M24SR_I2C_ADDR_WRITE)) != M24SR_ACTION_COMPLETED)
		  {
		    return error;
		  }
		  //vybratie NDEF súboru
		  	  if ((error = M24SR_SelectNDEFfile (M24SR_I2C_ADDR_WRITE, NDEF_FILE_ID)) != M24SR_ACTION_COMPLETED)
		  	  {
		  	  	return error;
		  	  }
		  if ((error = M24SR_Verify(M24SR_I2C_ADDR_WRITE, SUPERUSER ,0x10 ,DefaultPassword )) != M24SR_ACTION_COMPLETED)
		  {
		    return error;
		  }

		  if(bool == 1){
			  if((error = M24SR_EnablePermanentState (M24SR_I2C_ADDR_WRITE, READ  )) != M24SR_ACTION_COMPLETED)
				  {
				    return error;
				  }
		  }else{
			  if((error = M24SR_DisablePermanentState (M24SR_I2C_ADDR_WRITE, READ  )) != M24SR_ACTION_COMPLETED)
				{
				return error;
				}
		  }

		  // ukoncenie komunikacie
		  if ((error = M24SR_Deselect (M24SR_I2C_ADDR_WRITE)) != M24SR_ACTION_COMPLETED)
		  {
		  		return error;
		  }
		  return M24SR_ACTION_COMPLETED;
}

uint16_t disableReadProtection(void){
	if ((error = M24SR_KillSession (M24SR_I2C_ADDR_WRITE)) != M24SR_ACTION_COMPLETED)
			  {
			    return error;
			  }

		//Odoslanie príkazu SelectNDEFTagApplication
			  if ((error = M24SR_SelectApplication (M24SR_I2C_ADDR_WRITE)) != M24SR_ACTION_COMPLETED)
			  {
			    return error;
			  }
			  //vybratie NDEF súboru
			  	  if ((error = M24SR_SelectNDEFfile (M24SR_I2C_ADDR_WRITE, NDEF_FILE_ID)) != M24SR_ACTION_COMPLETED)
			  	  {
			  	  	return error;
			  	  }
			  if ((error = M24SR_Verify(M24SR_I2C_ADDR_WRITE, SUPERUSER ,0x10 ,DefaultPassword )) != M24SR_ACTION_COMPLETED)
			  {
			    return error;
			  }
			  if ((error = M24SR_DisableVerificationRequirement (M24SR_I2C_ADDR_WRITE, READ)) != M24SR_ACTION_COMPLETED)
			  {
			    return error;
			  }
			  return M24SR_ACTION_COMPLETED;
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
