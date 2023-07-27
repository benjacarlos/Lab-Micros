 /***************************************************************************
  @file     LCD_GDM1602A.c
  @brief    LCD controller
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/

/*************************************************************
 * 		INCLUDES
 *************************************************************/
#include <stdbool.h>
#include <string.h>

#include "SPI_wrapper.h"
#include "SysTick.h"
#include "LCD_GDM1602A.h"
#include "fsl_device_registers.h"
/**************************************************************
 * 		CONSTANTS AND MACROS
 **************************************************************/
#define BLANCK_SPACE			(0x20										)
#define CLEAR					(1											)
#define RET_HOME				(1<<1										)
#define ENTRY_MODE(i_d, sh)		(1<<2 | (i_d)<< 1 	   | (sh)				)
#define DISPLAY(d,c,b)			(1<<3 | (d)  << 2	   | (c)  <<1 | (b)		)
#define SHIFT(s_c, r_l)			(1<<4 | (s_c)<< 3 	   | (r_l)<<2			)
#define FUNCTIONS(dl, n, f)		(1<<5 | (dl) << 4 	   | (n)  <<3 | (f)<<2	)
#define SET_DDRAM(add)			(1<<7 | ((add) & 0x7F)						)

#define ENABLE(x)  				(((x) & 1)<<1								)
#define RS(x)					( (x) & 1									)
#define VAL(x)					(((x) & 0xF)<<4								)

#define BLOCK(e, rs, v) 		(ENABLE(e) | RS(rs) | VAL(v)				)

#define INIT_INSTRUCTIONS		10

#define SHIFTING_BUFFER_LEN		(25*DISPLAY_COLUMNS) //might be excessive...

#define CURSOR_VISIBLE			0
#define CURSOR_BLINK			0

#define FAST_SPEED				400

/*********************************************************
 * 		LOCAL STRUCTS AND ENUMS
 ********************************************************/
typedef enum{
	NOTHING,
	SHIFTING,
	BOUNCING
}lcd_line_state_t;

typedef struct{
	char buffer[SHIFTING_BUFFER_LEN];
	uint8_t pointer;
	uint8_t timer_id;
	uint8_t length;
	uint8_t begin;
	bool direction;
	lcd_line_state_t state;
	lcd_shift_speed_t speed;
}lcd_line_t;
/****************************************************************
 * 		FUNCTION DECLARATION WITH FILE SCOPE
 ******************************************************************/
static void sendBlock(uint8_t byte, uint8_t rs);
static void initRoutineTimerCallback(void);
static void initRoutineSPICallback(void);

static void LCD_writeInstruction(uint8_t instruct);
static void shifttingCallback(void);

/*****************************************************************
 * 		VARIABLES WITH FILE LEVEL SCOPE
 *****************************************************************/
static uint8_t 	init_status = 0;
static uint8_t 	timer_id;
static uint8_t 	init_fase;

static uint8_t init_instructions[INIT_INSTRUCTIONS] = {FUNCTIONS(1,0,0),FUNCTIONS(1,0,0),FUNCTIONS(1,0,0),
									  	  	  	  	   FUNCTIONS(0,0,0),FUNCTIONS(0,1,0),DISPLAY(0,0,0),
													   CLEAR, ENTRY_MODE(1, 0),DISPLAY(1,CURSOR_VISIBLE,CURSOR_BLINK),
													   RET_HOME};
static uint8_t init_delays[INIT_INSTRUCTIONS] = {5,1,1,1,1,1,2,1,1,2};

static bool send_status = true; // Free to send something

/*
static char lines[DISPLAY_ROWS][SHIFTING_BUFFER_LEN];
static bool shiftingLine[DISPLAY_ROWS] = {0U};
static uint8_t shifting_p[DISPLAY_ROWS] = {0U};
static uint8_t shifting_timer;
*/
static lcd_line_t lcdLines[DISPLAY_ROWS];
/**********************************************************************
 * 		FUNCTION WITH GOBAL SCOPE
 **********************************************************************/
void LCD_Init(void)
{
	if(init_status == 0)
	{
		SPI_Init(SPI_0_ID, SPI_SLAVE_0, 350000U);
		SysTick_Init();
		init_fase = 0;

		timer_id = SysTick_AddCallback(initRoutineTimerCallback, 15);
		for(int i = 0; i < DISPLAY_ROWS; i++)
		{
			lcdLines[i].length = 0;
			lcdLines[i].pointer = 0;
			lcdLines[i].state = NOTHING;
			lcdLines[i].timer_id = SysTick_AddCallback(shifttingCallback, FAST_SPEED);
			Systick_PauseCallback(lcdLines[i].timer_id);
		}
		init_status = 1;

	}
}

