/* STM8S003F3P: 10 active-HIGH outputs.
 * OUT1..10: PC3 PC4 PC5 PC6 PC7 PA1 PA2 PA3 PD2 PD3.
 * POT: PD6/AIN6. Optional active-low mode button: PD4.
 * No PWM. See config.h for control features.
 */


#include "STM8S003F3P.h"


#include "adc.h"

// --- EEPROM Functions ---


unsigned char eeprom_write_byte(unsigned int address, unsigned char data);
unsigned char eeprom_read_byte(unsigned int address);

// --- EEPROM Definitions ---
#define EEPROM_START_ADDR 0x4000
#define EEPROM_MODE_ADDR (EEPROM_START_ADDR + 0) // Store mode at the first byte


#define channel 10
unsigned char led_pins[channel] = {3,4,5,6,7,1,2,3,2,3};
volatile unsigned char *ports[channel] = {
    &PC_ODR,&PC_ODR,&PC_ODR,&PC_ODR,&PC_ODR,
    &PA_ODR,&PA_ODR,&PA_ODR,&PD_ODR,&PD_ODR
};
		
void myDelay(int l);
void myDelay1(int l);
void mode_forward_fill_forward_empty(void);
void mode_forward_single_led_chase(void);
void mode_reverse_single_led_chase(void);
void mode_forward_two_led_chase(void);
void mode_forward_strobe_chain_fill(void);
void mode_ping_pong_double_strobe_chase(void);
void mode_forward_fill_reverse_empty(void);
void mode_inward_mirror_strobe_fill(void);
void mode_outward_mirror_fill_inward_empty(void);
void mode_all_leds_strobe(void);
void mode_ping_pong_two_led_strobe_chase(void);
void mode_ping_pong_three_led_strobe_chase(void);
void mode_inward_outward_mirror_strobe_chase(void);
void mode_inward_outward_mirror_two_led_strobe_chase(void);
void mode_inward_mirror_strobe_chase(void);
void mode_outward_mirror_strobe_chase(void);
void mode_ping_pong_split_pair_strobe_chase(void);
void mode_ping_pong_triple_strobe_chase(void);
void mode_odd_eight_blinks_then_even_eight_blinks(void);
void mode_forward_three_strobe_fill(void);
void mode_split_odd_even_double_strobe_fill(void);
void mode_even_ten_blinks_then_odd_ten_blinks(void);
void mode_reverse_strobe_chain_fill(void);
void mode_ping_pong_dark_chase(void);
void allon(void);

void alloff(void);
void mode_comet_ping_pong(void);
void mode_twin_chase_cross_through(void);
void mode_center_out_strobe_fill(void);
void mode_dark_pair_mirror_chase(void);
void mode_two_ended_chase_and_stack(void);
void mode_expanding_contracting_band(void);
void mode_triple_strobe_burst_pause(void);
void mode_half_row_five_strobe_alternate(void);
void mode_half_row_ten_strobe_alternate(void);
void mode_bouncing_dark_window(void);
void mode_forward_reverse_ripple_fill(void);
void mode_test(void);
#define mapped_value adc_speed_read()

#define maxmode 2
int mode=0,curmode=0;
volatile unsigned char eeprom_write_ok = 1;
void controls_service(void)
{
#if ENABLE_MODE_BUTTON
    if (button_take_press()) {
        mode = (mode + 1) % maxmode;
        alloff();
        if (mode == 1) allon();
    }
#endif
}

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
	


/* Apply a 10-bit static ON/OFF frame; 16-bit masks suffice. */
unsigned int led_bit(int position)
{
    if (position < 0 || position >= channel) return 0U;
    return 1U << position;
}
void pattern_mask(unsigned int mask)
{
    int position;
    alloff();
    for (position = 0; position < channel; position++)
        if (mask & (1U << position)) ledon(position);
}

