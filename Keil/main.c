#include "ClockConfig.h"
#include "Delay.h"
#include "UART.h"
#include "RingBuffer.h"

#define RINGBUF_SIZE (128)
volatile char rb_buf[ RINGBUF_SIZE + 1 ];
ringbuf rb = {
  len: RINGBUF_SIZE,
  buf: rb_buf,
  pos: 0,
  ext: 0
};
volatile int newline = 0;

void Interrupt_Config(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // 1<<14; Enable SYSCNFG Clock
	SYSCFG->EXTICR[0] &= ~(0xfu<<0); // Bits[3:2:1:0] = (0:0:0:0) 
	SYSCFG->EXTICR[1] &= ~(0xfu<<0);
	EXTI->IMR |= (1<<0); // Disable Mask on EXTI0
	EXTI->RTSR |= (0x1F<<0); // Enable Rising edge Trigger for PA0-PA4
	EXTI->FTSR &= ~(0x1Fu<<0); // Disable Falling edge Trigger for PA0-PA4
	NVIC_SetPriority (EXTI0_IRQn, 0);
	NVIC_SetPriority (EXTI1_IRQn, 0);
	NVIC_SetPriority (EXTI2_IRQn, 0);
	NVIC_SetPriority (EXTI3_IRQn, 0);
	NVIC_SetPriority (EXTI4_IRQn, 0);
	NVIC_EnableIRQ (EXTI0_IRQn);
	NVIC_EnableIRQ (EXTI1_IRQn);
	NVIC_EnableIRQ (EXTI2_IRQn);
	NVIC_EnableIRQ (EXTI3_IRQn);
	NVIC_EnableIRQ (EXTI4_IRQn);
}



void GPIOA_Config(){
	GPIOA->MODER &= ~((3u<<0) | (3u<<2) | (3u<<4) | (3u<<6) | (3u<<8)); /* PA0-PA3 inputs from voltage battaries
	PA4 input from load current */
	
	GPIOA->MODER |= (1<<10); // PA5 load connection output
}	

void USART1_IRQn_handler(void){


	if (USART1->SR & USART_SR_RXNE) {
		char c = USART1->DR;
    ringbuf_write(rb, c);
		if (c == '\r') {newline = 1;}
    }

}



enum States{
	ST_IDLE, // Normal work
	ST_PROB, // Load off
};

enum States state = ST_IDLE;

struct Status_t{
	uint8_t V0_Stat;
	uint8_t V1_Stat;
	uint8_t V2_Stat;
	uint8_t V3_Stat;
	uint8_t Load_Stat;
} Status; 

void EXTI0_IRQHandler(void){
	Status.V0_Stat = 1;
	state = ST_PROB;
}
void EXTI1_IRQHandler(void){
	Status.V1_Stat = 1;
	state = ST_PROB;
}
void EXTI2_IRQHandler(void){
	Status.V2_Stat = 1;
	state = ST_PROB;
}
void EXTI3_IRQHandler(void){
	Status.V3_Stat = 1;
	state = ST_PROB;
}
void EXTI4_IRQHandler(void){
	Status.Load_Stat = 1;
	state = ST_PROB;
}




int main()
{
	SysClockConfig();
	TIM10Config();
	GPIOA_Config();
	Interrupt_Config();
	Uart_Config();
	
	while(1)
	{
		switch(state){
			case ST_IDLE:													
				GPIOA->BSRR |= (1<<5); // load on
					__WFI(); // wait for interrupt
				break;
			case ST_PROB:
				GPIOA->BSRR |= (1<<5)<<16; // load off
				Delay_ms(10000);
				state = ST_IDLE;
				break;
		}

	}
}