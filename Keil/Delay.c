#include "Delay.h"


void Delay_us (uint16_t us)
{
	/********* STEPS TO FOLLOW *********
	1. RESET the Counter
	2. Wait for the Counter to reach the entered value. As each count
	will take 1 us, the total waiting time will be the requiared
	us delay
	************************************/
	
	TIM10->CNT = 0; 
	while (TIM10->CNT < us);
}

void Delay_ms (uint16_t ms)
{
	for (uint16_t i=0; i<ms; i++)
	{
		Delay_us (1000); // delay of 1 ms
	}
}

void TIM10Config (void)
{
	/***************** STEPS TO FOLLOW *********************
	1. Enable Timer clock
	2. Set the prescalar and the ARR
	3. Enable the Timer, and wait for the update Flag to set
	********************************************************/
	
	
	// 1. Enable Timer clock
	RCC->APB2ENR |= (1<<17);
	
	
	// 2. Set the prescalar and the ARR
	TIM10->PSC = 84 - 1;
	
	
	// 3. Enable the Timer, and wait for the update Flag to set
	TIM10->CR1 |= (1<<0);
	while (!(TIM10->SR & (1<<0)));
}