void main(void)
{
    int i;
    #asm
        sim
    #endasm

    /* OFF latches first; preserve PD1/SWIM and PD4/PD6 inputs. */
    alloff();
    PC_CR1 |= 0xF8;
    PC_CR2 &= (unsigned char)~0xF8;
    PC_DDR |= 0xF8;
    PA_CR1 |= 0x0E;
    PA_CR2 &= (unsigned char)~0x0E;
    PA_DDR |= 0x0E;
    PD_CR1 |= 0x0C;
    PD_CR2 &= (unsigned char)~0x0C;
    PD_DDR |= 0x0C;
    mode = DEFAULT_BOOT_MODE;
#if ENABLE_EEPROM_BOOT_CYCLE
    mode = (int)eeprom_read_byte(EEPROM_MODE_ADDR);
    if (mode >= maxmode || mode < 0) mode = DEFAULT_BOOT_MODE;
    eeprom_write_ok = eeprom_write_byte(EEPROM_MODE_ADDR,
        (unsigned char)((mode + 1) % maxmode));
#endif
    curmode = mode;
    adc_init();
    control_timer_init();

		#asm
        rim
        #endasm
	//	mode_test();

	while(1)  
    {
		
		
			controls_service();
            if(mode==0)
			{
									for(i=0;i<1 && mode==0;i++) mode_forward_three_strobe_fill();
                for(i=0;i<10 && mode==0;i++) mode_all_leds_strobe();
                for(i=0;i<3 && mode==0;i++) mode_inward_mirror_strobe_chase();
                for(i=0;i<3 && mode==0;i++) mode_inward_outward_mirror_strobe_chase();
                for(i=0;i<3 && mode==0;i++) mode_bouncing_dark_window();
                for(i=0;i<3 && mode==0;i++) mode_ping_pong_two_led_strobe_chase();
                for(i=0;i<3 && mode==0;i++) mode_forward_single_led_chase();
                for(i=0;i<3 && mode==0;i++) mode_outward_mirror_strobe_chase();
                for(i=0;i<3 && mode==0;i++) mode_dark_pair_mirror_chase();
                for(i=0;i<1 && mode==0;i++) mode_reverse_strobe_chain_fill();
                for(i=0;i<3 && mode==0;i++) mode_triple_strobe_burst_pause();
                for(i=0;i<1 && mode==0;i++) mode_ping_pong_triple_strobe_chase();
                for(i=0;i<3 && mode==0;i++) mode_comet_ping_pong();
                for(i=0;i<3 && mode==0;i++) mode_center_out_strobe_fill();
                for(i=0;i<3 && mode==0;i++) mode_inward_outward_mirror_two_led_strobe_chase();
                for(i=0;i<3 && mode==0;i++) mode_outward_mirror_fill_inward_empty();
                for(i=0;i<3 && mode==0;i++) mode_inward_mirror_strobe_fill();
                for(i=0;i<3 && mode==0;i++) mode_odd_eight_blinks_then_even_eight_blinks();
                for(i=0;i<1 && mode==0;i++) mode_forward_strobe_chain_fill();
                for(i=0;i<3 && mode==0;i++) mode_ping_pong_dark_chase();
                for(i=0;i<3 && mode==0;i++) mode_reverse_single_led_chase();
                for(i=0;i<3 && mode==0;i++) mode_forward_two_led_chase();
                for(i=0;i<3 && mode==0;i++) mode_forward_fill_forward_empty();
                for(i=0;i<3 && mode==0;i++) mode_two_ended_chase_and_stack();
                for(i=0;i<3 && mode==0;i++) mode_ping_pong_three_led_strobe_chase();
                for(i=0;i<3 && mode==0;i++) mode_forward_reverse_ripple_fill();
                for(i=0;i<3 && mode==0;i++) mode_expanding_contracting_band();
                for(i=0;i<3 && mode==0;i++) mode_forward_fill_reverse_empty();
                for(i=0;i<3 && mode==0;i++) mode_twin_chase_cross_through();
                for(i=0;i<3 && mode==0;i++) mode_half_row_ten_strobe_alternate();
                for(i=0;i<3 && mode==0;i++) mode_half_row_five_strobe_alternate();
                for(i=0;i<3 && mode==0;i++) mode_even_ten_blinks_then_odd_ten_blinks();
                for(i=0;i<3 && mode==0;i++) mode_ping_pong_split_pair_strobe_chase();
                for(i=0;i<3 && mode==0;i++) mode_ping_pong_double_strobe_chase();
                for(i=0;i<3 && mode==0;i++) mode_split_odd_even_double_strobe_fill();
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
    volatile int i,j;
    controls_service();
    if (mode != 0) return;
    for(i=0;i<l;i++) {
        controls_service();
        if (mode != 0) return;
        for(j=0;j<100;j++) { }
    }
}

void myDelay1(int l)
{
    myDelay(l);
}

/* Forward Fill / Forward Empty. */
void mode_forward_fill_forward_empty(void)
{
	int i;
	for(i=0;i<channel;i++)
	{
    ledon(i);  // Turn ON LED
		myDelay(mapped_value);
        if (mode != 0) return;
	}
	
	for(i=0;i<channel;i++)
	{
    ledoff(i) ; // Turn ON LED
		myDelay(mapped_value);
        if (mode != 0) return;
	}
	
		alloff();
}
/* Forward Single LED Chase. */
void mode_forward_single_led_chase(void)
{
	int i;
	for(i=0;i<channel;i++)
	{
							
            ledon(i);  // Turn ON LED
						
						myDelay(mapped_value);
        if (mode != 0) return;
						ledoff(i);
			
	}
	alloff();	
}

/* Ping-pong Dark Chase. */
void mode_ping_pong_dark_chase(void)
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
        if (mode != 0) return;

        ledon(i);               // Restore LED
    }
		
		for(i = channel-2; i >= 0; i--)
    {
        ledoff(i);              // Moving OFF LED

        myDelay(mapped_value);
        if (mode != 0) return;

        ledon(i);               // Restore LED
    }
		

    alloff();
}



