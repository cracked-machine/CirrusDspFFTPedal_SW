/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "adc.h"
#include "dma.h"
#include "i2s.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <math.h>
#include <stdio.h>
#include "SEGGER_RTT.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BufSize 1000
#define Overlap 100

#define ADC_DATA_LEN 32
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */



int Buf[BufSize];

int WtrP;
float Rd_P;
float Shift;
float CrossFade;
float a0, a1, a2, b1, b2, hp_in_z1, hp_in_z2, hp_out_z1, hp_out_z2;

uint32_t adc_data_in = 2048;
uint32_t adc_dma_data_in[ADC_DATA_LEN] = {};

uint16_t rxBuf[8];
uint16_t txBuf[8];




/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int _write(int file, char *ptr, int len) {
  (void) file;  /// Not used, avoid warning
  SEGGER_RTT_Write(0, ptr, len);
  return len;
}

float calc_adc_oversample()
{
	float adc_samples_avg = 0.0;
	float adc_samples_sum = 0.0;

	for(int i=0; i<ADC_DATA_LEN; i++)
	{
		adc_samples_sum += adc_dma_data_in[i];
	}
	adc_samples_avg = adc_samples_sum / ADC_DATA_LEN;

	return adc_samples_avg;
}

int Do_HighPass (int inSample) {

	//Shift = adc_data_in/100;
	//300Hz high-pass, 96k
	a0 = 0.9862117951198142f;
	a1 = -1.9724235902396283f;
	a2 = 0.9862117951198142f;
	b1 = -1.972233470205696f;
	b2 = 0.9726137102735608f;
/*
	//Shift = adc_data_in/100;
	//300Hz high-pass, 192k
	a0 = 0.9930819695180432;
	a1 = -1.9861639390360863;
	a2 = 0.9930819695180432;
	b1 = -1.9861160802293198;
	b2 = 0.986211797842853;
*/

	float inSampleF = (float)inSample;
	float outSampleF =
			a0 * inSampleF
			+ a1 * hp_in_z1
			+ a2 * hp_in_z2
			- b1 * hp_out_z1
			- b2 * hp_out_z2;
	hp_in_z2 = hp_in_z1;
	hp_in_z1 = inSampleF;
	hp_out_z2 = hp_out_z1;
	hp_out_z1 = outSampleF;

	// TODO
	//outSampleF = outSampleF * 0.1f;

	return (int) outSampleF;
}


int Do_PitchShift(int lSample, int rSample) {
	int sum = lSample + rSample;
	//sum up and do high-pass
	sum=Do_HighPass(sum);

	//write to ringbuffer
	Buf[WtrP] = sum;

	//read fractional readpointer and generate 0° and 180° read-pointer in integer
	int RdPtr_Int = roundf(Rd_P);
	int RdPtr_Int2 = 0;
	if (RdPtr_Int >= BufSize/2) RdPtr_Int2 = RdPtr_Int - (BufSize/2);
	else RdPtr_Int2 = RdPtr_Int + (BufSize/2);

	//read the two samples...
	float Rd0 = (float) Buf[RdPtr_Int];
	float Rd1 = (float) Buf[RdPtr_Int2];

	//Check if first readpointer starts overlap with write pointer?
	// if yes -> do cross-fade to second read-pointer
	if (Overlap >= (WtrP-RdPtr_Int) && (WtrP-RdPtr_Int) >= 0 && Shift!=1.0f) {
		int rel = WtrP-RdPtr_Int;
		CrossFade = ((float)rel)/(float)Overlap;
	}
	else if (WtrP-RdPtr_Int == 0) CrossFade = 0.0f;

	//Check if second readpointer starts overlap with write pointer?
	// if yes -> do cross-fade to first read-pointer
	if (Overlap >= (WtrP-RdPtr_Int2) && (WtrP-RdPtr_Int2) >= 0 && Shift!=1.0f) {
			int rel = WtrP-RdPtr_Int2;
			CrossFade = 1.0f - ((float)rel)/(float)Overlap;
		}
	else if (WtrP-RdPtr_Int2 == 0) CrossFade = 1.0f;


	//do cross-fading and sum up
	sum = (Rd0*CrossFade + Rd1*(1.0f-CrossFade));

	//increment fractional read-pointer and write-pointer
	Rd_P += Shift;
	WtrP++;
	if (WtrP == BufSize) WtrP = 0;
	if (roundf(Rd_P) >= BufSize) Rd_P = 0.0f;

	return sum;


}

