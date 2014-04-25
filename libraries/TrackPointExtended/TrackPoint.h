/*
* TrackPoint.h
*
* Created: 14/03/2014 12:50:48 AM
* Author: CONG NGUYEN
*
* Expanding on Felix Kee's TrackPoint class: https://github.com/feklee/arduino-trackpoint
*
* Interface with a TrackPoint, supports stream mode using interrupt
* Parity checks slow down move movements thus are removed
*/


#ifndef __TRACKPOINTEXTENDED_H__
#define __TRACKPOINTEXTENDED_H__

#include "Arduino.h"

class TrackPoint
{
	public:
	struct DataReport {
		uint8_t state;
		int8_t x;
		int8_t y;
	};

	void write(uint8_t data);
	uint8_t read(void);
	uint8_t readFromRamLocation(uint8_t);
	void writeToRamLocation(uint8_t, uint8_t);
	void setSensitivityFactor(uint8_t);
	uint8_t sensitivityFactor();
	void setRemoteMode();
	void reset();
	DataReport readData();
	
	TrackPoint(uint8_t, uint8_t, uint8_t, uint8_t);
	~TrackPoint();
		
	void getDataBit(void);
	uint8_t reportAvailable(void);
	void setStreamMode(void);
	DataReport getStreamReport(void);
	
	protected:
	
	private:
	TrackPoint( const TrackPoint &c );
	TrackPoint& operator=( const TrackPoint &c );
	void gohi(uint8_t pin);
	void golo(uint8_t pin);
	
	//TrackPoint pins
	uint8_t _clkPin;
	uint8_t _dataPin;
	uint8_t _resetPin;
	
	//PS2 data
	DataReport data;
	
	//getDataBit() variables
	volatile uint8_t bitcount;
	volatile uint8_t n;
	volatile uint8_t val;
	volatile uint8_t incoming;
	volatile uint8_t counter;
	volatile uint8_t dataAvailable;

}; //TrackPoint

#endif //__TRACKPOINT_H__