/* Reverse Single LED Chase. */
void mode_reverse_single_led_chase(void)
{
	int i;
	for(i=0;i<channel;i++)
	{
            ledon(channel-1-i);  // Turn ON LED
						myDelay(mapped_value);
        if (mode != 0) return;
						ledoff(channel-1-i);
  }

}




/* Forward Two LED Chase. */
void mode_forward_two_led_chase(void)
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
        if (mode != 0) return;
	 }
	  ledoff(channel-2); //4
	 myDelay(mapped_value);
        if (mode != 0) return;
	  ledoff(channel-1);//3
	 myDelay(mapped_value);
        if (mode != 0) return;
	 
	
	 alloff();
	 
}

/* Forward Strobe Chain Fill. */
void mode_forward_strobe_chain_fill(void)
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
        if (mode != 0) return;
	 ledoff(i);
	
	 myDelay(mapped_value);
        if (mode != 0) return;
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
        if (mode != 0) return;
	 
	 if(i==(channel-2-j))
	 {
		 break;
	 }
	ledoff(i);
myDelay(mapped_value);
        if (mode != 0) return;
}
 if(i==(channel-2-j))
	 {
		 break;
	 }
}
}
alloff();
}

/* Ping-pong Double Strobe Chase. */
void mode_ping_pong_double_strobe_chase(void)
{
int i,j,k;
for(i=0;i<channel;i++)
{
	
	for(k=0;k<2;k++)
	{	
	ledon(i);
	myDelay(100);
        if (mode != 0) return;
	
	ledoff(i);
 	myDelay(mapped_value);
        if (mode != 0) return;
	}

}
for(i=channel-1;i>0;i--)
{
	
		
	for(k=0;k<2;k++)
	{	
	ledon(i);
	myDelay(100);
        if (mode != 0) return;
	
	 

	ledoff(i);
myDelay(mapped_value);
        if (mode != 0) return;
}

}

alloff();

}


/* Forward Fill / Reverse Empty. */
void mode_forward_fill_reverse_empty(void)
{
	int i=0,j=0;

	for(i=0;i<channel;i++)
	{
		ledon(i);
	
	 myDelay(mapped_value);
        if (mode != 0) return;

}
		for(i=channel-1;i>=0;i--)
	{
		ledoff(i);
	
	 myDelay(mapped_value);
        if (mode != 0) return;

}

alloff();
}

/* Inward Mirror Strobe Fill. */
void mode_inward_mirror_strobe_fill(void)
{
		
		int j=0,i=0;

	for(i=0;i<channel/2;i++)
	{
		
		ledon(i);
    ledon(channel-1-i);
    myDelay(40);
        if (mode != 0) return;

    ledoff(i);
    ledoff(channel-1-i);
		myDelay(mapped_value);
        if (mode != 0) return;
		
		ledon(i);
		ledon(channel-1-i);
		
	}
	for(i=channel-1;i>=channel/2;i--)
	{
	
		ledoff(i);
		ledoff(channel-1-i);
		myDelay(mapped_value);
        if (mode != 0) return;
	}


alloff();
}

