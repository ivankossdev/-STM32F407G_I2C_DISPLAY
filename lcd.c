#include "lcd.h"
uint8_t buf[1]={0};
extern I2C_HandleTypeDef hi2c1;
char str1[100];
uint8_t portlcd;


/*00 DelayMicro()
 * Функция формирует задержку передачи сигнала
 * в LCD Дисплей
 *
 * */
__STATIC_INLINE void DelayMicro(__IO uint32_t micros)
{
	micros *=(SystemCoreClock / 1000000) / 5;
	while (micros--);
}


/*01 LCD_WriteByteI2CLCD()
 * Функция отправляет байт в шину I2C
 *
 *
 * */
void LCD_WriteByteI2CLCD(uint8_t bt)
{
	buf[0]=bt;
	HAL_I2C_Master_Transmit(&hi2c1,(uint16_t) 0x4E,buf,1,1000);
}


/*02 sendhalfbyte()
 * Функция формирует полубайт в 4 старших разядах
 * для 4 проводного интерфейса
 *
 * */
void sendhalfbyte(uint8_t c)
{
	c<<=4;
	e_set();
	DelayMicro(50);
	LCD_WriteByteI2CLCD(portlcd|c);
	e_reset();
	DelayMicro(50);
}


/*03 sendbyte()
 * Функция отправляет байт в LCD Дисплей
 *
 *
 * */
void sendbyte(uint8_t c, uint8_t mode)
{
	if(mode==0) rs_reset();
	else rs_set();
	uint8_t hc=0;
	hc=c>>4;
	sendhalfbyte(hc);sendhalfbyte(c);
}


/*04 LCD_Clear()
 * Функция очищает LCD Дисплей
 *
 *
 * */
void LCD_Clear(void)
{
	sendbyte(0x01,0);
	HAL_Delay(2);
}


/*05 LCD_SendChar()
 * Функция отправляет символ на LCD Дисплей
 *
 *
 * */
void LCD_SendChar(char ch)
{
	sendbyte(ch,1);
}


/*06 LCD_SendChar()
 * Функция отправляет символ на LCD Дисплей
 *
 *
 * */
void LCD_String(char* st)
{
	uint8_t i=0;
	while(st[i]!=0)
	{
		sendbyte(st[i],1);
		i++;
	}
}


/*07 LCD_SetPos()
 * Функция устанавливает положение символа на LCD Дисплей
 *
 *
 * */
void LCD_SetPos(uint8_t x, uint8_t y)
{
	switch(y)
	{
		case 0:
			sendbyte(x|0x80,0);
			HAL_Delay(1);
			break;
		case 1:
			sendbyte((0x40+x)|0x80,0);
			HAL_Delay(1);
			break;
	}
}
/*07 LCD_SetPos()
 * Функция инициализирует LCD Дисплей
 *
 *
 * */
void LCD_ini(void)
{
	HAL_Delay(15);
	sendhalfbyte(0x03);
	HAL_Delay(4);
	sendhalfbyte(0x03);
	DelayMicro(100);
	sendhalfbyte(0x03);
	HAL_Delay(1);
	sendhalfbyte(0x02);
	HAL_Delay(1);
	sendbyte(0x28,0);
	HAL_Delay(1);
	sendbyte(0x0C,0);
	HAL_Delay(1);
	sendbyte(0x01,0);
	HAL_Delay(2);
	sendbyte(0x06,0);
	HAL_Delay(1);
	setled();
	setwrite();
}

