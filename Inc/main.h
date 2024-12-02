#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Exported functions prototypes ---------------------------------------------*/

/* Private defines -----------------------------------------------------------*/
#define DAC_IN_Pin GPIO_PIN_1
#define DAC_IN_GPIO_Port GPIOC
#define DAC_OUT_Pin GPIO_PIN_0
#define DAC_OUT_GPIO_Port GPIOA
#define OLED_CS_Pin GPIO_PIN_2
#define OLED_CS_GPIO_Port GPIOA
#define OLED_DC_Pin GPIO_PIN_3
#define OLED_DC_GPIO_Port GPIOA
#define OLED_RST_Pin GPIO_PIN_4
#define OLED_RST_GPIO_Port GPIOA
#define BUTTON_1_Pin GPIO_PIN_15
#define BUTTON_1_GPIO_Port GPIOA
#define ST_LED1_Pin GPIO_PIN_8
#define ST_LED1_GPIO_Port GPIOC
#define ST_LED2_Pin GPIO_PIN_9
#define ST_LED2_GPIO_Port GPIOC
#define FLASH_CS_Pin GPIO_PIN_2
#define FLASH_CS_GPIO_Port GPIOD

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
