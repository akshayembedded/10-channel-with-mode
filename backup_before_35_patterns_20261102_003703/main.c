/* MAIN.C file
 * LED C3 C4 C5 C6 C7 A1 A2 A3 D3 D2
 * SWITCH D4
 * POT D6
 * UART D5
 * 
 * 
 */


#include "STM8S003F3P.h"
#include<stdio.h>
#include <stdlib.h>
#include "adc.h"

// --- EEPROM Functions ---
void eeprom_unlock(void);
void eeprom_lock(void);
void eeprom_write_byte(unsigned int address, unsigned char data);
unsigned char eeprom_read_byte(unsigned int address);

// --- EEPROM Definitions ---
#define EEPROM_START_ADDR 0x4000
#define EEPROM_MODE_ADDR (EEPROM_START_ADDR + 0) // Store mode at the first byte


char led_pins[10] = {3, 4, 5, 6, 7,1, 2, 3,  2, 3};  // A1, A2, A3, C3, C4, C5, C6, C7
	 char i;
    volatile char *ports[10] = {&PC_ODR, &PC_ODR, &PC_ODR, &PC_ODR,&PC_ODR,&PA_ODR, &PA_ODR, &PA_ODR,  &PD_ODR, &PD_ODR};
		int flag=0;
		
void myDelay(int l);
void myDelay1(int l);
void mode_fw_fill(void);
void mode_fw_chaser(void);
void mode_bw_chaser(void);
void mode_fw_odd_even_strobe(void);
void mode_fw_two_chaser(void);
void mode_fw_chain(void);
void mode_bw_strobe_chaser(void);
void mode_both_running(void);
void mode_inward_mirror_fill(void);
void mode_outward_mirror_fill(void);
void mode_no_strobe_blink(void);
void mode_both_strobe_two_chaser(void);
void mode_both_strobe_three_chaser(void);
void mode_both_strobe_mirror_chaser(void);
void mode_inward_strobe_mirror_chaser(void);
void mode_both_strobe_split_chaser(void);
void mode_both_strobe_chaser(void);
void mode_no_strobe_odd_even(void);
void mode_fw_strobe_strobefill(void);
void mode_both_fill(void);
void mode_no_strobe_odd_blink_even_once(void);
void mode_bw_chain(void);
void mode_no_strobe_even_blink_odd_once(void);
void mode_both_chaser_strobe_complement(void);
void mode_both_chaser_complement(void);
void allon(void);

void alloff(void);
void mode_test(void);
extern int adc,mapped_value;

#define maxmode 1
#define max mapped_value0
#define min 10
#define channel 10
int mode=0,curmode=0;

void ledon(int i)
{
	*ports[i] |= (1 << led_pins[i]);
}
void ledoff(int i)
{
	*ports[i] &= ~(1 << led_pins[i]);
}
void alloff()
{
	int i=0;
	for(i=0;i<channel;i++)
	{
            ledoff(i); // Turn ON LED
						
					
	}
}

void allon()
{
	int i=0;
	for(i=0;i<channel;i++)
	{
            ledon(i); // Turn ON LED
						
					
	}
}
	
@far @interrupt void EXTI4_IRQHandler(void) {
	int i;
	EXTI_CR1=0x0;
    // Handle the interrupt (toggle an LED, for example)
    mode++;  // Toggle PC3
		
		myDelay1(1000);
		
	for(i=0;i<channel;i++)
	{
            *ports[i] &= ~(1 << led_pins[i]);  // Turn ON LED
						
  }

	if(mode==maxmode)	mode=-1;
  EXTI_CR1=0x80;  // Clear the interrupt flag (optional for STM8)
}

