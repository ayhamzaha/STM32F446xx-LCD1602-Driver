#include "../Inc/LCD.h"
#include "../Inc/Delay.h"

void GPIO_Write(GPIO_TypeDef * ,uint16_t ,uint8_t); // Sets GPIO pin output level high or low.
void LCD_Data(uint8_t , unsigned char);             // Sets GPIO output pins to send data for both 4-/8-bit modes
void LCD_Send_Command(uint8_t , unsigned char);     // Sends commands to LCD module by sending data then toggling the EN pin

void GPIO_Write(GPIO_TypeDef * GPIOx,uint16_t GPIO_Pin,uint8_t PinState) {
	if(PinState != 0) GPIOx->BSRR = (1U << GPIO_Pin);
	else GPIOx->BSRR = (1U << ((uint32_t)GPIO_Pin + 16U));
}

void LCD_Send_Command(uint8_t LCD_Index, unsigned char cmd) {

	GPIO_Write(LCD[LCD_Index].RS_PORTx, LCD[LCD_Index].RS_PINx, 0); // Reset RS pin

	LCD_Data(LCD_Index, cmd); // Send the data

	// Toggle the EN pin
	GPIO_Write(LCD[LCD_Index].EN_PORTx, LCD[LCD_Index].EN_PINx, 0);
	Delay_us(LCD[LCD_Index].TIMx, 5);
	GPIO_Write(LCD[LCD_Index].EN_PORTx, LCD[LCD_Index].EN_PINx, 1);
	Delay_us(LCD[LCD_Index].TIMx, 5);
	GPIO_Write(LCD[LCD_Index].EN_PORTx, LCD[LCD_Index].EN_PINx, 0);
	Delay_us(LCD[LCD_Index].TIMx, 100);
}

void LCD_Data(uint8_t LCD_Index, unsigned char data) {

		// D0 pin
		if(LCD[LCD_Index].Mode ? 1 : 0){
			GPIO_Write(LCD[LCD_Index].D0_PORTx,LCD[LCD_Index].D0_PINx , (data & 1));
		}

		// D1 pin
		if(LCD[LCD_Index].Mode ? 1 : 0){
			GPIO_Write(LCD[LCD_Index].D1_PORTx, LCD[LCD_Index].D1_PINx, (data & 2));
		}

		// D2 pin
		if(LCD[LCD_Index].Mode ? 1 : 0){
			GPIO_Write(LCD[LCD_Index].D2_PORTx, LCD[LCD_Index].D2_PINx, (data & 4));
		}

		// D3 pin
		if(LCD[LCD_Index].Mode ? 1 : 0){
			GPIO_Write(LCD[LCD_Index].D3_PORTx, LCD[LCD_Index].D3_PINx, (data & 8));
		}

		// D4 pin
		GPIO_Write(LCD[LCD_Index].D4_PORTx, LCD[LCD_Index].D4_PINx, LCD[LCD_Index].Mode ? (data & 16) : (data & 1));

		// D5 pin
		GPIO_Write(LCD[LCD_Index].D5_PORTx,LCD[LCD_Index].D5_PINx , LCD[LCD_Index].Mode ? (data & 32) : (data & 2));

		// D6 pin
		GPIO_Write(LCD[LCD_Index].D6_PORTx, LCD[LCD_Index].D6_PINx, LCD[LCD_Index].Mode ? (data & 64) : (data & 4));

		// D7 pin
		GPIO_Write(LCD[LCD_Index].D7_PORTx, LCD[LCD_Index].D7_PINx, LCD[LCD_Index].Mode ? (data & 128) : (data & 8));

}

void LCD_Init(uint8_t LCD_Index, MODE DL, DISP_LINES N, FONT F, SCREEN_PWR D, CURSOR C, CURSOR_BLINK B) {

	// Initialize timer delay function
	TIMx_InitDelay(LCD[LCD_Index].TIMx);
	
	uint8_t disp_cfg1 = (1U << (5U)) | (DL << (4U)) | (N << (3U)) | (F << (2U));
	uint8_t disp_cfg2 = (1U << (3U)) | (D << (2U)) | (C << (1U)) | (B << (0U));
	LCD[LCD_Index].Mode = DL;
	
	// Wait 50ms after power on
	Delay_us(LCD[LCD_Index].TIMx, 50000);
	
	// RS (Register Select) set to 0 (Instruction Register selected)
	GPIO_Write(LCD[LCD_Index].RS_PORTx,LCD[LCD_Index].RS_PINx, 0);
	GPIO_Write(LCD[LCD_Index].EN_PORTx,LCD[LCD_Index].EN_PINx, 0);
	LCD_Data(LCD_Index, 0x0);
	Delay_us(LCD[LCD_Index].TIMx,150000);
	LCD_Send_Command(LCD_Index, 0x3); //D4-7: 0011
	Delay_us(LCD[LCD_Index].TIMx,5000);
	LCD_Send_Command(LCD_Index, 0x3); //D4-7: 0011
	Delay_us(LCD[LCD_Index].TIMx,5000);
	LCD_Send_Command(LCD_Index, 0x3); //D4-7: 0011
	Delay_us(LCD[LCD_Index].TIMx,150);
	LCD_Send_Command(LCD_Index, (disp_cfg1 >> 4U));
	LCD_Send_Command(LCD_Index, (disp_cfg1 >> 4U)); // Controls mode selection
	LCD_Send_Command(LCD_Index, (disp_cfg1));       // Controls font size and disp lines 
	LCD_Send_Command(LCD_Index, 0x0);
	LCD_Send_Command(LCD_Index, (disp_cfg2)); 			// Controls cursor pos disp, cursor blink, and disp on/off
	LCD_Send_Command(LCD_Index, 0x0);
	LCD_Send_Command(LCD_Index, 0x6);								// Entry mode set lower 4
	LCD_Send_Command(LCD_Index, 0x0);
	LCD_Send_Command(LCD_Index, 0x1);
	LCD_Clear(LCD_Index);

}

