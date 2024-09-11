#ifndef __UART_H
#define __UART_H
#include "stm32f401xc.h"

void Uart_Config(void);
void UART_SendChar(uint8_t c);
void UART_SendString(char *string);
uint8_t UART_GetChar(void);


#endif 