void HAL_I2SEx_TxRxHalfCpltCallback(I2S_HandleTypeDef *hi2s){

	//restore signed 24 bit sample from 16-bit buffers
	int lSample = (int) (rxBuf[0]<<16)|rxBuf[1];
	int rSample = (int) (rxBuf[2]<<16)|rxBuf[3];

	//SEGGER_RTT_printf(0, "lSample:%lu\r\n", lSample);
	printf("lSample:%i\r\n", lSample);

	int ret_sample = Do_PitchShift(lSample, rSample);
	lSample = ret_sample;
	rSample = ret_sample;

	//restore to buffer
	txBuf[0] = (lSample>>16)&0xFFFF;
	txBuf[1] = lSample&0xFFFF;
	txBuf[2] = (rSample>>16)&0xFFFF;
	txBuf[3] = rSample&0xFFFF;

}

void HAL_I2SEx_TxRxCpltCallback(I2S_HandleTypeDef *hi2s){

	//restore signed 24 bit sample from 16-bit buffers
	int lSample = (int) (rxBuf[4]<<16)|rxBuf[5];
	int rSample = (int) (rxBuf[6]<<16)|rxBuf[7];

	//printf("%i\n", lSample);

	int ret_sample = Do_PitchShift(lSample, rSample);
	lSample = ret_sample;
	rSample = ret_sample;

	//restore to buffer
	txBuf[4] = (lSample>>16)&0xFFFF;
	txBuf[5] = lSample&0xFFFF;
	txBuf[6] = (rSample>>16)&0xFFFF;
	txBuf[7] = rSample&0xFFFF;

}

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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2S2_Init();
  /* USER CODE BEGIN 2 */
  SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_TRIM);
  //int res = testfunc();
  HAL_I2SEx_TransmitReceive_DMA (&hi2s2, txBuf, rxBuf, 4);

	//0°
	WtrP = 0;

	Rd_P = 0.0f;
	Shift = 1.0f;
	CrossFade = 1.0f;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  //HAL_GPIO_TogglePin(RelayCoil_OUT_GPIO_Port, RelayCoil_OUT_Pin);
	  //HAL_Delay(1000);
/*	  HAL_GPIO_TogglePin(LEDA_RED_GPIO_Port, LEDA_RED_Pin);
	  HAL_GPIO_TogglePin(LEDB_RED_GPIO_Port, LEDB_RED_Pin);
	  HAL_Delay(1000);
	  HAL_GPIO_TogglePin(LEDA_RED_GPIO_Port, LEDA_RED_Pin);
	  HAL_GPIO_TogglePin(LEDB_RED_GPIO_Port, LEDB_RED_Pin);


	  HAL_GPIO_TogglePin(LEDA_GREEN_GPIO_Port, LEDA_GREEN_Pin);
	  HAL_GPIO_TogglePin(LEDB_GREEN_GPIO_Port, LEDB_GREEN_Pin);
	  HAL_Delay(1000);
	  HAL_GPIO_TogglePin(LEDA_GREEN_GPIO_Port, LEDA_GREEN_Pin);
	  HAL_GPIO_TogglePin(LEDB_GREEN_GPIO_Port, LEDB_GREEN_Pin);


	  HAL_GPIO_TogglePin(LEDA_BLUE_GPIO_Port, LEDA_BLUE_Pin);
	  HAL_GPIO_TogglePin(LEDB_BLUE_GPIO_Port, LEDB_BLUE_Pin);
	  HAL_Delay(1000);
	  HAL_GPIO_TogglePin(LEDA_BLUE_GPIO_Port, LEDA_BLUE_Pin);
	  HAL_GPIO_TogglePin(LEDB_BLUE_GPIO_Port, LEDB_BLUE_Pin);
*/


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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV8;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 384;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
