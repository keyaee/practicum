/*
 * 
 *
 * Created: 10/16/2014 2:47:21 PM
 *  Author: Carlos Mariscal
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "Controller_Driver_2.h"

unsigned char Previous_Read = 0x07;
unsigned char Current_Read  = 0x00;
volatile int  Confidence_Level = 0;
int main(void)
{
    //setting PB0-2 and B6-7 as inputs and PB3-5 as outputs
	DDRB= 0b00111000;
	
while(1)
    {
		Current_Read=PINB; //reading pins
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
unsigned char Button_Press(unsigned char Button_Value)
{
	unsigned char Output=0x00;
	int i;
	for (i=7;i>=0;i--)
	{
		if (Button_Value==Buttons[i])
		{
			PORTB=Output_Test[i];
		}
	}
	return Output;
}