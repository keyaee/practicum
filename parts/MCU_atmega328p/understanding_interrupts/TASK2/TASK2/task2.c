/*! \file *********************************************************************
 *
 * \brief
 *      Nested external interrupt usage on ATmega2560.
 *
 *      This task2.c file contains the routine that implements two external 
		interrupts configured in nested operation on ATmega2560.
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
 *  Enables I-bit and Blinks LED0  */
ISR(INT0_vect)
{
	//Set global interrupt enable bit
	sei();	

	//Turn on LED0
	PORTB = (0<<PB0);

	//wait for some delay	
	for(int i=0;i<200;i++)	
		for(int j=0;j<625;j++);

	//Turn off LED0
	PORTB = (1<<PB0);	
}

/*! \brief Interrupt handler for External interrupt INT1
 *  Blinks LED1  */
ISR(INT1_vect)
{
	//Turn on LED1
	PORTC = (0<<PC0);

	//wait for some delay	
	for(int k=0;k<5000;k++);
	
	//Turn off LED1
	PORTC = (1<<PC0);	
}

/*! \brief Main routine
 *  Configures and enables external interrupt INT0
 *  and external interrupt INT1 */
int main(void)
{
	//Configure PORTB BIT0 as output to drive LED0
	DDRB = (1<<DDB0);	

	//Turn off LED0
	PORTB = (1<<PB0);

	//Configure PORTC BIT0 as output to drive LED1	
	DDRC = (1<<DDC0);	

	//Turn off LED1
	PORTC = (1<<PC0);	

	//Enabling pull up on PORTD
	PORTD = 0xFF;	
	
	//Configure INT0 sense rising edge
	Configure_Interrupt(INTR0,RISING);
	
	//Configure INT1 sense rising edge
	Configure_Interrupt(INTR1,RISING);	

	//Enable INT0 interrupt
	Enable_Interrupt(INTR0);

	//Enable INT1 interrupt	
	Enable_Interrupt(INTR1);

	//Set global interrupt enable bit	
	sei();	

    while(1)
    {
        //Empty loop - Wait forever until interrupt occurs 
    }
}