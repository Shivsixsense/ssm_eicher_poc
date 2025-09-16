/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
void MX_FDCAN1_Init(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CAN1_SLP_Pin GPIO_PIN_13
#define CAN1_SLP_GPIO_Port GPIOC
#define IGN_SENSE_Pin GPIO_PIN_0
#define IGN_SENSE_GPIO_Port GPIOA
#define IAD_LOAD_Pin GPIO_PIN_4
#define IAD_LOAD_GPIO_Port GPIOA
#define WDG_RST_Pin GPIO_PIN_5
#define WDG_RST_GPIO_Port GPIOA
#define U_LED_Pin GPIO_PIN_6
#define U_LED_GPIO_Port GPIOA
#define AIR_INJECTION_LOAD_Pin GPIO_PIN_2
#define AIR_INJECTION_LOAD_GPIO_Port GPIOB
#define IWD_LOAD_Pin GPIO_PIN_6
#define IWD_LOAD_GPIO_Port GPIOC
#define HIGH_IN3_SEN_Pin GPIO_PIN_7
#define HIGH_IN3_SEN_GPIO_Port GPIOC
#define EX_LOAD2_Pin GPIO_PIN_15
#define EX_LOAD2_GPIO_Port GPIOA
#define EX_LOAD1_Pin GPIO_PIN_0
#define EX_LOAD1_GPIO_Port GPIOD
#define LOW_IN_SENSE_Pin GPIO_PIN_1
#define LOW_IN_SENSE_GPIO_Port GPIOD
#define HIGH_IN2_SENSE_Pin GPIO_PIN_2
#define HIGH_IN2_SENSE_GPIO_Port GPIOD
#define HIGH_IN_SENSE_Pin GPIO_PIN_3
#define HIGH_IN_SENSE_GPIO_Port GPIOD
#define CAN2_SLP_Pin GPIO_PIN_7
#define CAN2_SLP_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
