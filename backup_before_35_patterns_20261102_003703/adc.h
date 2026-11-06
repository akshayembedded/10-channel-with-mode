void adc_init(void);
@far @interrupt void ADC_IRQHandler(void);
int map_value(long int input, int in_min, int in_max, int out_min, int out_max) ;