/* Outward Mirror Fill / Inward Empty. */
void mode_outward_mirror_fill_inward_empty(void)
{
    int i, j;


        // Fill from center outward
        for(i = channel/2 - 1; i >= 0; i--)
        {
            ledon(i);
            ledon(channel - 1 - i);

            myDelay(mapped_value);
        if (mode != 0) return;
        }

        // Unfill from ends inward
        for(i = 0; i < channel/2; i++)
        {
            ledoff(i);
            ledoff(channel - 1 - i);

            myDelay(mapped_value);
        if (mode != 0) return;
        }
    

    alloff();
}
/* All LEDs Strobe: short fixed ON pulse; POT controls OFF interval. */
void mode_all_leds_strobe(void)
{
    int i;
    int j;
   
  
        for (i = 0; i < channel; i++)
        {
            ledon(i);
        }
        myDelay(20);
        if (mode != 0) return;
        for (i = 0; i < channel; i++)
        {
            ledoff(i);
        }
        myDelay(mapped_value);
        if (mode != 0) return;
   
    alloff();
}

/* Ping-pong Two LED Strobe Chase. */
void mode_ping_pong_two_led_strobe_chase(void)
{
    int i;

    // Forward
    for(i = 0; i < channel; i++)
    {
        if(i > 0)
            ledon(i - 1);

        ledon(i);

        myDelay(100);
        if (mode != 0) return;          // ON time

        if(i > 0)
            ledoff(i - 1);

        ledoff(i);

        myDelay(mapped_value);
        if (mode != 0) return; // OFF time
    }

    // Reverse
    for(i = channel - 1; i >= 0; i--)
    {
        if(i < channel - 1)
            ledon(i + 1);

        ledon(i);

        myDelay(100);
        if (mode != 0) return;          // ON time

        if(i < channel - 1)
            ledoff(i + 1);

        ledoff(i);

        myDelay(mapped_value);
        if (mode != 0) return; // OFF time
    }

    alloff();

}
/* Ping-pong Three LED Strobe Chase. */
void mode_ping_pong_three_led_strobe_chase(void)
{
   int i;

    // Forward
    for(i = 0; i < channel; i++)
    {
        if(i >= 2) ledon(i - 2);
        if(i >= 1) ledon(i - 1);
        ledon(i);

        myDelay(100);
        if (mode != 0) return;              // ON time

        if(i >= 2) ledoff(i - 2);
        if(i >= 1) ledoff(i - 1);
        ledoff(i);

        myDelay(mapped_value);
        if (mode != 0) return;     // OFF time
    }

    // Reverse
    for(i = channel - 1; i >= 0; i--)
    {
        if(i <= channel - 3) ledon(i + 2);
        if(i <= channel - 2) ledon(i + 1);
        ledon(i);

        myDelay(100);
        if (mode != 0) return;              // ON time

        if(i <= channel - 3) ledoff(i + 2);
        if(i <= channel - 2) ledoff(i + 1);
        ledoff(i);

        myDelay(mapped_value);
        if (mode != 0) return;     // OFF time
    }

    alloff();
}

/* Inward Outward Mirror Strobe Chase. */
void mode_inward_outward_mirror_strobe_chase(void)
{
       int i;

    // Inward
    for(i = 0; i < channel/2; i++)
    {
        ledon(i);
        ledon(channel-1-i);

        myDelay(100);
        if (mode != 0) return;

        ledoff(i);
        ledoff(channel-1-i);

        myDelay(mapped_value);
        if (mode != 0) return;
    }

    // Outward
    for(i = channel/2-1; i >= 0; i--)
    {
        ledon(i);
        ledon(channel-1-i);

        myDelay(100);
        if (mode != 0) return;

        ledoff(i);
        ledoff(channel-1-i);

        myDelay(mapped_value);
        if (mode != 0) return;
    }

    alloff();
}


/* Inward Mirror Strobe Chase. */
void mode_inward_mirror_strobe_chase()
{
  int i, j;


        for(i = 0; i < channel / 2; i++)
        {
            ledon(i);
            ledon(channel - 1 - i);

            myDelay(100);
        if (mode != 0) return;              // ON time

            ledoff(i);
            ledoff(channel - 1 - i);

            myDelay(mapped_value);
        if (mode != 0) return;     // OFF time
        }
    

    alloff();
	}

