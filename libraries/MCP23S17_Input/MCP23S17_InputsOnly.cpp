/* 
* MCP23S17_InputsOnly.cpp
* 
* Created: 2/12/2013 5:05:47 PM
* Author: CONG
*/


#include "MCP23S17_InputsOnly.h"
#include <SPI.h>
#include <Arduino.h>
#include <stdint.h>

/*
	By default:
	- Use byte mode and bank 0
	- All pins are inputs
	
	Pin A0 is connected to ~CS of MCP23S17
*/

// default constructor
MCP23S17_InputsOnly::MCP23S17_InputsOnly()
{
	pinMode(A0, OUTPUT);
	SPI.setClockDivider(CLOCKDIV);
	SPI.begin();
	
} //MCP23S17_InputsOnly	

// default destructor
MCP23S17_InputsOnly::~MCP23S17_InputsOnly()
{
} //~MCP23S17_InputsOnly

int MCP23S17_InputsOnly::readPin(int pin) {
	if(pin < 0 || pin > 15) return 0;
	
	uint8_t portState = 0;
	uint8_t gpio = (pin < 8 ? GPIOA : GPIOB);
	PORTF &= ~BIT7;
	SPI.transfer(OPCODER);
	SPI.transfer(gpio);
	portState = SPI.transfer(0);
	//Serial.println(portState, BIN);
	PORTF |= BIT7;
	
	pin = (pin < 8 ? pin : pin-8);
	portState &= (1 << pin); //get that bit only but with all the trailing zeroes if there are any
	
	return (portState > 0 ? 1 : 0);
}

void MCP23S17_InputsOnly::writeData(uint8_t addr, uint8_t bData) {
	PORTF &= ~BIT7;
	SPI.transfer(OPCODEW);
	SPI.transfer(addr);
	SPI.transfer(bData);
	PORTF |= BIT7;
}