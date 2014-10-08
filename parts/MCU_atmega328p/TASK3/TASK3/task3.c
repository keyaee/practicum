/*! \file *********************************************************************
 *
 * \brief
 *      Generating Software interrupt with external interrupt pin
 *	configured as output in ATmega2560.
 *
 *      This task3.c file contains the routine that implements one external 
		interrupt and configures Timer3 to produce 
 *		software interrupt on ATmega2560.
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

//Initialize a global variable for using in delay routines
int i=0;

/*! \brief Interrupt handler for External interrupt INT4
 *  Blinks LED0  */
ISR(INT4_vect)
{
	//Turn on LED0
	PORTA=(PORTA&(~(1<<PA0)))|(0<<PA0);	

	//Wait for some delay
	for(i=0;i<10000;i++);	

	//Turn off LED0	
	PORTA=(PORTA&(~(1<<PA0)))|(1<<PA0);	
}

/*! \brief Function that initializes Timer3
 *  Configuration registers  */
void Timer_Init()
{
	//Configure PORTE pin4 (INT4 pin/OC3B pin) as output
	DDRE=(DDRE&(~(1<<DDE4)))|(1<<DDE4);	

	//Configure Timer3 in CTC mode with OC3B pin toggling at compare match
	TCCR3A = (1<<COM3B0);	

	//Load some compare value
	OCR3A = 0x7FFF;	

	//Timer3 in CTC mode (OCR3A as TOP) and prescalar (divide by 256)
	TCCR3B = (1<<WGM32)|(0<<CS32)|(1<<CS31)|(1<<CS30);	
}

/*! \brief Main routine
 *  Configures and enables external interrupt INT4
 *  Calls timer3 initialization function */
int main(void)
{
	//Configure PORTA BIT0 as output to drive LED0
	DDRA=(DDRA&(~(1<<DDA0)))|(1<<DDA0);	

	//Turn off LED0
	PORTA=(PORTA&(~(1<<PA0)))|(1<<PA0);	

	//Configure INT4 to sense rising edge
	Configure_Interrupt(INTR4,RISING);	

	//Enable INT4
	Enable_Interrupt(INTR4);

	//Set global interrupt enable bit
	sei();	
	
	//Initialize Timer3
	Timer_Init();
	
    while(1)
    {
        //Empty loop - Wait forever until interrupt occurs 
    }
}