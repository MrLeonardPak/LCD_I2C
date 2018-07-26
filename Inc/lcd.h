#ifndef __LCD_H__
#define __LCD_H__

#include "stm32f1xx_hal.h" 
#include "stm32f1xx_it.h"
#include "dwt_stm32_delay.h"

#define e_set() LCD_WriteByteI2CLCD(portlcd |= 0x04)
#define e_reset() LCD_WriteByteI2CLCD(portlcd &= ~0x04)
#define rs_set() LCD_WriteByteI2CLCD(portlcd |= 0x01)
#define rs_reset() LCD_WriteByteI2CLCD(portlcd &= ~0x01)
#define led_set() LCD_WriteByteI2CLCD(portlcd |= 0x08);
#define setwrite() LCD_WriteByteI2CLCD(portlcd &= ~0x02)



//typedef struct {I2C_HandleTypeDef *hi2c; uint16_t DecAddress;} LCD1602;     

void LCD_sendByte (uint8_t bt, uint8_t mode);
void LCD_init(void);
void LCD_writeChar (char s);
void LCD_writeSrting (char *str);
void LCD_Clear (void);
void LCD_moveXY (uint8_t x, uint8_t y);
void LCD_moveDisplayRight (void);
void LCD_moveDisplayLeft (void);

#endif /* __LCD_H__ */
