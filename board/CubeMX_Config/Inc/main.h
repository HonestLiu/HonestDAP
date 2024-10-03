/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SPI_FLASH_CS_Pin GPIO_PIN_4
#define SPI_FLASH_CS_GPIO_Port GPIOA
#define SD_CARD_DET_Pin GPIO_PIN_3
#define SD_CARD_DET_GPIO_Port GPIOD

#define V_5V0_OUT_MEASURE_Pin GPIO_PIN_0
#define V_5V0_OUT_MEASURE_GPIO_Port GPIOC
#define I_5V0_OUT_MEASURE_Pin GPIO_PIN_1
#define I_5V0_OUT_MEASURE_GPIO_Port GPIOC
#define V_3V3_OUT_MEASURE_Pin GPIO_PIN_2
#define V_3V3_OUT_MEASURE_GPIO_Port GPIOC
#define I_3V3_OUT_MEASURE_Pin GPIO_PIN_3
#define I_3V3_OUT_MEASURE_GPIO_Port GPIOC
#define I_PD_OUT_MEASURE2_Pin GPIO_PIN_3
#define I_PD_OUT_MEASURE2_GPIO_Port GPIOA
#define SPI_FLASH_CS_Pin GPIO_PIN_4
#define SPI_FLASH_CS_GPIO_Port GPIOA
#define DAC_OUT_Pin GPIO_PIN_5
#define DAC_OUT_GPIO_Port GPIOA
#define V_PD_OUT_MEASURE_Pin GPIO_PIN_4
#define V_PD_OUT_MEASURE_GPIO_Port GPIOC
#define I_PD_OUT_MEASURE1_Pin GPIO_PIN_5
#define I_PD_OUT_MEASURE1_GPIO_Port GPIOC
#define USER_LED_Pin GPIO_PIN_2
#define USER_LED_GPIO_Port GPIOB
#define TCK_SWCLK_Pin GPIO_PIN_7
#define TCK_SWCLK_GPIO_Port GPIOE
#define TMS_SWDIO_Pin GPIO_PIN_8
#define TMS_SWDIO_GPIO_Port GPIOE
#define PWM_OUT_Pin GPIO_PIN_9
#define PWM_OUT_GPIO_Port GPIOE
#define NRST_Pin GPIO_PIN_10
#define NRST_GPIO_Port GPIOE
#define TDI_Pin GPIO_PIN_12
#define TDI_GPIO_Port GPIOE
#define LCD_CS_Pin GPIO_PIN_12
#define LCD_CS_GPIO_Port GPIOB
#define LCD_SCL_Pin GPIO_PIN_13
#define LCD_SCL_GPIO_Port GPIOB
#define LCD_BLK_Pin GPIO_PIN_14
#define LCD_BLK_GPIO_Port GPIOB
#define LCD_SDA_Pin GPIO_PIN_15
#define LCD_SDA_GPIO_Port GPIOB
#define CDC_TX_Pin GPIO_PIN_8
#define CDC_TX_GPIO_Port GPIOD
#define CDC_RX_Pin GPIO_PIN_9
#define CDC_RX_GPIO_Port GPIOD
#define SD_CARD_DET_Pin GPIO_PIN_3
#define SD_CARD_DET_GPIO_Port GPIOD
#define TDO_Pin GPIO_PIN_6
#define TDO_GPIO_Port GPIOD
#define BEEP_PWM_Pin GPIO_PIN_9
#define BEEP_PWM_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
