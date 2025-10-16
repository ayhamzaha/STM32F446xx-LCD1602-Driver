#ifndef INC_DELAY_H_
#define INC_DELAY_H_

#include "stm32f446xx.h"
#include "system_stm32f4xx.h"
#include <stdint.h>

#define PCLK1	(SystemCoreClock >> APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos])

void TIMx_InitDelay(TIM_TypeDef *);
void Delay_us(TIM_TypeDef *, uint32_t);


#endif /* INC_DELAY_H_ */
