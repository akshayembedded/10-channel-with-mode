#include "STM8S003F3P.h"
#include "adc.h"
static volatile unsigned int adc_value = 512U;
static volatile unsigned char adc_busy = 0;
static volatile unsigned char button_event = 0;
static volatile unsigned char button_candidate = 0;
static unsigned char button_count = 0;
static unsigned char button_held = 0;

void adc_request(void)
{
    /* Foreground or TIM4 ISR; byte flag claimed with foreground IRQs masked. */
    if (!adc_busy) {
        adc_busy = 1;
        ADC_CR1 |= 0x01;
    }
}

void adc_init(void)
{
    volatile unsigned int settle;
    PD_DDR &= (unsigned char)~0x40;
    PD_CR1 &= (unsigned char)~0x40;
    PD_CR2 &= (unsigned char)~0x40;
    CLK_PCKENR2 |= 0x08;
    ADC_CR1 = 0;
    ADC_CR2 = 0x08;
    ADC_CR3 = 0;
    ADC_CSR = 0x26;
    ADC_TDRL |= 0x40;
    ADC_CR1 = 0x01;
    for (settle = 0; settle < 100U; settle++) { }
    adc_request();
}

void control_timer_init(void)
{
#if ENABLE_MODE_BUTTON
    PD_CR2 &= (unsigned char)~0x10;
    PD_DDR &= (unsigned char)~0x10;
    PD_CR1 |= 0x10; /* PD4 pull-up; button connects PD4 to ground. */
    EXTI_CR1 = (EXTI_CR1 & 0x3F) | 0x80; /* Port D falling edge. */
    button_candidate = 1; /* Also detect a button held at startup. */
    PD_CR2 |= 0x10;
#else
    PD_CR2 &= (unsigned char)~0x10;
#endif
#if ENABLE_TIMER_ADC || ENABLE_MODE_BUTTON
    CLK_PCKENR1 |= 0x10;
    TIM4_CR1 = 0;
    TIM4_IER = 0;
    TIM4_PSCR = 7;
    TIM4_ARR = 155; /* 9.984 ms at reset 2 MHz master clock. */
    TIM4_CNTR = 0;
    TIM4_EGR = 1;
    TIM4_SR = 0;
    TIM4_IER = 1;
    TIM4_CR1 = 1;
#endif
}

@far @interrupt void EXTI_PORTD_IRQHandler(void)
{
#if ENABLE_MODE_BUTTON
    button_candidate = 1;
#endif
}

@far @interrupt void TIM4_UPD_OVF_IRQHandler(void)
{
    TIM4_SR &= (unsigned char)~0x01;
#if ENABLE_TIMER_ADC
    adc_request();
#endif
#if ENABLE_MODE_BUTTON
    /* Three stable samples for press AND release; one event per hold. */
    if (button_candidate || button_held) {
        if ((PD_IDR & 0x10) == 0) {
            if (!button_held) {
                if (++button_count >= 3) {
                    button_event = 1;
                    button_held = 1;
                    button_count = 0;
                }
            } else button_count = 0;
        } else {
            if (button_held) {
                if (++button_count >= 3) {
                    button_held = 0;
                    button_candidate = 0;
                    button_count = 0;
                }
            } else {
                button_candidate = 0;
                button_count = 0;
            }
        }
    }
#endif
}

@far @interrupt void ADC_IRQHandler(void)
{
    unsigned char low;
    unsigned char high;
    if (ADC_CSR & 0x80) {
        low = ADC_DRL;
        high = ADC_DRH;
        adc_value = ((unsigned int)(high & 3) << 8) | low;
        ADC_CSR &= (unsigned char)~0x80;
        adc_busy = 0;
    }
}

/* Foreground only, called with interrupts enabled. */
int adc_speed_read(void)
{
    unsigned int value;
    #asm
        sim
    #endasm
    value = adc_value;
#if !ENABLE_TIMER_ADC
    adc_request(); /* Demand sampling when periodic ADC is disabled. */
#endif
    #asm
        rim
    #endasm
    return 10 + (int)(((unsigned long)value * 990UL) / 1023UL);
}

unsigned char button_take_press(void)
{
    unsigned char event;
    #asm
        sim
    #endasm
    event = button_event;
    button_event = 0;
    #asm
        rim
    #endasm
    return event;
}
