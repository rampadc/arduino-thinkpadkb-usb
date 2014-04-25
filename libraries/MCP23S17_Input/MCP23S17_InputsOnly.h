/* 
* MCP23S17_InputsOnly.h
*
* Created: 2/12/2013 5:05:48 PM
* Author: CONG
*/


#ifndef __MCP23S17_INPUTSONLY_H__
#define __MCP23S17_INPUTSONLY_H__

//MCP's related
#define OPCODEW		0x40
#define OPCODER		0x41
#define GPIOA		0x12
#define GPIOB		0x13
#define BIT7		0x80

//SPI related
#define CLOCKDIV	2

class MCP23S17_InputsOnly
{
//functions
public:
	MCP23S17_InputsOnly();
	~MCP23S17_InputsOnly();
	int readPin(int pin); //from pin 0 to pin 15
protected:
private:
	void writeData(unsigned char addr, unsigned char bData);
}; //MCP23S17_InputsOnly

#endif //__MCP23S17_INPUTSONLY_H__
