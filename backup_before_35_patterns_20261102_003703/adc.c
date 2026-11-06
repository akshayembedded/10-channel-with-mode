#include "STM8S103F3P.h"
#include "adc.h"
int adc,mapped_value;
extern int mode;
int map_value(long int input, int in_min, int in_max, int out_min, int out_max) {
    return ((input - in_min) * (out_max - out_min)) / (in_max - in_min) + out_min;
}


@far @interrupt void ADC_IRQHandler(void) {
    // Handle the interrupt (toggle an LED, for example)
    char str[20];
		
	
		ADC_CSR = 6|(1<<5);
		adc=((ADC_DRH<<8)|ADC_DRL);
		mapped_value = map_value(adc, 0, 1023, 10, 1000);

	//mapped_value+=10;
	//	sprintf(str,"ADC %4d %4d\n\r",mode,mapped_value);
	//	uart_String(str);

    // Clear the interrupt flag (optional for STM8)
}

void adc_init()
{

		PD_DDR &=~(1<<6);
		ADC_TDRH=0x00;
		ADC_TDRL=0x00;
		
		ADC_CSR =(6|(1<<5));
		ADC_CR1 = 1;
		ADC_CR2 = 1<<3;
		ADC_CR3 = 0;
}