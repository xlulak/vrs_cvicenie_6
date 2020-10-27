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
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"

void process_serial_data_read(uint8_t ch);
void SystemClock_Config(void);

void process_serial_data(uint8_t ch);

int main(void)
{

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);			//nastavenie hodin , povolenie
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  SystemClock_Config();

  MX_GPIO_Init();												//nastavenie GPIO pinu
  MX_USART2_UART_Init();

  USART2_RegisterCallback(process_serial_data_read);			//toto som modifikoval na _read





  char tx_LED_state = 'u';

  while (1)
  {
	  	if ( LL_GPIO_IsOutputPinSet(GPIOB, LL_GPIO_PIN_3)){
	  		tx_LED_state ='o';
	  	}else
	  	{
	  		tx_LED_state='f';
	  	}
	 // tx_LED_state = LL_GPIO_IsOutputPinSet(GPIOB, LL_GPIO_PIN_3);
	  LL_USART_TransmitData8(USART2, tx_LED_state);
	  LL_mDelay(50);
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  Error_Handler();  
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  
  }
  LL_Init1msTick(8000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(8000000);
}


void process_serial_data(uint8_t ch)
{
	static uint8_t count = 0;

	if(ch == 'a')
	{
		count++;

		if(count >= 3)
		{
			if((LL_GPIO_ReadInputPort(GPIOB) & (1 << 3)) >> 3)
			{
				LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);
			}
			else
			{
				LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3);
			}

			count = 0;
			return;
		}
	}
}

char string[6];

void vynuluj_string(){
	  memset(string, 0, strlen(string));								//vynuluje políČko
}

void PosunPole()
{
	int i=0;
	char h[6];
	strcpy(h,string);		// string ulozi do h
	vynuluj_string();
							//pre istotu
		for(i=0;i<5;i++){	//5 znakov sa prepise
			string[i]=h[i+1];
		}
}

void load(uint8_t ch)
{
	int l=strlen(string);

	if(l==6){
				PosunPole();
				string[5]=ch;
	}else
	{
				string[l]=ch;
	}
}


void process_serial_data_read(uint8_t ch)
{
//	static uint8_t count = 0;

	static char on[]="ledON";
	static char off[]="ledOFF";
	load(ch);
	char *new_on,*new_off;
	new_on = strstr(string,on);
	new_off = strstr(string,off);

	if(strcmp(on,string)==0 || strcmp(on,new_on)==0)
	{
		LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3);
		vynuluj_string();
	}
	else if(strcmp(off,string)==0 || strcmp(off,new_off)==0)
	{
		LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);
		vynuluj_string();
	}

}






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