main()
{
	int i,j;
   /* // PB.5 as Output 0111 1100  7a D3 to d6
    
		*/
		#asm
        sim
        #endasm
		PD_DDR =0x20;//D5 Transmit(OUT) D6 AIN6(IN) 
		PD_CR1 =0x00;// PB.5 as Push Pull Type Output
		PD_CR2 =0x20;
		
		
		I2C_CR1=0;
		PD_DDR|=(3<<2);
		PD_CR2|=(3<<2);
		PD_CR1|=(3<<2);
	    PD_ODR|=(3<<2);

		PC_DDR=0XFC;//C3 TI C7
		PC_CR2=0Xfc;
		PC_CR1=0XFC;
		
		PA_DDR=0X0E;
		PA_CR2=0X0e;
		PA_CR1=0X0E;

		
        alloff();
		adc_init();

        mode = (int)eeprom_read_byte(EEPROM_MODE_ADDR);

    // Validate the read mode. If out of range, default to auto-cycle mode.
    if (mode >=maxmode  || mode < -1) {
        mode = 0;
    }
    curmode = mode; // Sync current mode
		eeprom_write_byte(EEPROM_MODE_ADDR, (unsigned char)(mode+1));
			
		#asm
        rim
        #endasm
	//	mode_test();

	while(1)  
    {
		
		
			if(mode==0)
			{
									for(i=0;i<3;i++) mode_fw_fill();
                 for(i=0;i<3;i++) mode_fw_chaser();
							
                 for(i=0;i<3;i++) mode_bw_chaser();
                for(i=0;i<3;i++) mode_fw_odd_even_strobe();
                 for(i=0;i<3;i++) mode_fw_two_chaser();
                 for(i=0;i<3;i++) mode_fw_chain();
								 for(i=0;i<3;i++) mode_bw_chain();
                 for(i=0;i<3;i++) mode_bw_strobe_chaser();
                 for(i=0;i<3;i++) mode_both_running();
                 for(i=0;i<3;i++) mode_inward_mirror_fill();
								 for(i=0;i<3;i++) mode_outward_mirror_fill();
                 for(i=0;i<20;i++) mode_no_strobe_blink();
                 for(i=0;i<3;i++) mode_both_strobe_two_chaser();
								 for(i=0;i<3;i++) mode_both_chaser_complement();
								 
                
                 for(i=0;i<3;i++) mode_both_strobe_mirror_chaser();
                 for(i=0;i<3;i++) mode_inward_strobe_mirror_chaser();
                 for(i=0;i<3;i++) mode_both_strobe_split_chaser();
                 for(i=0;i<3;i++) mode_both_strobe_chaser();
								  for(i=0;i<3;i++) mode_both_strobe_three_chaser();
                 for(i=0;i<3;i++) mode_no_strobe_odd_even();
								 for(i=0;i<3;i++) mode_both_chaser_strobe_complement();
						
                for(i=0;i<3;i++) mode_fw_strobe_strobefill();
                for(i=0;i<3;i++) mode_both_fill();
                for(i=0;i<3;i++) mode_no_strobe_odd_blink_even_once();
								for(i=0;i<3;i++) mode_no_strobe_even_blink_odd_once();
			}
	
			
		if(mode==1)
		{
		allon();
		curmode=mode;
		}
	
		
		
	}
		
		
	
    
}

void myDelay(int l)
{
       int i,j;
		 ADC_CR1 = 1;
       for(i=0;i<l;i++)
       {
            for(j=0;j<100;)
			{
			   j++;
			}
        }
}

void myDelay1(int l)
{
       int i,j;
//		 ADC_CR1 = 1;
       for(i=0;i<l;i++)
       {
            for(j=0;j<100;)
			{
			   j++;
			}
        }
}

void mode_fw_fill(void)
{
	int i;
	for(i=0;i<channel;i++)
	{
    ledon(i);  // Turn ON LED
		myDelay(mapped_value);
	}
	
	for(i=0;i<channel;i++)
	{
    ledoff(i) ; // Turn ON LED
		myDelay(mapped_value);
	}
	
		alloff();
}
void mode_fw_chaser(void)
{
	int i;
	for(i=0;i<channel;i++)
	{
							
            ledon(i);  // Turn ON LED
						
						myDelay(mapped_value);
						ledoff(i);
			
	}
	alloff();	
}

void mode_both_chaser_complement(void)
{
	int i;

    // Turn all LEDs ON first
    for(i = 0; i < channel; i++)
    {
        ledon(i);
    }

    for(i = 0; i < channel; i++)
    {
        ledoff(i);              // Moving OFF LED

        myDelay(mapped_value);

        ledon(i);               // Restore LED
    }
		
		for(i = channel-2; i >= 0; i--)
    {
        ledoff(i);              // Moving OFF LED

        myDelay(mapped_value);

        ledon(i);               // Restore LED
    }
		

    alloff();
}

