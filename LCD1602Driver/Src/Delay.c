#include "../Inc/Delay.h"

/* Initializes the selected timer for us delay.
 * Make sure to enable peripheral clock (RCC) for the selected 32-bit timer
*/
void TIMx_InitDelay(TIM_TypeDef * TIMx) {

	// Set ARR to max value
	TIMx->ARR = 0xFFFFFFFEUL;

	// Set PSC to tick system clock every 1us
	TIMx->PSC = ((PCLK1 == SystemCoreClock ? PCLK1 : PCLK1 * 2U) / 1000000U) - 1U;

	// Update timer registers
	TIMx->EGR |= TIM_EGR_UG;

	// Enable the timer
	TIMx->CR1 |= TIM_CR1_CEN;

}


void Delay_us(TIM_TypeDef * TIMx, uint32_t delay){

	TIMx->CNT = 0;
	while(delay >= TIMx->CNT){
		__ASM("NOP");
	}
}
