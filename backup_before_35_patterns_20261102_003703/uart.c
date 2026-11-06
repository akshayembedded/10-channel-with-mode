#include "STM8S103F3P.h"
#include "uart.h"

void trans(char a)
{
		while((UART1_SR&0x80)==0);
		UART1_DR=a;
		while((UART1_SR&0x40)==0);
}
void uart_String(const char *a)
{
	while(*a)
	{
		trans(*a++);
	}
}

void uart_init(void)
{
		UART1_CR1=0;
		UART1_CR2=(1<<3);
		UART1_CR3=0;
		UART1_CR4=0;
		UART1_CR5=0;

		UART1_BRR1=0x0d;
		UART1_BRR2=0x00;
}