void LCD_Clear(uint8_t LCD_Index) {

	if(LCD[LCD_Index].Mode) {
		LCD_Send_Command(LCD_Index, CLEAR_INSTR);
	} else {
		LCD_Send_Command(LCD_Index, (CLEAR_INSTR >> 4U));
		LCD_Send_Command(LCD_Index, CLEAR_INSTR);
	}
	Delay_us(LCD[LCD_Index].TIMx, 2000);

}

void LCD_Reset(uint8_t LCD_Index) {

	if(LCD[LCD_Index].Mode) {
		LCD_Send_Command(LCD_Index, RESET_INSTR);
	} else {
		LCD_Send_Command(LCD_Index, (RESET_INSTR >> 4U));
		LCD_Send_Command(LCD_Index, RESET_INSTR);
	}

}


void LCD_String_Write(uint8_t LCD_Index, char* msg) {
	unsigned i = 0;
	while(msg[i] != '\0'){
		LCD_Char_Write(LCD_Index, msg[i]);
		++i;
	}
}

void LCD_Char_Write(uint8_t LCD_Index, char data) {

	if(LCD[LCD_Index].Mode) {
		LCD_Data(LCD_Index, data);
	} else {
		GPIO_Write(LCD[LCD_Index].RS_PORTx, LCD[LCD_Index].RS_PINx, 1);
		uint8_t upper,lower;
		upper = ((data & 0xF0) >> 4U);
		lower = data & 0x0F;
		
		// Send upper 4 bits
		LCD_Data(LCD[LCD_Index].Index, upper);
		GPIO_Write(LCD[LCD_Index].EN_PORTx, LCD[LCD_Index].EN_PINx, 0);
		Delay_us(LCD[LCD_Index].TIMx, 5);
		GPIO_Write(LCD[LCD_Index].EN_PORTx, LCD[LCD_Index].EN_PINx, 1);
		Delay_us(LCD[LCD_Index].TIMx, 5);
		GPIO_Write(LCD[LCD_Index].EN_PORTx, LCD[LCD_Index].EN_PINx, 0);
		Delay_us(LCD[LCD_Index].TIMx, 100);

		// Send lower 4 bits
		LCD_Data(LCD[LCD_Index].Index, lower);
	}
		GPIO_Write(LCD[LCD_Index].EN_PORTx, LCD[LCD_Index].EN_PINx, 0);
		Delay_us(LCD[LCD_Index].TIMx, 5);
		GPIO_Write(LCD[LCD_Index].EN_PORTx, LCD[LCD_Index].EN_PINx, 1);
		Delay_us(LCD[LCD_Index].TIMx, 5);
		GPIO_Write(LCD[LCD_Index].EN_PORTx, LCD[LCD_Index].EN_PINx, 0);
		Delay_us(LCD[LCD_Index].TIMx, 100);
}

void LCD_Set_Cursor(uint8_t LCD_Index,uint8_t row ,uint8_t col) {
	
	if((row > 2) || (col > 16)) return;

		if(row == 1) {
			uint8_t pos = 0x80U + col - 0x1U;
			if(LCD[LCD_Index].Mode) {
				LCD_Send_Command(LCD_Index, (pos));
			}
			else {
			LCD_Send_Command(LCD_Index, (pos >> 4U));
			LCD_Send_Command(LCD_Index, (pos));
		}
	}
		else if (row == 2){
			uint8_t pos = 0xC0U + col - 0x1U;
			if(LCD[LCD_Index].Mode) {
				LCD_Send_Command(LCD_Index, (pos));	
			}
			else {
				LCD_Send_Command(LCD_Index, (pos >> 4U));
				LCD_Send_Command(LCD_Index, (pos));	
			}
		}
}

void LCD_Left_Shift(uint8_t LCD_Index) {

	if(LCD[LCD_Index].Mode) {
		LCD_Send_Command(LCD_Index, LSH_INSTR);
	}
	else {
		LCD_Send_Command(LCD_Index, (LSH_INSTR >> 4U));
		LCD_Send_Command(LCD_Index, LSH_INSTR);
	}
}

void LCD_Right_Shift(uint8_t LCD_Index) {

	if(LCD[LCD_Index].Mode) {
		LCD_Send_Command(LCD_Index, RSH_INSTR);
	} 
	else {
		LCD_Send_Command(LCD_Index, (RSH_INSTR >> 4U));
		LCD_Send_Command(LCD_Index, RSH_INSTR);
	}
}





