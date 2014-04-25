/*
	Arduino sketch for ThinkPad T60 keyboard with TrackPoint
	Written for Arduino Micro, using:
	- MCP23S17 for columns inputs
	- CD4017 for rows outputs
	Can be adapted to any ThinkPad keyboards that share the same receptacle

	Coded in Atmel Studio 6 with VisualMicro.

	Created by CONG NGUYEN (26/04/2014)
*/

#include <SPI.h>
#include <stdint.h>
//Libraries
#include <TrackPointExtended/TrackPoint.h>
#include <MCP23S17_Input/MCP23S17_InputsOnly.h>

//Bits definitions
#define BIT0			(0x0001)
#define BIT1			(0x0002)
#define BIT2			(0x0004)
#define BIT3			(0x0008)
#define BIT4			(0x0010)
#define BIT5			(0x0020)
#define BIT6			(0x0040)
#define BIT7			(0x0080)

//Extra keycode definitions
#define KEY_VOLUME_UP	0
#define KEY_VOLUME_DOWN	0
#define KEY_MUTE		0
#define KEY_PREV_FILE	0
#define KEY_NEXT_FILE	0

#define KEY_PAUSE           0xD0
#define KEY_PRINT_SCREEN    0xCE
#define KEY_SCROLL_LOCK     0xCF
#define KEYPAD_NUMLOCK      0xDB
#define KEYPAD_0            0xEA
#define KEYPAD_1            0xE1
#define KEYPAD_2            0xE2
#define KEYPAD_3            0xE3
#define KEYPAD_4            0xE4
#define KEYPAD_5            0xE5
#define KEYPAD_6            0xE6
#define KEYPAD_7            0xE7
#define KEYPAD_8            0xE8
#define KEYPAD_9            0xE9
#define KEYPAD_SLASH        0xDC
#define KEYPAD_ASTERISK     0xDD
#define KEYPAD_MINUS        0xDE
#define KEYPAD_PLUS         0xDF
#define KEYPAD_ENTER        0xE0
#define KEYPAD_PERIOD       0xEB

//GPIO according to schematic
#define FN_PIN				7
#define CLOCK4017			9		//PB5
#define RESET4017			8		//PB4
#define TP_Data				3		//PD0
#define TP_Clock			2		//PD1
#define TP_Reset			6		//PD7
#define TP_Clock_Int		1		//if using stream mode, define clock pin's corresponding interrupt number

//TrackPoint variables
TrackPoint trackpoint(TP_Clock, TP_Data, TP_Reset, false);
void sendButtonState(uint8_t);

//Keyboard scanning related
MCP23S17_InputsOnly cols;
volatile int row = 0;
volatile int col = 0;
void rowsClock(void);
void rowsReset(void);

/***** KEYBOARD MATRIX WITHOUT NUMLOCK *******/
uint8_t MATRIX_WITHOUT_NUMLOCK[8][16] = {
	{'`',KEY_F1,KEY_F2,'5','6','=',KEY_F8,'-',KEY_F9,KEY_INSERT,KEY_DELETE,KEY_PAGE_UP,KEY_HOME,0,0,KEY_LEFT_CTRL},
	{'1','2','3','4','7','8','9','0',KEY_F10,KEY_F12,KEY_F11,KEY_PAGE_DOWN,KEY_END,KEY_PRINT_SCREEN,0,0},
	{'q','w','e','r','u','i','o','p',0,0,KEY_VOLUME_UP,KEY_LEFT_GUI,0,KEY_SCROLL_LOCK,0,0},
	{KEY_TAB,KEY_CAPS_LOCK,KEY_F3,'t','y',']',KEY_F7,'[',KEY_BACKSPACE,0,KEY_VOLUME_DOWN,0,0,0,KEY_LEFT_SHIFT,0},
	{'a','s','d','f','j','k','l',';','\\',0,KEY_MUTE,KEY_RIGHT_GUI,0,0,0,0},
	{KEY_ESC,0,KEY_F4,'g','h',KEY_F6,0,'\'',KEY_F5,0,0,0,KEY_UP_ARROW,KEY_LEFT_ALT,0,0},
	{'z','x','c','v','m',',','.',0,KEY_RETURN,0,0,KEY_PREV_FILE,KEY_PAUSE,0,KEY_RIGHT_SHIFT,KEY_RIGHT_CTRL},
	{0,0,0,'b','n',0,0,'/',' ',KEY_RIGHT_ARROW,KEY_DOWN_ARROW,KEY_NEXT_FILE,KEY_LEFT_ARROW,KEY_RIGHT_ALT,0,0}
};
/******* SKETCH STARTS HERE ********/
void initGPIO(void);

void setup()
{
	//MCP23S17 has inputs by default
	pinMode(FN_PIN, INPUT_PULLUP);				//active low
	pinMode(CLOCK4017, OUTPUT);
	pinMode(RESET4017, OUTPUT);
	
	Keyboard.begin();
	Mouse.begin();
	
	Serial.begin(9600);
	
	trackpoint.reset();
	trackpoint.setSensitivityFactor(0xC0);
	trackpoint.setRemoteMode();
}

void loop()
{
	//TrackPoint in report mode
	TrackPoint::DataReport d = trackpoint.readData();
	if (d.state & (1 << 2)) { // middle button down => scroll
		Mouse.move(0, 0, d.y);
		} else {
		Mouse.move(d.x, -d.y, 0);
		sendButtonState(d.state);
	}
	
	//Keyboard scanning
	if(row < 8) {
		for(col = 0; col < 16; col++) {
			if(cols.readPin(col)) {
				//current key is pressed								
				Keyboard.press(MATRIX_WITHOUT_NUMLOCK[row][col]);
			} else {
				//current key is not pressed
				Keyboard.release(MATRIX_WITHOUT_NUMLOCK[row][col]);
			}
		}
		row++;
		rowsClock();
	} else {
		row = 0;
		rowsReset();
	}
}

/******* FUNCTIONS IMPLEMENTATIONS **********/

/*
	Direct port manipulation to increase speed
	CLOCK4017			9		//PB5
	RESET4017			8		//PB4
*/

void rowsClock(void) {
	PORTB |= BIT5;
	PORTB &= ~BIT5;
}

void rowsReset(void) {
	PORTB |= BIT4;
	PORTB &= ~BIT4;
}

//From Felix Klee's Trackpoint library
void sendButtonState(byte state) {
	static const char hidStates[] = {MOUSE_LEFT, MOUSE_RIGHT};

	for (byte i = 0; i < sizeof(hidStates); i++) {
		byte hidState = hidStates[i];
		if (state & (1 << i)) {
			Mouse.press(hidState);
			} else if (Mouse.isPressed(hidState)) {
			Mouse.release(hidState);
		}
	}
}
