//definitions for controller buttons in hex
#define Start_Button 0x00
#define B_Button 0x01
#define A_Button 0x02
#define Up_button 0x03
#define Right_Button 0x04
#define Down_Button 0x05
#define Left_Button 0x06
#define NO_Press 0x07


unsigned char Buttons[8]={Start_Button,B_Button,A_Button,Up_button,Right_Button,Down_Button,Left_Button,NO_Press};
unsigned char Output [8]={0x00,0x24,0x14,0x20,0xe7,0x10,0xf7,0x00};
//for debug this version of the code uses PORTC0-PORTC2 as LED outputs to debug the code
unsigned char Output_Debug_LED[8]={0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00};
//arbitarty combination I picked for debug up-down-right-left-A-B the array represents the outputs that I

//I am already using to output to the LEDs. will make volatile since we might change the combination at runtime.
volatile unsigned char Combination[6]={0x04,0x02,0x03,0x01,0x05,0x06};






unsigned char Button_Press_Detected(unsigned char Button_Value);
unsigned char PollController();
void Check_For_Match(unsigned char Button_Pressed);
void Check_Combination();
void Reset_Return_Home();