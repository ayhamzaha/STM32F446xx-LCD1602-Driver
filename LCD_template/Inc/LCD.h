#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "stm32f446xx.h"
#include <stdint.h>

#define MAX_LCDS 1 // Set to max LCDs in project

#define CLEAR_INSTR 0x01U // Clear screen instruction
#define RESET_INSTR 0x02U // Reset screen instruction
#define LSH_INSTR   0x18U // Left-shift display instruction 
#define RSH_INSTR   0x1CU // Right-shift display instruction

typedef enum {
	screen_off,
	screen_on
} SCREEN_PWR;

typedef enum {
	blink_off,
	blink_on
}CURSOR_BLINK;

typedef enum {
	cursor_off,
	cursor_on
}CURSOR;

typedef enum {
	one_line,
	two_line
}DISP_LINES;

typedef enum {
	small,
	large
}FONT;

typedef enum {
	four_bit,
	eight_bit
}MODE;

typedef struct {

	uint8_t Index;

	MODE Mode; // 0 - 4 Bit | 1 - 8 bit, defaults to 4 bit

	GPIO_TypeDef * D0_PORTx;
	uint16_t D0_PINx;

	GPIO_TypeDef * D1_PORTx;
	uint16_t D1_PINx;

	GPIO_TypeDef * D2_PORTx;
	uint16_t D2_PINx;

	GPIO_TypeDef * D3_PORTx;
	uint16_t D3_PINx;

	GPIO_TypeDef * D4_PORTx;
	uint16_t D4_PINx;

	GPIO_TypeDef * D5_PORTx;
	uint16_t D5_PINx;

	GPIO_TypeDef * D6_PORTx;
	uint16_t D6_PINx;

	GPIO_TypeDef * D7_PORTx;
	uint16_t D7_PINx;

	GPIO_TypeDef * EN_PORTx;
	uint16_t EN_PINx;

	GPIO_TypeDef * RS_PORTx;
	uint16_t RS_PINx;

	TIM_TypeDef * TIMx; // Peripheral clock (RCC) for selected timer must be enabled before passing clock address, 32-bit timer only

}LCD_CFG;

extern LCD_CFG LCD[MAX_LCDS]; // Initializes array of LCDs to allow multiple LCDs with diff. configuration

void LCD_Init(uint8_t,MODE,DISP_LINES,FONT,SCREEN_PWR,CURSOR,CURSOR_BLINK); // Initializes LCD screen in either 4-/8-bit mode
void LCD_Clear(uint8_t);                                                    // Clears the LCD screen
void LCD_Char_Write(uint8_t,char);                                          // Writes a character on the screen
void LCD_String_Write(uint8_t,char*);																				// Writes a string of characters on the screen
void LCD_Left_Shift(uint8_t);																								// Shifts the display left
void LCD_Right_Shift(uint8_t);																							// Shifts the display right
void LCD_Set_Cursor(uint8_t,uint8_t,uint8_t);																// Sets the cursors position
void LCD_Reset(uint8_t);																										// Resets cursor and any display shifts 

#endif /* INC_LCD_H_ */
