/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "SEGGER_RTT.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
int _write(int file, char *ptr, int len);
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FootSwitchA_IN_Pin GPIO_PIN_13
#define FootSwitchA_IN_GPIO_Port GPIOC
#define FootSwitchB_IN_Pin GPIO_PIN_14
#define FootSwitchB_IN_GPIO_Port GPIOC
#define RelayCoil_OUT_Pin GPIO_PIN_0
#define RelayCoil_OUT_GPIO_Port GPIOC
#define UserSwitchA_IN_Pin GPIO_PIN_1
#define UserSwitchA_IN_GPIO_Port GPIOC
#define UserSwitchB_IN_Pin GPIO_PIN_2
#define UserSwitchB_IN_GPIO_Port GPIOC
#define FootExp_IN_Pin GPIO_PIN_0
#define FootExp_IN_GPIO_Port GPIOA
#define UserPotA_IN_Pin GPIO_PIN_1
#define UserPotA_IN_GPIO_Port GPIOA
#define UserPotB_IN_Pin GPIO_PIN_2
#define UserPotB_IN_GPIO_Port GPIOA
#define LEDA_RED_Pin GPIO_PIN_0
#define LEDA_RED_GPIO_Port GPIOB
#define LEDA_GREEN_Pin GPIO_PIN_1
#define LEDA_GREEN_GPIO_Port GPIOB
#define LEDA_BLUE_Pin GPIO_PIN_2
#define LEDA_BLUE_GPIO_Port GPIOB
#define LEDB_RED_Pin GPIO_PIN_10
#define LEDB_RED_GPIO_Port GPIOA
#define LEDB_GREEN_Pin GPIO_PIN_11
#define LEDB_GREEN_GPIO_Port GPIOA
#define LEDB_BLUE_Pin GPIO_PIN_12
#define LEDB_BLUE_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
