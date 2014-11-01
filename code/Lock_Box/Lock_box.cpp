// testing branching




/*
this code is used to debug the NES controller interface,external interrupts, and timer interrupts.
LEDs are used for debug and are placed on pin B0-B3 with some resistors in series ~1kohm.
the input pins from the encoder are PC0-PC3 

I have started to write the main loop for the lock box I have incorporated the code that i had and have left 
to do comments through out the code. 
this code is still mainly for debug using LED but does not differ much from the real code we will use
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "Controller_Driver_3.h"

unsigned char Previous_Read = 0x07; //variable to hold encoder status
unsigned char Current_Read  = 0x00;
int  Confidence_Level = 0;
volatile bool User_Input = false;
volatile int Input_Index = 0;
volatile int Combiantion_Match_counter = 0;
volatile bool Continue_Match_Check =true;
volatile bool No_Press_Flag = false;
volatile char Button_Held = 0x00;





/*interrupt routine to handle START button press. we will call "home screen" when 
the LCD displays "press START to begin" at the beginning of the program the LCD will be set
to home screen also after a determined time out the program will return to home screen.
while in home screen the user must press START to begin the unlocking process. in this interrupt
function the user input will be enable and the timer interrupt will also be enabled.*/
// toggles arbitrary LED on and off for debug
ISR(INT0_vect)
{
	int k=0;
	//debug++++++++++++++++++++++++++++++
	PORTB |= (1<<PORTB3); //turn red LED on to signify in user input mode
	PORTB &= ~(1<<PORTB4);// turn blue LED off
    //++++++++++++++++++++++++++++++++++++++++++
	
	EIMSK &= ~(1<<INT0); //disable interrupt on INT0
	TCNT1 = 0; //reset timer counter to 0
	TIFR1 |= (1<<OCF1A); //clear timer interrupt flag
	
	User_Input=true; // enable User input from NES controller
	
	//counter for debounce
	for (k=0;k<1000;k++);
	TIMSK1 |= 1<<OCIE1A; //enable interrupt for counter
	

	/*to add

		restart all counter i.e. Input_Index,Match, Combiantion_Match_counter, Cont_Match_Check 	*/
	
	
}

/*interrupt routine for internal counter.this will be used as our 
time out counter. after a determined time out the program will return to home screen were 
user input is disabled. to enable user input the user must press START which in turn causes
an interrupt described above*/
ISR(TIMER1_COMPA_vect)
{
	//for debug-toggle LED on PB4++++++++++++++++++++++++++++++++++++++++++
	PORTB |= (1<<PORTB4); //turn blue LED on to signify we are in at home screen
	PORTB &= ~(1<<PORTB3); //turn red LED off
	//++++++++++++++++++++++++++++++++++++++++++
	
	EIMSK |= (1<<INT0); //enable interrupt on INT0
	TIMSK1 &= ~(1<<OCIE1A); // disable interrupt for counter
	
	//disable user input from NES controller and return to home screen 
	User_Input=false;	
	
	/*to add
		 all counter i.e. Input_Index,Match, Combiantion_Match_counter, Cont_Match_Check */	
					User_Input=false;
					Combiantion_Match_counter=0;
					Input_Index=0;
					Continue_Match_Check =true;
}


int main(void)
{
    //setting PBC0-PC3 as inputs
	DDRC &= ~((1<<DDC0)|(1<<DDC1)|(1<<DDC2)|(1<<DDC3));//all port B as inputs
	PORTC |= ((1<<PORTC0)|(1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3));//enable pull-ups

	//for debug ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	DDRB |= ((1<<DDB0)|(1<<DDB1)|(1<<DDB2)|(1<<DDB3)|(1<<DDB4)|(1<<DDB5)|(1<<DDB6)); //pin PB0-PB3 as outputs
	PORTB = 0x00;// all off
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
	
	//configuring interrupt INT0
	DDRD &= ~(1<<DDD2); //PD2 as input
	PORTD |= (1<<PORTD2); //enable pull-up on PD2
	EICRA |= (1<<ISC01); //set interrupt on INT0 falling edge
	EIMSK |= (1<<INT0); //enable interrupt on INT0
	
	//timer setup
	//set prescale-divide clock by 1024 and enable clear after match
	//so counter runs at ~1kHz.
	TCCR1B |= (1<<CS12 | 1<<CS10 | 1<<WGM12); 
	OCR1A = 30000; // set value for counter every 1000=~ 1second
	
	sei(); //enable global interrupts
	
	PORTB |= (1<<PORTB4); //turn blue LED on to signify we are in at home screen
	
	//main program loop
	while(1)
	{
		
		//to do:
		//print to LCD :"Press SELCT to Start" 
		
		while (User_Input)
		{
			
			unsigned char Button_Pressed = 0x00;
			/*to do:
				in this block of code we will have to display the user input into 
				the LCD and also match the input to the predetermined combination.if 
				a match is reached we can then move out of this while statement and disable 
				user input, and open lock*/
			Button_Pressed = PollController();
			
			//if encoder output is 0x00 means no buttons have been pressed
			if ((Button_Pressed != 0x00) & (Button_Held != Button_Pressed))
			{
				Button_Held = Button_Pressed;
				No_Press_Flag = false;
				
				//this delay is to prevent multiply entries of the same button in a short period of time
				//_delay_ms(10);
				
				//to do:
				//print button pressed to LCD
				
				/*this if statement will only be true if the user pressed a button for the
				firsts time or if a partial match to the combination was made. this will allow us 
				to skip a matching inputs to the combination if the a wrong value was entered previously.*/
				if ((Input_Index==0) | Continue_Match_Check)
				{
					Check_For_Match(Button_Pressed);
					Input_Index++;
				}
				/* if we get into this else we already now that the combination entered is wrong,
				so we only need to increment a counter so we can keep track of when to alert
				 the user the combination is wrong i.e. wait till all user inputs have been entered*/
				else
				{
					Input_Index++;
				}
				
				
				//if the user has pressed a button 6 times lets check if 
				//the lock needs to open or not and return to home screen
				if (Input_Index>=6)
				{
					Check_Combination();
				}
				
			}
			
			
			if (No_Press_Flag)
			{
				Button_Held = 0x00;
			}
			
		}
			
	}
	return 0;
}

