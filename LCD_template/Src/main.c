#include "stm32f446xx.h"
#include <stdio.h>
#include "../Inc/LCD.h"

#define LCD_1  0U

#define RS_PIN 5U  // Pin A5
#define EN_PIN 6U  // Pin A6
#define D4_PIN 7U  // Pin A7
#define D5_PIN 6U  // Pin B6
#define D6_PIN 7U  // Pin C7
#define D7_PIN 9U  // Pin A9
#define BTN 	 13U // Pin C13

void GPIO_Init(void); // Initializes GPIO pins as outputs for LCD control & btn for input

LCD_CFG LCD[MAX_LCDS];

int main(void){
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;
	
  // 4-bit mode setup
  LCD[LCD_1].RS_PINx = RS_PIN;
  LCD[LCD_1].RS_PORTx = GPIOA;

  LCD[LCD_1].EN_PINx = EN_PIN;
  LCD[LCD_1].EN_PORTx = GPIOA;

  LCD[LCD_1].D4_PINx = D4_PIN;
  LCD[LCD_1].D4_PORTx = GPIOA;

  LCD[LCD_1].D5_PINx = D5_PIN;
  LCD[LCD_1].D5_PORTx = GPIOB;

  LCD[LCD_1].D6_PINx = D6_PIN;
  LCD[LCD_1].D6_PORTx = GPIOC;

  LCD[LCD_1].D7_PINx = D7_PIN;
  LCD[LCD_1].D7_PORTx = GPIOA;

  LCD[LCD_1].TIMx = (TIM_TypeDef*)TIM2_BASE;
	
  LCD[LCD_1].Index = 0;
	
	GPIO_Init();
	LCD_Init(LCD_1, four_bit,two_line,small,screen_on,cursor_on,blink_off);
	
	uint16_t clicks = 0;
	char cnt[6];
	LCD_Char_Write(LCD_1,'0');
	while(1) {
		if(!(GPIOC->IDR & GPIO_IDR_ID13)){
			clicks++;
			snprintf(cnt,sizeof(cnt),"%u",clicks);
			LCD_Clear(LCD_1);
			LCD_String_Write(LCD_1, cnt);
			
			// Wait until button is released
			while(!(GPIOC->IDR & GPIO_IDR_ID13)) {
				__ASM("NOP");
			}
		}
	}
}

void GPIO_Init(void) {
	
	// Set LCD pins to output mode (01) and btn to input mode (00)
	GPIOA->MODER &= ~(GPIO_MODER_MODE5 | GPIO_MODER_MODE6 | GPIO_MODER_MODE7 | GPIO_MODER_MODE9);
	GPIOA->MODER |= (1U << (RS_PIN * 2U)) | (1U << (EN_PIN * 2U)) | (1U << (D4_PIN * 2U)) | (1U << (D7_PIN * 2U));
	
	GPIOB->MODER &= ~(GPIO_MODER_MODE6);
	GPIOB->MODER |= (1U << (D5_PIN * 2U));
	
	GPIOC->MODER &= ~(GPIO_MODER_MODE7 | GPIO_MODER_MODE13);
	GPIOC->MODER |= (1U << (D6_PIN * 2U)) | (0U << (BTN * 2U));

}
