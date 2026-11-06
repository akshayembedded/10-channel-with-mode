#ifndef ADC_H
#define ADC_H
#include "config.h"
void adc_init(void);
void control_timer_init(void);
void adc_request(void);
int adc_speed_read(void);
unsigned char button_take_press(void);
@far @interrupt void ADC_IRQHandler(void);
@far @interrupt void TIM4_UPD_OVF_IRQHandler(void);
@far @interrupt void EXTI_PORTD_IRQHandler(void);
#endif