void mode_both_chaser_strobe_complement(void)
{
	int i, k;

    // All LEDs ON initially
    for(i = 0; i < channel; i++)
    {
        ledon(i);
    }

    // Forward
    for(i = 0; i < channel; i++)
    {
        for(k = 0; k < 5; k++)
        {
            ledon(i);

            myDelay(100);

            ledoff(i);

            myDelay(mapped_value);
        }

        ledon(i); // Leave ON
    }

    // Reverse (skip last LED for smooth ping-pong)
    for(i = channel - 2; i >= 0; i--)
    {
        for(k = 0; k < 5; k++)
        {
            ledon(i);

            myDelay(100);

            ledoff(i);

            myDelay(mapped_value);
        }

        ledon(i); // Leave ON
    }

    alloff();
}

void mode_bw_chaser(void)
{
	int i;
	for(i=0;i<channel;i++)
	{
            ledon(channel-1-i);  // Turn ON LED
						myDelay(mapped_value);
						ledoff(channel-1-i);
  }

}


void mode_fw_odd_even_strobe(void)
{
	int i,j,k;

	for(i=0;i<channel;i++)
	{
		for(k=0;k<10;k++)
		{
		if(i%2)
		{
            ledon(i);  // Turn ON LED
							myDelay(100);
							 ledoff(i);  // Turn ON LED
							 myDelay(mapped_value);
		}
	
}
}
	for(i=0;i<channel;i++)
	{
				for(k=0;k<10;k++)
		{
		if(i%2==0)
		{
	
		ledon(i);  // Turn ON LED
							myDelay(100);
							 ledoff(i);  // Turn ON LED
							 myDelay(mapped_value);
	}
	
}
}
	


alloff();

}

void mode_fw_two_chaser(void)
{
	int i,j;
	 for(i=0;i<channel;i++)
	 {
		 if(i!=0 && i!= 1)
		 {
			  ledoff(i-2);
		 }
		 ledon(i);
	 myDelay(mapped_value);
	 }
	  ledoff(channel-2); //4
	 myDelay(mapped_value);
	  ledoff(channel-1);//3
	 myDelay(mapped_value);
	 
	
	 alloff();
	 
}

void mode_fw_chain(void)
{
int i=0,j,k;
for(i=0;i<channel;i++)
{
	if(i!=0)
	{
		ledoff(i-1);
	}
	for(k=0;k<2;k++)
	{
	ledon(i);
	
	 myDelay(100);
	 ledoff(i);
	
	 myDelay(mapped_value);
 }
	ledon(i);
}
for(j=0;j<channel-1;j++)
{
for(i=0;i<channel-j;i++)
{
	
		for(k=0;k<2;k++)
		{
	ledon(i);
	 myDelay(100);
	 
	 if(i==(channel-2-j))
	 {
		 break;
	 }
	ledoff(i);
myDelay(mapped_value);
}
 if(i==(channel-2-j))
	 {
		 break;
	 }
}
}
alloff();
}

void mode_bw_strobe_chaser(void)
{
int i,j,k;
for(i=0;i<channel;i++)
{
	
	for(k=0;k<2;k++)
	{	
	ledon(i);
	myDelay(100);
	
	ledoff(i);
 	myDelay(mapped_value);
	}

}
for(i=channel-1;i>0;i--)
{
	
		
	for(k=0;k<2;k++)
	{	
	ledon(i);
	myDelay(100);
	
	 

	ledoff(i);
myDelay(mapped_value);
}

}

alloff();

}


void mode_both_running(void)
{
	int i=0,j=0;

	for(i=0;i<channel;i++)
	{
		ledon(i);
	
	 myDelay(mapped_value);

}
		for(i=channel-1;i>=0;i--)
	{
		ledoff(i);
	
	 myDelay(mapped_value);

}

alloff();
}

void mode_inward_mirror_fill(void)
{
		
		int j=0,i=0;

	for(i=0;i<channel/2;i++)
	{
		
		ledon(i);
    ledon(channel-1-i);
    myDelay(40);

    ledoff(i);
    ledoff(channel-1-i);
		myDelay(mapped_value);
		
		ledon(i);
		ledon(channel-1-i);
		
	}
	for(i=channel-1;i>=channel/2;i--)
	{
	
		ledoff(i);
		ledoff(channel-1-i);
		myDelay(mapped_value);
	}


alloff();
}