void LCD_UpdateClock(void)
{
	SPI_Config(SPI_0_ID, SPI_SLAVE_0, 350000U);
}

bool LCD_isInit(void)
{
	return (init_status == 2);
}

void LCD_writeData(uint8_t data)
{
	sendBlock(data, 1);
}

bool LCD_setCursor(uint8_t row, uint8_t column)
{
	if(row < DISPLAY_ROWS && column < DISPLAY_COLUMNS)
	{
		uint8_t address = column + row * 0x40;
		LCD_writeInstruction(SET_DDRAM(address));
		//lcdLines[row].state = NOTHING;
		return true;
	}
	return false;
}

void LCD_writeDataInPos(uint8_t data, uint8_t row, uint8_t column)
{

	if(LCD_setCursor(row, column))
	{
		LCD_writeData(data);
	}
}

void LCD_writeStrInPos(char * str, uint8_t len, uint8_t row, uint8_t column)
{
	uint16_t i, max = ((column + len ) < DISPLAY_COLUMNS)? column + len : DISPLAY_COLUMNS ;
	if(LCD_setCursor(row, column))
	{
		for(i = 0; i<max; i++)
		{
			LCD_writeData(str[i]);
		}
	}
}

void LCD_clearRow(uint8_t row)
{
	uint8_t i;
	memset(lcdLines[row].buffer, 0x20, SHIFTING_BUFFER_LEN);
	if(LCD_setCursor(row, 0))
	{
		for(i = 0; i<DISPLAY_COLUMNS; i++)
		{
			LCD_writeData(BLANCK_SPACE);
		}
		LCD_setCursor(row, 0); // back to the beginning of the row
		lcdLines[row].state = NOTHING;
	}
}

void LCD_stopMove(uint8_t row)
{
	lcdLines[row].state = NOTHING;
}

void LCD_clearDisplay(void)
{
	// Not using the display build in function to have better timing control
	// Slightly slower
	uint8_t i;
	memset(lcdLines[0].buffer, 0x20, SHIFTING_BUFFER_LEN);
	memset(lcdLines[1].buffer, 0x20, SHIFTING_BUFFER_LEN);
	LCD_setCursor(0, 0);
	for(i = 0; i<DISPLAY_COLUMNS; i++)
	{
		LCD_writeData(BLANCK_SPACE);
	}
	lcdLines[0].state = NOTHING;
	LCD_setCursor(1, 0);
	for(i = 0; i<DISPLAY_COLUMNS; i++)
	{
		LCD_writeData(BLANCK_SPACE);
	}
	LCD_setCursor(0, 0); // back to the beginning of the display
	lcdLines[1].state = NOTHING;
}

void LCD_writeShiftingStr(char * str, uint8_t len, uint8_t row, lcd_shift_speed_t speed)
{
	uint8_t i;
	if(row < DISPLAY_ROWS)
	{
		memset(lcdLines[row].buffer, 0x20, SHIFTING_BUFFER_LEN);
		for(i = 0; i < len; i++)
		{
			lcdLines[row].buffer[i] = str[i];
		}

		Systick_ChangeCallbackPeriod(lcdLines[row].timer_id, (3-speed)*FAST_SPEED);
		lcdLines[row].speed = speed;

		if(lcdLines[row].state == NOTHING)
		{
			Systick_ResumeCallback(lcdLines[row].timer_id);
		}

		lcdLines[row].state = SHIFTING;
		lcdLines[row].length = len;
		lcdLines[row].pointer = 0;
		lcdLines[row].begin = 0;
	}
}

