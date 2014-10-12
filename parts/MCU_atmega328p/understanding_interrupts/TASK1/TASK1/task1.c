/*! \file *********************************************************************
 *
 * \brief
 *      Basic external interrupt usage on ATmega2560.
 *
 *      This task1.c file contains the routine that implements one external 
		interrupt and one pin change interrupt on ATmega2560.
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
#include "ext_interrupt_driver.h"
#include "ext_interrupt_driver.c"

/*! \brief Interrupt handler for External interrupt INT0
 *  Blinks LED0  */
ISR(INT0_vect)
{
	//Turn on LED0
	PORTA = 0xFE;

	//wait for some delay	
	for(int i=0;i<5000;i++);
	
	//Turn off LED0
	PORTA = 0xFF;	
}

/*! \brief Interrupt handler for Pin change interrupt PCINT0
 *  Blinks LED1  */
ISR(PCINT0_vect)
{
	//Turn on LED1
	PORTA = 0xFD;

	//wait for some delay	
	for(int i=0;i<5000;i++);

	//Turn off LED1	
	PORTA = 0xFF;	
}

/*! \brief Main routine
 *  Configures and enables external interrupt INT0
 *  Enables pin change interrupt PCINT0 */
int main(void)
{
	//Configure PORTA as output to drive LEDs
	DDRA = 0xFF;	

	//Turn off all LEDs
	PORTA = 0xFF;	

	//Enabling pull up in PORTB
	PORTB = 0xFF;	

	//Enabling pull up in PORTD
	PORTD = 0xFF;	
	
	//Configure INT0 to sense rising edge
	Configure_Interrupt(INTR0,RISING);
	
	//Enable INT0 interrupt	
	Enable_Interrupt(INTR0);

	//Enable pin change interrupt PCINT0 on PORTB0		
	Enable_Pcinterrupt(PCINTR0);		

	//Enable global interrupt enable bit
	sei();	
	
    while(1)
    {
        //Empty loop - Wait forever until interrupt occurs 
    }
}