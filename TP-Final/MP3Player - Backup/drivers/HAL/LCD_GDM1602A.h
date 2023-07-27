 /***************************************************************************
  @file     LCD_GDM1602A.h
  @brief    LCD controller
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/

#ifndef LCD_GDM1602A_H_
#define LCD_GDM1602A_H_

#include <stdbool.h>

#define DISPLAY_ROWS			2
#define DISPLAY_COLUMNS			16

typedef  enum{
	SLOW,
	MIDIUM,
	FAST
}lcd_shift_speed_t;

/**
 * Initialize the LCD using SPI interface
 */
void LCD_Init(void);

bool LCD_isInit(void);

void LCD_stopMove(uint8_t row);

void LCD_UpdateClock(void);

void LCD_writeData(uint8_t data);

/**
 * @brief Set the cursor in a fixed position
 * @param 	row: 0 for the upper line, 1 for the lower one
 * 		  	column: position to begin writing the string
 * @return if a valid position was selected
 */
bool LCD_setCursor(uint8_t row, uint8_t column);

/**
 * @brief Write a single data in the display into a determin row and column
 * @param 	data: char to write
 * 		  	row: 0 for the upper line, 1 for the lower one
 * 		  	column: position to begin writing the string
 */
void LCD_writeDataInPos(uint8_t data, uint8_t row, uint8_t column);

/**
 * @brief write a string from a position in the display
 * @param 	str: buffer
 * 		  	len: string length
 * 		  	row: 0 for the upper line, 1 for the lower one
 * 		  	column: position to begin writing the string
 *  Overflow from the display not manage
 */
void LCD_writeStrInPos(char * str, uint8_t len, uint8_t row, uint8_t column);

/**
 * @brief Write a shifting string in one line, writing again with another function will stop shifting
 * @param 	str: buffer
 * 			len: buffer len
 * 			row: row to write 0-> upper, 1->lower
 * 			Sped: low, med or fast shifting
 */
void LCD_writeShiftingStr(char * str, uint8_t len, uint8_t row, lcd_shift_speed_t speed);

/**
 * @brief Write a string in the display that bounce between begin and the end of the string
 */
//void LCD_writeBouncingStr(char * str, uint8_t len, uint8_t row, uint8_t begin, lcd_shift_speed_t speed);

/**
 * @brief Clean one line of the display
 * @param row: 0 for the upper line, 1 for the lower one
 */
void LCD_clearRow(uint8_t row);

/**
 * @brief Clean everything in the display
 */
void LCD_clearDisplay(void);

/*
 * @brief Turn display on/off
 * @param state: true for turning on, false turn off
 */
void LCD_changeState(bool state);

#endif /* LCD_GDM1602A_H_ */
