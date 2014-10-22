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
unsigned char Output_Debug[8]={0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00};




unsigned char Button_Press(unsigned char Button_Value);