/* Outward Mirror Strobe Chase: center pair to outermost pair. */
void mode_outward_mirror_strobe_chase(void)
{
    int i;
    alloff();
    for (i = channel/2 - 1; i >= 0; i--)
    {
        ledon(i);
        ledon(channel - 1 - i);
        myDelay(100);
        if (mode != 0) return;
        ledoff(i);
        ledoff(channel - 1 - i);
        myDelay(mapped_value);
        if (mode != 0) return;
    }
    alloff();
}

// Paired LEDs in two halves move forward and back, flashing at each step.
/* Ping-pong Split Pair Strobe Chase. */
void mode_ping_pong_split_pair_strobe_chase(void)
{
   int i;

    // Inward
    for(i = 0; i < channel/2; i++)
    {
        ledon(i);
        ledon(i + channel/2);

        myDelay(100);
        if (mode != 0) return;

        ledoff(i);
        ledoff(i + channel/2);

        myDelay(mapped_value);
        if (mode != 0) return;
    }

    // Outward
    for(i = channel/2 - 1; i >= 0; i--)
    {
        ledon(i);
        ledon(i + channel/2);

        myDelay(100);
        if (mode != 0) return;

        ledoff(i);
        ledoff(i + channel/2);

        myDelay(mapped_value);
        if (mode != 0) return;
    }

    alloff();
}

// One LED flashes three times at each step, moving forward and back.
/* Ping-pong Triple Strobe Chase. */
void mode_ping_pong_triple_strobe_chase(void)
{
 int i,k;

    // Forward
    for(i = 0; i < channel; i++)
    {
			for(k=0;k<3;k++)
			{
        ledon(i);

        myDelay(100);
        if (mode != 0) return;          // Strobe ON

        ledoff(i);

        myDelay(mapped_value);
        if (mode != 0) return; // Strobe OFF
			}
    }

    // Reverse (skip last LED)
    for(i = channel - 2; i >= 0; i--)
    {
			for(k=0;k<3;k++)
			{
        ledon(i);

        myDelay(100);
        if (mode != 0) return;          // Strobe ON

        ledoff(i);

        myDelay(mapped_value);
        if (mode != 0) return; // Strobe OFF
			}
    }

    alloff();
}

/* Odd x8 / Even x8 Blink. */
void mode_odd_eight_blinks_then_even_eight_blinks(void)
{
	int i,j,k;

		for(k=0;k<8;k++)
		{
	for (i = 0; i < channel; i+=2)
        {
					
            ledon(i);
						
					
						
        }
				myDelay(100);
        if (mode != 0) return;
       
			
				alloff();
				 myDelay(mapped_value);
        if (mode != 0) return;
			}
				for(k=0;k<8;k++)
		{
	for (i = 1; i < channel; i+=2)
        {
            ledon(i);
						
        }
				myDelay(100);
        if (mode != 0) return;
						
       
			
			
				alloff();
				 myDelay(mapped_value );
        if (mode != 0) return;
			}
					
}

void mode_forward_three_strobe_fill(void) //flicker individual LEDs 
{
     int i, s;

    alloff();

    for(i = 0; i < channel; i++)
    {
        // Previous LEDs remain ON
        for(s = 0; s < 3; s++)   // Number of strobes
        {
            ledon(i);            // New LED ON

            myDelay(100);
        if (mode != 0) return;

            ledoff(i);           // New LED OFF

            myDelay(mapped_value);
        if (mode != 0) return;
        }

        // Leave new LED ON permanently
        ledon(i);
				myDelay(mapped_value);
        if (mode != 0) return;
    }

    

    alloff();
}



/* Odd x10 / Even x1 Blink. */


/* Even x10 / Odd x1 Blink. */
void mode_even_ten_blinks_then_odd_ten_blinks(void)
{
    int group, flash, position;
    alloff();
    for (group = 0; group < 2; group++) {
        for (flash = 0; flash < 10; flash++) {
            for (position = 0; position < channel; position++)
                if (position % 2 == (group == 0 ? 1 : 0)) ledon(position);
            myDelay(100);
            if (mode != 0) return;
            alloff();
            myDelay(mapped_value);
            if (mode != 0) return;
        }
    }
}
/* Reverse Strobe Chain Fill. */
void mode_reverse_strobe_chain_fill(void)
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
        if (mode != 0) return;
            ledoff(i);

            myDelay(mapped_value);
        if (mode != 0) return;
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
        if (mode != 0) return;

                if(i == (j + 1))
                {
                    break;
                }

                ledoff(i);
                myDelay(mapped_value);
        if (mode != 0) return;
            }

            if(i == (j + 1))
            {
                break;
            }
        }
    }

    alloff();
}

