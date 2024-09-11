#include "UART.h"

void Uart_Config(void)
{	
	RCC->APB2ENR |= (1<<4); // UART Clock enable
	
	
	RCC->AHB1ENR |= (1<<0); // Enable GPIOA Clock
	
	GPIOA->MODER |= (2<<10*2) |  (2<<9*2); // AF mode for PA9 & PA10
	
	GPIOA->AFR[1] |= (7<<8) | (7<<4); // Configure AF7 for PA9 & PA10 
	
	GPIOA->OSPEEDR |= (3<<10*2) |  (3<<9*2);
	
	USART1->CR1 |= (USART_CR1_RE | USART_CR1_TE | USART_CR1_RXNEIE);
	
	USART1->CR1 |= (1<<13); // UART enable
	
	USART1->CR1 &= ~(1u<<12); // 8 data length
	
	USART1->CR2 &= ~(3u<<12); // 1 stop bit
	
	USART1->BRR = 0x2d9;
	
	USART1->CR1 |= (3<<2); // Enable RX & TX
	
	NVIC_SetPriority (USART1_IRQn, 1);
	
	NVIC_EnableIRQ(USART1_IRQn);
	
}

void UART_SendChar(uint8_t c)
{
	USART1->DR = c; // load the data into DR register
	while (!(USART1->SR & (1<<6)));
	
}

void UART_SendString(char *string)
{
	while (*string) UART_SendChar (*string++);
}



uint8_t UART_GetChar(void)
{
	uint8_t temp;
	
	while (!(USART1->SR & (1<<5))); // wait for RXNE bit to set
	temp = (uint8_t)USART1->DR; // Read the data. This clears the RXNE also
	return temp;
}