/*This function polls the NES controller for user input. the user input is 
processed by the Button_Press_Detected function which outputs a hex value 
that will be sent to the LCD and to compare for 
a match of correct combination.
Input:  none
Output: Hex value that will be sent to LCD and for combination match*/
unsigned char PollController()
{
		unsigned char Button_Pressed=0x00;
		Current_Read=PINC; //reading pins
		
		/********************************
		*********************************
		masking pins that don't matter
		will need to unmask bit4 if we want to detect START being pressed*/
		Current_Read= Current_Read & 0b00000111; 
		
		//this check will check the previous status of the pins for debouncing 
		if (Current_Read==Previous_Read)
		{
			//if the previous state equals current increase confidence level else so nothing
			Confidence_Level++;
			if(Confidence_Level>200) //arbitrary confidence level
			{
				Button_Pressed=Button_Press_Detected(Current_Read);
				Confidence_Level=0; //reset counter
				
				/*this statement is to set a flag variable when a no press has been detected.
				this is useful so we can track when the user has depressed a button.*/
				if (Button_Pressed==0x00)
				{
					No_Press_Flag = true;
				}
				
			}
		}
		Previous_Read=Current_Read; //store current state	
	
	return Button_Pressed;
}




/*This function takes the current state of the encoder and outputs 
the corresponding hex value to print out the user input to the LCD
Input: current state of encoder
Output: Hex value that will be sent to LCD*/
unsigned char Button_Press_Detected(unsigned char Button_Value)
{
	unsigned char Button_Pressed=0x00;
	int i;
	/*step through all possible values of the encoder output 
	and match it to a button being pressed or not*/
	for (i=7;i>=0;i--)
	{
		if (Button_Value==Buttons[i])
		{
			/*this block of code is only for debug purposes in the final code
			we do not have to output the value to GPIO pins we just need to output
			a desired value that uniquely identifies a button*/
			PORTB = PORTB & 0b11111000; //masking other values of PORTB
			PORTB = PORTB | Output_Debug_LED[i]; //writing to PRTB0-2
			Button_Pressed = Output_Debug_LED[i];
			
			// add real code here
			//Button_Pressed = some_array[i];
			
			
		}
	}
	return Button_Pressed;
}

void Check_For_Match(unsigned char Button_Pressed)
{
	/*if the button that was pressed matches to the correct combination
	given the index the we will increase the "correctness counter*/
	if (Button_Pressed == Combination[Input_Index])
	{
		Combiantion_Match_counter++; //increase correctness counter
	} 
	/* if the input does not match the correct combination then set the 
	continue checking variable to false. this will allow us to ignore future inputs. 
	i.e. makes no sense to check checking if one is already incorrect */
	else
	{
		Continue_Match_Check=false;
	}
}



void Check_Combination()
{
	//handle when the user has inputed more than 6 buttons
	if (Combiantion_Match_counter>=6)
	{
		//To do:
		//turn LCD green display "unlocking"
		//open lock
			
			
		//debug  +++++++++++++++++++++++++++++++
		PORTB |= 1<<PORTB6; //vibrator on  to signify open lock
		_delay_ms(300);
		PORTB &= ~(1<<PORTB6); //LED off
		PORTB = 0; //all LEDs off
		PORTB |=1<<PORTB4; //blue LED on to signify program is at home screen
		//++++++++++++++++++++++++++++++++++++++
			
		Reset_Return_Home();
			
	}
	else
	{
		//To do:
		//turn LCD red display "wrong code"
			
			
		//debug  +++++++++++++++++++++++++++++++
		PORTB |= 1<<PORTB5; //red LED no to signify code not correct
		_delay_ms(300);
		PORTB &= ~(1<<PORTB5); //LED off
		PORTB = 0; //all LEDs off
		PORTB |=1<<PORTB4; //blue LED on to signify program is at home screen
		//++++++++++++++++++++++++++++++++++++++
			
		Reset_Return_Home();
	}
}




void Reset_Return_Home()
{
	User_Input=false;
	Combiantion_Match_counter=0;
	Input_Index=0;
	Continue_Match_Check =true;	
	EIMSK |= (1<<INT0); //enable interrupt on INT0
	TCNT1 = 0; //reset timer counter to 0
	TIFR1 |= (1<<OCF1A); //clear timer interrupt flag
	TIMSK1 &= ~(1<<OCIE1A); // disable interrupt for counter
}