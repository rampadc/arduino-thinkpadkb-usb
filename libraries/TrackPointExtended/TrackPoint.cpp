/*
* TrackPoint.cpp
*
* Created: 14/03/2014 12:50:47 AM
* Author: CONG NGUYEN
*
* Expanding on Felix Kee's TrackPoint class: https://github.com/feklee/arduino-trackpoint
* Removed onesCounter and incomingParity
*/


#include "TrackPoint.h"

// default constructor
TrackPoint::TrackPoint(uint8_t clockPin, uint8_t dataPin, uint8_t resetPin, uint8_t streamMode) : _clkPin(clockPin), _dataPin(dataPin), _resetPin(resetPin)
{
	if(streamMode) {
		bitcount = 0;
		n = 0;
		val = 0;
		incoming = 0;
		counter = 0;
		dataAvailable = 0;
	}
	
	gohi(_clkPin);
	gohi(_dataPin);
} //TrackPoint

// default destructor
TrackPoint::~TrackPoint()
{
} //~TrackPoint

uint8_t TrackPoint::reportAvailable(void) {
	return dataAvailable;
}

TrackPoint::DataReport TrackPoint::getStreamReport(void) {
	return data;
}

void TrackPoint::getDataBit(void) {
	dataAvailable = 0;

	val = (digitalRead(_dataPin) ? 1 : 0);
	n = bitcount-1;
	if(n <= 7) {
		incoming |= (val << n);
	}
	bitcount++;

	if(bitcount == 11) {
		switch(counter) {
			case 0:
			data.state = incoming;
			counter++;
			break;

			case 1:
			data.x = incoming;
			counter++;
			break;

			case 2:
			data.y = incoming;
			counter = 0;
			dataAvailable = 1;
			break;
		}
		bitcount = 0;
		incoming = 0;
	}
}

// errors are ignored
void TrackPoint::writeToRamLocation(uint8_t location, uint8_t value) {
	write(0xe2);
	read(); // ACK
	write(0x81);
	read(); // ACK
	write(location);
	read(); // ACK
	write(value);
	read(); // ACK
}

// undefined in case of error
uint8_t TrackPoint::readFromRamLocation(uint8_t location) {
	write(0xe2);
	read(); // ACK
	write(0x80);
	read(); // ACK
	write(location);
	read(); // ACK
	return read();
}

void TrackPoint::setSensitivityFactor(uint8_t sensitivityFactor) {
	writeToRamLocation(0x4a, sensitivityFactor);
}

uint8_t TrackPoint::sensitivityFactor() {
	return readFromRamLocation(0x4a);
}

void TrackPoint::setRemoteMode() {
	write(0xf0);
	read();
}

void TrackPoint::setStreamMode() {
	write(0xea);
	read();
	write(0xf4); //enable report
	read();
	
	//put mouse into idle mode, ready to send
	gohi(_clkPin);
	gohi(_dataPin);
}

void TrackPoint::reset() {
	pinMode(_resetPin, OUTPUT);
	digitalWrite(_resetPin, HIGH);
	delay(2000); // empirical value
	digitalWrite(_resetPin, LOW);
}

TrackPoint::DataReport TrackPoint::readData() {
	DataReport d;

	write(0xeb);
	read(); // ACK
	d.state = read();
	d.x = read();
	d.y = read();

	return d;
}

// Comments and code below mostly unchanged from `ps2.h`.

/*
* according to some code I saw, these functions will
* correctly set the clock and data pins for
* various conditions.  It's done this way so you don't need
* pullup resistors.
*/
void TrackPoint::gohi(uint8_t pin) {
	pinMode(pin, INPUT);
	digitalWrite(pin, HIGH);
}

void TrackPoint::golo(uint8_t pin) {
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
}

/* write a uint8_t to the PS2 device */
void TrackPoint::write(uint8_t data) {
	uint8_t i;
	uint8_t parity = 1;

	gohi(_dataPin);
	gohi(_clkPin);
	delayMicroseconds(300);
	golo(_clkPin);
	delayMicroseconds(300);
	golo(_dataPin);
	delayMicroseconds(10);
	gohi(_clkPin);	// start bit
	/* wait for device to take control of clock */
	while (digitalRead(_clkPin) == HIGH)
	;	// this loop intentionally left blank
	// clear to send data
	for (i=0; i < 8; i++)
	{
		if (data & 0x01)
		{
			gohi(_dataPin);
			} else {
			golo(_dataPin);
		}
		// wait for clock
		while (digitalRead(_clkPin) == LOW)
		;
		while (digitalRead(_clkPin) == HIGH)
		;
		parity = parity ^ (data & 0x01);
		data = data >> 1;
	}
	// parity bit
	if (parity)
	{
		gohi(_dataPin);
		} else {
		golo(_dataPin);
	}
	// clock cycle - like ack.
	while (digitalRead(_clkPin) == LOW)
	;
	while (digitalRead(_clkPin) == HIGH)
	;
	// stop bit
	gohi(_dataPin);
	delayMicroseconds(50);
	while (digitalRead(_clkPin) == HIGH)
	;
	// mode switch
	while ((digitalRead(_clkPin) == LOW) || (digitalRead(_dataPin) == LOW))
	;
	// hold up incoming data
	golo(_clkPin);
}


/*
* read a uint8_t of data from the ps2 device.  Ignores parity.
*/
uint8_t TrackPoint::read(void) {
	uint8_t data = 0x00;
	uint8_t i;
	uint8_t bit = 0x01;

	// start clock
	gohi(_clkPin);
	gohi(_dataPin);
	delayMicroseconds(50);
	while (digitalRead(_clkPin) == HIGH)
	;
	delayMicroseconds(5);	// not sure why.
	while (digitalRead(_clkPin) == LOW)
	;	// eat start bit
	for (i=0; i < 8; i++)
	{
		while (digitalRead(_clkPin) == HIGH)
		;
		if (digitalRead(_dataPin) == HIGH)
		{
			data = data | bit;
		}
		while (digitalRead(_clkPin) == LOW)
		;
		bit = bit << 1;
	}
	// eat parity bit, ignore it.
	while (digitalRead(_clkPin) == HIGH)
	;
	while (digitalRead(_clkPin) == LOW)
	;
	// eat stop bit
	while (digitalRead(_clkPin) == HIGH)
	;
	while (digitalRead(_clkPin) == LOW)
	;
	golo(_clkPin);	// hold incoming data

	return data;
}