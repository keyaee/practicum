/*definitions for controller buttons in hex
these HEX values are the outputs of the encoder
DO NOT USE THIS VALUES AS THE COMBINATION
******************************************************/
#define Select_Button 0x00
#define B_Button 0x01
#define A_Button 0x02
#define Up_button 0x03
#define Right_Button 0x04
#define Down_Button 0x05
#define Left_Button 0x06
#define NO_Press 0x07
//******************************************************

/* these are the HEX values taht will be sent to the LCD, most of teh values 
correspond to the ASCCII table, but not all do*/
#define UP		0x55
#define DOWN	0x44
#define RIGHT	0x7E
#define LEFT	0X7F
#define A		0x41
#define B		0x42
#define SELECT  0x10




unsigned char Buttons[8]={B_Button,A_Button,Up_button,Right_Button,Down_Button,Left_Button,NO_Press};


unsigned char Output [8]={B,A,UP,RIGHT,DOWN,LEFT,0x00};
	



/* this will be the "master" combination. the combination needs to be 10 inputs long. if the user desires 
to change combination simple change pattern bellow*/
 unsigned char Combination[10]={UP,UP,DOWN,DOWN,LEFT,RIGHT,LEFT,RIGHT,B,A};	
	// unsigned char Combination[10]={UP,UP,UP,UP,UP,DOWN,DOWN,DOWN,DOWN,DOWN};	
	
/* to do:
	add a volatile combination that will serve as a user defined combination
	the master will always work and is necessary to enter a new input combination 
*/





unsigned char Button_Press_Detected(unsigned char Button_Value);
unsigned char PollController();
void Check_For_Match(unsigned char Button_Pressed);
void Check_Combination();
void Reset_Return_Home();
