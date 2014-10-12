/*! \file *********************************************************************
 *
 * \brief
 *      External interrupt for device wake-up of ATmega2560.
 *
 *      This task4.c file contains the routine that implements one external 
		interrupt used to wake-up ATmega2560 from power-down mode.
 *
 *      The driver is not intended for size and/or speed critical code, since
 *      most functions are just a few lines of code, and the function call
 *      overhead would decrease code performance. The driver is intended for
 *      rapid prototyping and documentation purposes for getting started with
 *      the ATmega2560 external interrupts.
 *
 * \par Application note:
 *      AVR1200: Using External Interrupts for megaAVR devices.
 *
 * \par Documentation
 *      For comprehensive code documentation, supported compilers, compiler
 *      settings and supported devices see readme.html
 *
 * \author
 *      Atmel Corporation: http://www.atmel.com \n
 *      Support email: avr@atmel.com
 *
 * Copyright (c) 2011, Atmel Corporation All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of ATMEL may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "ext_interrupt_driver.h"
#include "ext_interrupt_driver.c"

/*! \brief Interrupt handler for External interrupt INT0
 *  Turns ON LED0  */
ISR(INT0_vect)
{
	//Turn on LED0
	PORTA=(PORTA&(~(1<<PA0)))|(0<<PA0);	
}

/*! \brief Main routine
 *  Configures and enables external interrupt INT0
 *  Sets the device to power-down sleep mode */
int main(void)
{
	//Configure PORTA BIT0 as output to drive LED0
	DDRA=(DDRA&(~(1<<DDA0)))|(1<<DDA0);	

	//Turn off LED0
	PORTA=(PORTA&(~(1<<PA0)))|(1<<PA0);	

	//Enable pull-up on PORTB BIT0 to connect to switch SW0
	PORTB=(1<<PB0);	

	//Enable pull-up on PORTD BIT0 (INT0) to connect to switch SW1
	PORTD=(1<<PD0); 
	
	//Configure INT0 to sense rising edge
	Configure_Interrupt(INTR0,RISING);	

	//Enable INT0
	Enable_Interrupt(INTR0);

	//Set global interrupt enable bit
	sei();	
	
	//Set sleep mode to Power-Down mode
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);	
	sleep_enable();
	
	//Turn on LED0
	PORTA=(PORTA&(~(1<<PA0)))|(0<<PA0);	
	
	//Loop forever
    while(1)
    {
		//Wait until SW0 is pressed
        	while(PINB & (1<<PB0));	

		//Turn off LED0
		PORTA=(PORTA&(~(1<<PA0)))|(1<<PA0);	

		//Enter into sleep mode
		sleep_cpu();	
    }
}