/*
void LCD_writeBouncingStr(char * str, uint8_t len, uint8_t row, uint8_t begin, lcd_shift_speed_t speed)
{
	uint8_t i;
	if(row < DISPLAY_ROWS)
	{
		memset(lcdLines[row].buffer, 0x20, SHIFTING_BUFFER_LEN);
		for(i = 0; i < len; i++)
		{
			lcdLines[row].buffer[i] = str[i];
		}
		if(len+begin <= DISPLAY_COLUMNS) // static
		{
			LCD_writeStrInPos(lcdLines[row].buffer, DISPLAY_COLUMNS, row, begin);
			Systick_PauseCallback(lcdLines[row].timer_id);
			lcdLines[row].state = NOTHING;
			return;
		}
		Systick_ChangeCallbackPeriod(lcdLines[row].timer_id, (3-speed)*FAST_SPEED);
		lcdLines[row].speed = speed;

		if(lcdLines[row].state == NOTHING)
		{
			Systick_ResumeCallback(lcdLines[row].timer_id);
		}

		lcdLines[row].state = BOUNCING;
		lcdLines[row].pointer = 0;
		lcdLines[row].begin = begin;
		lcdLines[row].length = len;
		lcdLines[row].direction = true;
	}
}*/
/*
void LCD_changeState(bool state)
{
	volatile uint8_t val = DISPLAY(state,CURSOR_VISIBLE,CURSOR_BLINK);
	printf("Alguien esta matando todo");
	LCD_writeInstruction(val);
}
*/

/****************************************************************
 * 		FUNCTIONS WITH FILE LEVEL SCOPE
 ***************************************************************/
static void LCD_writeInstruction(uint8_t instruct)
{
	sendBlock(instruct, 0);
}

static void sendBlock(uint8_t byte, uint8_t rs)
{
	char msg[6] = {0U};

	// MSB first
	msg[0] = BLOCK(0, rs, (byte & 0xF0) >> 4);
	msg[1] = BLOCK(1, rs, (byte & 0xF0) >> 4);
	msg[2] = BLOCK(0, rs, (byte & 0xF0) >> 4);

	msg[3] = BLOCK(0, rs, byte & 0xF);
	msg[4] = BLOCK(1, rs, byte & 0xF);
	msg[5] = BLOCK(0, rs, byte & 0xF);

	SPI_Send(SPI_0_ID, SPI_SLAVE_0, msg, 6, (init_status == 2)? NULL:initRoutineSPICallback);
	send_status = false;
}

static void sendNybble(uint8_t nybble, uint8_t rs)
{
	char msg[6] = {0U};

	// MSB first
	msg[0] = BLOCK(0, rs, (nybble & 0xF0)>>4);
	msg[1] = BLOCK(1, rs, (nybble & 0xF0)>>4);
	msg[2] = BLOCK(0, rs, (nybble & 0xF0)>>4);

	SPI_Send(SPI_0_ID, SPI_SLAVE_0, msg, 3, (init_status == 2)? NULL:initRoutineSPICallback);
	send_status = false;
}

static void initRoutineTimerCallback(void)
{
	Systick_PauseCallback(timer_id);

	if (init_fase < 4)
	{
		Systick_ChangeCallbackPeriod(timer_id, init_delays[init_fase]);
		sendNybble(init_instructions[init_fase], 0);
	}
	else if(init_fase < INIT_INSTRUCTIONS)
	{
		Systick_ChangeCallbackPeriod(timer_id, init_delays[init_fase]);
		LCD_writeInstruction(init_instructions[init_fase]);
	}
	else
	{
		Systick_ClrCallback(timer_id);
		init_status = 2;
	}
}

static void initRoutineSPICallback(void)
{
	send_status = true;
	Systick_ResumeCallback(timer_id);
	init_fase++;
}


static void shifttingCallback(void)
{

	char buffer[DISPLAY_COLUMNS];
	lcd_line_state_t curr;
	for(int i = 0; i < DISPLAY_ROWS; i++)
	{
		if(!Systick_GetStatus(lcdLines[i].timer_id))
		{
			continue;
		}
		curr = lcdLines[i].state;
		if(curr == NOTHING)
		{
			Systick_PauseCallback(lcdLines[i].timer_id);
		}
		else
		{
			uint8_t start = lcdLines[i].begin;
			for(int j = 0; j<(DISPLAY_COLUMNS-start); j++)
			{
				buffer[j] = lcdLines[i].buffer[(lcdLines[i].pointer + j)%lcdLines[i].length];
			}

			/*if(curr == BOUNCING)
			{
				if(!lcdLines[i].direction && lcdLines[i].pointer==0)
					lcdLines[i].direction = true;
				else if(lcdLines[i].direction && ((start + lcdLines[i].length -  lcdLines[i].pointer) <= DISPLAY_COLUMNS))
					lcdLines[i].direction = false;

				lcdLines[i].pointer += lcdLines[i].direction?1:-1;
			}
			else*/
			lcdLines[i].pointer = (lcdLines[i].pointer + 1)%SHIFTING_BUFFER_LEN;

			LCD_writeStrInPos(buffer, DISPLAY_COLUMNS-start, i, start);
		}
	}
}
