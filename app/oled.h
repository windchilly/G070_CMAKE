#ifndef __OLED_H__
#define __OLED_H__

#include "stm32g0xx.h"
#include "SGUI_Basic.h"

#define OLED_CMD    0   //Ð´ÃüÁî
#define OLED_DATA   1   //Ð´Êý¾Ý

#define OLED_SCL_Clr() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0)   //SCL
#define OLED_SCL_Set() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,1)

#define OLED_SDA_Clr() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,0)   //SDA
#define OLED_SDA_Set() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,1)

#define OLED_RES_Clr() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,0)   //RES
#define OLED_RES_Set() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,1)

#define OLED_DC_Clr()  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,0)   //DC
#define OLED_DC_Set()  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,1)

#define OLED_CS_Clr()  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,0)   //CS
#define OLED_CS_Set()  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,1)

void refresh(void);
void setpixel(int x, int y, unsigned int t);
void clear(void);
void draw_dino(void);
void oled_init(void);

#endif