/* Split Odd/Even Double-strobe Fill.
 * Odd: 1,7,3,9,5; then clear all.
 * Even: 6,2,8,4,10; then clear all.
 * Two flashes per new LED; earlier LEDs remain ON. No reverse unfill.
 */
void mode_split_odd_even_double_strobe_fill(void)
{
    int group, step, position, flash;
    alloff();
    for (group = 0; group < 2; group++)
    {
        for (step = 0; step < channel/2; step++)
        {
            if (group == 0)
                position = (step % 2 == 0) ? step : step + channel/2;
            else
                position = (step % 2 == 0) ? step + channel/2 : step;
            for (flash = 0; flash < 2; flash++)
            {
                ledon(position);
                myDelay(100);
                if (mode != 0) return;
                ledoff(position);
                myDelay(mapped_value);
                if (mode != 0) return;
            }
            ledon(position);
            myDelay(mapped_value);
            if (mode != 0) return;
        }
        alloff();
        myDelay(mapped_value);
        if (mode != 0) return;
    }
}

/* Three fully ON LEDs form a moving tail; no PWM or dimming. */
void mode_comet_ping_pong(void)
{
    int step, position, direction;
    for (step = 0; step < 2*channel-2; step++) {
        position = step < channel ? step : 2*channel-2-step;
        direction = step < channel ? 1 : -1;
        pattern_mask(led_bit(position) | led_bit(position-direction) |
                     led_bit(position-2*direction));
        myDelay(mapped_value);
        if (mode != 0) return;
    }
    alloff();
}

void mode_twin_chase_cross_through(void)
{
    int position;
    for (position = 0; position < channel; position++) {
        pattern_mask(led_bit(position) | led_bit(channel-1-position));
        myDelay(mapped_value);
        if (mode != 0) return;
    }
    alloff();
}

void mode_center_out_strobe_fill(void)
{
    int position;
    unsigned int mask, pair;
    mask = 0U;
    for (position = channel/2-1; position >= 0; position--) {
        pair = led_bit(position) | led_bit(channel-1-position);
        pattern_mask(mask | pair);
        myDelay(100);
        if (mode != 0) return;
        pattern_mask(mask);
        myDelay(mapped_value);
        if (mode != 0) return;
        mask |= pair;
        pattern_mask(mask);
        myDelay(mapped_value);
        if (mode != 0) return;
    }
    alloff();
}

void mode_dark_pair_mirror_chase(void)
{
    int step, position;
    for (step = 0; step < channel-1; step++) {
        position = step < channel/2 ? step : channel-2-step;
        pattern_mask(((1U << channel)-1U) ^ (led_bit(position) | led_bit(channel-1-position)));
        myDelay(mapped_value);
        if (mode != 0) return;
    }
    alloff();
}

void mode_two_ended_chase_and_stack(void)
{
    int depth, position;
    unsigned int mask;
    mask = 0U;
    for (depth = 0; depth < channel/2; depth++) {
        for (position = 0; position <= channel/2-1-depth; position++) {
            pattern_mask(mask | led_bit(position) | led_bit(channel-1-position));
            myDelay(mapped_value);
            if (mode != 0) return;
        }
        mask |= led_bit(channel/2-1-depth) | led_bit(channel/2+depth);
        pattern_mask(mask);
        myDelay(mapped_value);
        if (mode != 0) return;
    }
    alloff();
}

void mode_expanding_contracting_band(void)
{
    int step, width, position;
    unsigned int mask;
    for (step = 0; step < channel; step++) {
        width = step < channel/2 ? step+1 : channel-1-step;
        mask = 0U;
        for (position = 0; position < width; position++)
            mask |= led_bit(channel/2-1-position) | led_bit(channel/2+position);
        pattern_mask(mask);
        myDelay(mapped_value);
        if (mode != 0) return;
    }
    alloff();
}