void mode_outward_mirror_fill(void)
{
    int i, j;


        // Fill from center outward
        for(i = channel/2 - 1; i >= 0; i--)
        {
            ledon(i);
            ledon(channel - 1 - i);

            myDelay(mapped_value);
        }

        // Unfill from ends inward
        for(i = 0; i < channel/2; i++)
        {
            ledoff(i);
            ledoff(channel - 1 - i);

            myDelay(mapped_value);
        }
    

    alloff();
}
void mode_no_strobe_blink(void)
{
    int i;
    int j;
   
  
        for (i = 0; i < channel; i++)
        {
            ledon(i);
        }
        myDelay(100);
        for (i = 0; i < channel; i++)
        {
            ledoff(i);
        }
        myDelay(mapped_value);
   
    alloff();
}

void mode_both_strobe_two_chaser(void)
{
    int i;

    // Forward
    for(i = 0; i < channel; i++)
    {
        if(i > 0)
            ledon(i - 1);

        ledon(i);

        myDelay(100);          // ON time

        if(i > 0)
            ledoff(i - 1);

        ledoff(i);

        myDelay(mapped_value); // OFF time
    }

    // Reverse
    for(i = channel - 1; i >= 0; i--)
    {
        if(i < channel - 1)
            ledon(i + 1);

        ledon(i);

        myDelay(100);          // ON time

        if(i < channel - 1)
            ledoff(i + 1);

        ledoff(i);

        myDelay(mapped_value); // OFF time
    }

    alloff();

}
void mode_both_strobe_three_chaser(void)
{
   int i;

    // Forward
    for(i = 0; i < channel; i++)
    {
        if(i >= 2) ledon(i - 2);
        if(i >= 1) ledon(i - 1);
        ledon(i);

        myDelay(100);              // ON time

        if(i >= 2) ledoff(i - 2);
        if(i >= 1) ledoff(i - 1);
        ledoff(i);

        myDelay(mapped_value);     // OFF time
    }

    // Reverse
    for(i = channel - 1; i >= 0; i--)
    {
        if(i <= channel - 3) ledon(i + 2);
        if(i <= channel - 2) ledon(i + 1);
        ledon(i);

        myDelay(100);              // ON time

        if(i <= channel - 3) ledoff(i + 2);
        if(i <= channel - 2) ledoff(i + 1);
        ledoff(i);

        myDelay(mapped_value);     // OFF time
    }

    alloff();
}

void mode_both_strobe_mirror_chaser(void)
{
       int i;

    // Inward
    for(i = 0; i < channel/2; i++)
    {
        ledon(i);
        ledon(channel-1-i);

        myDelay(100);

        ledoff(i);
        ledoff(channel-1-i);

        myDelay(mapped_value);
    }

    // Outward
    for(i = channel/2-1; i >= 0; i--)
    {
        ledon(i);
        ledon(channel-1-i);

        myDelay(100);

        ledoff(i);
        ledoff(channel-1-i);

        myDelay(mapped_value);
    }

    alloff();
}


void mode_inward_strobe_mirror_chaser()
{
  int i, j;


        for(i = 0; i < channel / 2; i++)
        {
            ledon(i);
            ledon(channel - 1 - i);

            myDelay(100);              // ON time

            ledoff(i);
            ledoff(channel - 1 - i);

            myDelay(mapped_value);     // OFF time
        }
    

    alloff();
	}

// Pattern 14 (formerly blink_15): LEDs pair from ends inward, then outward, repeated twice
void mode_both_strobe_split_chaser(void)
{
   int i;

    // Inward
    for(i = 0; i < channel/2; i++)
    {
        ledon(i);
        ledon(i + channel/2);

        myDelay(100);

        ledoff(i);
        ledoff(i + channel/2);

        myDelay(mapped_value);
    }

    // Outward
    for(i = channel/2 - 1; i >= 0; i--)
    {
        ledon(i);
        ledon(i + channel/2);

        myDelay(100);

        ledoff(i);
        ledoff(i + channel/2);

        myDelay(mapped_value);
    }

    alloff();
}

// Pattern 15: Randomized strobe effect with varying LED subsets
void mode_both_strobe_chaser(void)
{
 int i,k;

    // Forward
    for(i = 0; i < channel; i++)
    {
			for(k=0;k<3;k++)
			{
        ledon(i);

        myDelay(100);          // Strobe ON

        ledoff(i);

        myDelay(mapped_value); // Strobe OFF
			}
    }

    // Reverse (skip last LED)
    for(i = channel - 2; i >= 0; i--)
    {
			for(k=0;k<3;k++)
			{
        ledon(i);

        myDelay(100);          // Strobe ON

        ledoff(i);

        myDelay(mapped_value); // Strobe OFF
			}
    }

    alloff();
}

