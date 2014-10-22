/*
this code is used to debug the NES controller interface,external interrupts, and timer interrupts.
LEDs are used for debug and are placed on pin B0-B3 with some resistors in series ~1kohm.
the input pins from the encoder are PC0-PC3 
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "Controller_Driver_3.h"

unsigned char Previous_Read = 0x07;
unsigned char Current_Read  = 0x00;
volatile int  Confidence_Level = 0;

//interrupt routine to handle SELECT button press
// toggles arbitrary LED on and off for debug
ISR(INT0_vect)
{
	int k=0;
	PORTB ^= (1<<PORTB3); //toggle LED
	//counter for debounce
	for (k=0;k<1000;k++);
}

//interrupt routine for internal counter.this will be used as our 
//time out counter.
ISR(TIMER1_COMPA_vect)
{
	PORTB ^= 1<<PORTB4;
}


int main(void)
{
    //setting PBC0-PC3 as inputs
	DDRC &= ~((1<<DDC0)|(1<<DDC1)|(1<<DDC2)|(1<<DDC3));//all port B as inputs
	PORTC |= ((1<<PORTC0)|(1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3));//enable pull-ups

	
	DDRB |= ((1<<DDB0)|(1<<DDB1)|(1<<DDB2)|(1<<DDB3)|(1<<DDB4)); //pin PB0-PB3 as outputs
	PORTB = 0x00;// all off
	 
	
	//configuring interrupt INT0
	DDRD &= ~(1<<DDD2); //PD2 as input
	PORTD |= (1<<PORTD2); //enable pull-up on PD2
	EICRA |= (1<<ISC01); //set interrupt on INT0 falling edge
	EIMSK |= (1<<INT0); //enable interrupt on INT0
	
	//timer setup
	//set prescale-divide clock by 1024 and enable clear after match
	//so counter runs at ~1kHz.
	TCCR1B |= (1<<CS12 | 1<<CS10 | 1<<WGM12); 
	TIMSK1 |= 1<<OCIE1A; //enable interrupt for counter
	OCR1A = 3000; // set value for counter every 1000=~ 1second
	
	sei(); //enable global interrupts
	
while(1)
    {
		Current_Read=PINC; //reading pins
		Current_Read= Current_Read & 0b00000111; //masking pins that don't matter
		//this check will check the previous status of the pins for debouncing 
		if (Current_Read==Previous_Read)
		{
			//if the previous state equals current increase confidence level
			Confidence_Level++;
			if(Confidence_Level>200) //arbitrary confidence level
			{
				Button_Press(Current_Read);
				Confidence_Level=0; //reset counter
			}
		}
		Previous_Read=Current_Read; //store current state	
	}
	return 0;
	
}

/*This function takes the current state of the encoder and outputs 
the corresponding hex value to print out the user input to the LCD
Input: current state of encoder
Output: Hex value that will be sent to LCD*/
unsigned char Button_Press(unsigned char Button_Value)
{
	unsigned char Output=0x00;
	int i;
	for (i=7;i>=0;i--)
	{
		if (Button_Value==Buttons[i])
		{
			
			PORTB=PORTB & 0b11111000; //masking other values of PORTB
			PORTB=PORTB | Output_Debug[i]; //writing to PRTB0-2
		
		}
	}
	return Output;
}