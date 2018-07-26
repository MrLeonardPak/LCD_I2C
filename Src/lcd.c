#include "lcd.h"

extern I2C_HandleTypeDef hi2c1;
extern uint8_t delay;
extern uint16_t DecAddress;
uint8_t portlcd = 0; //ячейка для хранения данных порта микросхемы расширения
uint8_t RL;

void LCD_WriteByteI2CLCD(uint8_t bt) { 
	HAL_I2C_Master_Transmit(&hi2c1, DecAddress, &bt, 1, 1000); 
}

/*__weak uint32_t GetDelay(void){
  return delay;
}

void DelayMicro60( uint8_t micro) {
	uint32_t tmp = GetDelay();
	while ((GetDelay() - tmp) < micro) {
	}
}*/

__STATIC_INLINE void MicroDelay( uint32_t wait) {
	uint32_t tick = SysTick->VAL;
	while ((tick - (SysTick->VAL)) < (RL*wait)) {
	}
}

void sendHalfByte (uint8_t hbt) {
	hbt <<= 4;
	e_set();
	MicroDelay(150);
	LCD_WriteByteI2CLCD(portlcd | hbt);
	e_reset();
	MicroDelay(250);
}

void LCD_sendByte(uint8_t bt, uint8_t mode) {                                    
	if (mode == 0) rs_reset(); 
		else rs_set();
	uint8_t halfbt = 0; // Первый полубайт
	halfbt = bt>>4; 
	sendHalfByte(halfbt);	
	sendHalfByte(bt);
}

void LCD_writeChar (char s) {
	LCD_sendByte((uint8_t) s, 1);
}

void LCD_writeSrting (char *str) {
	uint8_t i = 0;
	while (str[i] != 0) {
		LCD_writeChar(str[i]);
		i++;
	}
}

void LCD_Clear (void) {
	LCD_sendByte(0x01, 0); 
	HAL_Delay(2);
}

void LCD_moveDisplayRight (void) {
	LCD_sendByte(0x14, 0);
	MicroDelay(40);
}

void LCD_moveDisplayLeft (void) {
	LCD_sendByte(0x10, 0);
	MicroDelay(40);
}

void LCD_moveXY (uint8_t x, uint8_t y) {
	x--;
	x |= 0x80;
	switch (y) {
		case 1 :
			LCD_sendByte(x, 0);
			MicroDelay(40);
			break;
		case 2 :
			LCD_sendByte(x | 0x40,0);
			MicroDelay(40);
			break;
	}		
}

void LCD_init(void) {
	RL = ((SysTick->LOAD + 1) / 1000);
	
	HAL_Delay(15);	
	sendHalfByte(0x03);
	MicroDelay(4);
	sendHalfByte(0x03);
	MicroDelay(100);
	sendHalfByte(0x03);
	/////////////////
	sendHalfByte(0x02);
	MicroDelay(40);
	/////////////////
	LCD_sendByte(0x28, 0); // Режим 4 бит
	MicroDelay(40);
	LCD_sendByte(0x0E, 0); // Включаем дисплей, чтобы с мигание 0x0F, без - 0x0E
	MicroDelay(40);
	LCD_sendByte(0x01, 0); // Очистка дисплея
	HAL_Delay(2);
	LCD_sendByte(0x06, 0); // Движение влево
	MicroDelay(40);
	LCD_sendByte(0x02, 0); // В начало
	HAL_Delay(2);
	led_set();
	setwrite();
}