void mode_no_strobe_odd_even(void)
{
	int i,j,k;

		for(k=0;k<8;k++)
		{
	for (i = 0; i < channel; i+=2)
        {
					
            ledon(i);
						
					
						
        }
				myDelay(100);
       
			
				alloff();
				 myDelay(mapped_value);
			}
				for(k=0;k<8;k++)
		{
	for (i = 1; i < channel; i+=2)
        {
            ledon(i);
						
        }
				myDelay(100);
						
       
			
			
				alloff();
				 myDelay(mapped_value );
			}
					
}

void mode_fw_strobe_strobefill(void) //flicker individual LEDs 
{
     int i, s;

    alloff();

    for(i = 0; i < channel; i++)
    {
        // Previous LEDs remain ON
        for(s = 0; s < 5; s++)   // Number of strobes
        {
            ledon(i);            // New LED ON

            myDelay(100);

            ledoff(i);           // New LED OFF

            myDelay(mapped_value);
        }

        // Leave new LED ON permanently
        ledon(i);
				myDelay(mapped_value);
    }

    

    alloff();
}

void mode_both_fill(void)
{
    int i;

    // Fill
    for(i = 0; i < channel; i++)
    {
        ledon(i);
        myDelay(mapped_value);
    }

    // Reverse fill (remove from end)
    for(i = channel - 1; i > 0; i--)
    {
        ledoff(i);
        myDelay(mapped_value);
    }

    alloff();
}

void mode_no_strobe_even_blink_odd_once(void)
{
       int i, k;
    

    // Left half strobes 10 times
    for(k = 0; k < 10; k++)
    {
        for(i = 0; i < channel; i++)
			
      if(i%2==0)      ledon(i);

        myDelay(100);

        alloff();

        myDelay(mapped_value);
    }

    // Right half strobes once
    for(i = 0; i < channel; i++)
       if(i%2) ledon(i);

    myDelay(100);

    alloff();

    myDelay(mapped_value);
}

void mode_no_strobe_odd_blink_even_once(void)
{
       int i, k;
    

    // Left half strobes 10 times
    for(k = 0; k < 10; k++)
    {
        for(i = 0; i < channel; i++)
			
      if(i%2)      ledon(i);

        myDelay(100);

        alloff();

        myDelay(mapped_value);
    }

    // Right half strobes once
    for(i = 0; i < channel; i++)
       if(i%2==0) ledon(i);

    myDelay(100);

    alloff();

    myDelay(mapped_value);
}
void mode_bw_chain(void)
{
    int i = 0, j, k;

    /* Forward section reversed */
    for(i = channel - 1; i >= 0; i--)
    {
        if(i != (channel - 1))
        {
            ledoff(i + 1);
        }

        for(k = 0; k < 2; k++)
        {
            ledon(i);

            myDelay(100);
            ledoff(i);

            myDelay(mapped_value);
        }

        ledon(i);
    }

    /* Tail shrinking in reverse direction */
    for(j = 0; j < channel - 1; j++)
    {
        for(i = channel - 1; i >= j; i--)
        {
            for(k = 0; k < 2; k++)
            {
                ledon(i);

                myDelay(100);

                if(i == (j + 1))
                {
                    break;
                }

                ledoff(i);
                myDelay(mapped_value);
            }

            if(i == (j + 1))
            {
                break;
            }
        }
    }

    alloff();
}

// --- EEPROM Functions ---

void eeprom_unlock(void) {
    if (FLASH_IAPSR & (1 << 3))
        return;
    FLASH_DUKR = 0xAE;
    FLASH_DUKR = 0x56;
    while (!(FLASH_IAPSR & (1 << 3)))
        ;
}

void eeprom_lock(void) {
    FLASH_IAPSR &= ~(1 << 3);
}

void eeprom_write_byte(unsigned int address, unsigned char data) {
    if (address >= EEPROM_START_ADDR && address < (EEPROM_START_ADDR + 128)) {
        eeprom_unlock();
        *(volatile unsigned char *)address = data;
        while (!(FLASH_IAPSR & (1 << 2)))
            ;
        eeprom_lock();
    }
}

unsigned char eeprom_read_byte(unsigned int address) {
    if (address >= EEPROM_START_ADDR && address < (EEPROM_START_ADDR + 128))
        return *(volatile unsigned char *)address;
    return 0xFF;
}