void mode_triple_strobe_burst_pause(void)
{
    int flash;
    for (flash = 0; flash < 3; flash++) {
        allon();
        myDelay(40);
        if (mode != 0) return;
        alloff();
        myDelay(flash == 2 ? mapped_value*3 : mapped_value);
        if (mode != 0) return;
    }
}

void mode_half_row_ten_strobe_alternate(void)
{
    int half, flash;
    for (half = 0; half < 2; half++) {
        for (flash = 0; flash < 10; flash++) {
            pattern_mask(half == 0 ? ((1U << (channel/2))-1U) : (((1U << (channel/2))-1U) << (channel/2)));
            myDelay(100);
            if (mode != 0) return;
            alloff();
            myDelay(mapped_value);
            if (mode != 0) return;
        }
    }
}

void mode_half_row_five_strobe_alternate(void)
{
    int half, flash;
    for (half = 0; half < 2; half++) {
        for (flash = 0; flash < 5; flash++) {
            pattern_mask(half == 0 ? ((1U << (channel/2))-1U) : (((1U << (channel/2))-1U) << (channel/2)));
            myDelay(100);
            if (mode != 0) return;
            alloff();
            myDelay(mapped_value);
            if (mode != 0) return;
        }
    }
}

void mode_bouncing_dark_window(void)
{
    int step, position;
    for (step = 0; step < 2*channel-6; step++) {
        position = step < channel-2 ? step : 2*channel-6-step;
        pattern_mask(((1U << channel)-1U) ^ (led_bit(position) |
                     led_bit(position+1) | led_bit(position+2)));
        myDelay(mapped_value);
        if (mode != 0) return;
    }
    alloff();
}

void mode_forward_reverse_ripple_fill(void)
{
    int reverse, position;
    unsigned int mask;
    for (reverse = 0; reverse < 2; reverse++) {
        mask = 0U;
        for (position = 0; position < channel; position++) {
            mask |= led_bit(reverse ? channel-1-position : position);
            pattern_mask(mask);
            myDelay(mapped_value);
            if (mode != 0) return;
        }
        for (position = 0; position < channel; position++) {
            mask &= ~led_bit(reverse ? channel-1-position : position);
            pattern_mask(mask);
            myDelay(mapped_value);
            if (mode != 0) return;
        }
    }
    alloff();
}

/* Two adjacent LEDs on EACH side move inward, then outward.
 * Four LEDs are ON per flash; mirrored blocks stop at the center.
 */
void mode_inward_outward_mirror_two_led_strobe_chase(void)
{
    int direction, step, position;
    alloff();
    for (direction = 0; direction < 2; direction++)
    {
        for (step = 0; step < channel/2 - 1; step++)
        {
            position = direction == 0 ? step : channel/2 - 2 - step;
            ledon(position);
            ledon(position+1);
            ledon(channel-1-position);
            ledon(channel-2-position);
            myDelay(100);
            if (mode != 0) return;
            alloff();
            myDelay(mapped_value);
            if (mode != 0) return;
        }
    }
}

// --- EEPROM Functions ---
/* Startup only, with interrupts masked; bounded polling and readback. */
unsigned char eeprom_write_byte(unsigned int address, unsigned char data)
{
    unsigned int timeout;
    unsigned char status;
    if (address < EEPROM_START_ADDR || address >= EEPROM_START_ADDR + 128U)
        return 0;
    FLASH_DUKR = 0xAE;
    FLASH_DUKR = 0x56;
    timeout = EEPROM_WAIT_LIMIT;
    while (!(FLASH_IAPSR & 0x08)) {
        if (--timeout == 0U) {
            FLASH_IAPSR &= (unsigned char)~0x08;
            return 0;
        }
    }
    status = FLASH_IAPSR;
    *(volatile unsigned char *)address = data;
    timeout = EEPROM_WAIT_LIMIT;
    do {
        status = FLASH_IAPSR;
        if (status & 0x05) break;
    } while (--timeout != 0U);
    FLASH_IAPSR &= (unsigned char)~0x08;
    return ((status & 0x05) == 0x04 &&
            *(volatile unsigned char *)address == data);
}

unsigned char eeprom_read_byte(unsigned int address)
{
    if (address >= EEPROM_START_ADDR && address < EEPROM_START_ADDR + 128U)
        return *(volatile unsigned char *)address;
    return 0xFF;
}
