#ifndef __DELAY_H
#define __DELAY_H
#include "stm32f401xc.h"

void Delay_us (uint16_t us);
void Delay_ms (uint16_t ms);
void TIM10Config (void);
#endif 